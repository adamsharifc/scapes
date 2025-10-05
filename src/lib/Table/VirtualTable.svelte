<script>
	import "./VirtualTable.css";
	/**
	 * Virtual table component that renders only visible rows for performance
	 * @param {any[]} items - Array of data items to display
	 * @param {number} itemHeight - Height of each row in pixels - Necessary for virtualization
	 * @param {Function} sortBy - Function to handle column sorting
	 * @param {Function} getSortIndicator - Function to get sort direction indicator
	 * @param {Function} g - Safe getter function for object properties
	 */
	let {
		items = [],
		itemHeight = 40,
		sortBy,
		getSortIndicator,
		g
	} = $props();

	let scrollContainer = $state();
	let tableEl = $state();
	let colLeftOffsets = $state([]); // left offsets (px) for visible columns
	let colWidths = $state([]); // measured widths for visible columns
	let scrollTop = $state(0);
	let scrollLeft = $state(0);
	let containerHeight = $state(400); // Default fallback height

	// Column configuration (pixel widths that can exceed viewport)
	/**
	 * @typedef {{key:string,label:string,width:number,visible?:boolean,sticky?:'left'|'right'|null}} Column
	 */
	/** @type {Column[]} */
	let columns = $state([
		{ key: 'id', label: 'ID', width: 150, visible: true, sticky: null },
		{ key: 'first_name', label: 'First Name', width: 300, visible: true, sticky: null },
		{ key: 'last_name', label: 'Last Name', width: 300, visible: true, sticky: null },
		{ key: 'email', label: 'Email', width: 400, visible: true, sticky: null },
		{ key: 'gender', label: 'Gender', width: 200, visible: true, sticky: null },
		{ key: 'ip_address', label: 'IP Address', width: 250, visible: true, sticky: null }
	]);

	// Resizing state
	/** @type {boolean} */
	let isResizing = $state(false);
	/** @type {number} */
	let resizeIndex = $state(-1);
	/** @type {number} */
	let startX = $state(0);
	/** @type {number[]} */
	let startWidths = $state([]); // pixel widths at start
	/** Minimum column width in pixels */
	const MIN_PX = 50;

	// Selection state
	/** @type {Set<string|number>} - Selected row IDs */
	let selectedRows = $state(new Set());
	/** @type {string|number|null} - Last selected row for shift selection */
	let lastSelectedRow = $state(null);
	/** @type {boolean} - Detect if we're on macOS */
	const isMac = typeof navigator !== 'undefined' && /Mac|iPod|iPhone|iPad/.test(navigator.platform);

	// Calculate total table width
	const totalTableWidth = $derived(() => {
		// prefer measured widths when available
		const visible = columns.filter(col => col.visible !== false);
		if (colWidths && colWidths.length === visible.length) {
			return colWidths.reduce((s, w) => s + (w || 0), 0);
		}
		// fallback to the configured widths (if any)
		return visible.reduce((sum, col) => sum + (col.width || 0), 0);
	});

	/**
	 * Get the style for sticky positioning
	 * @param {Column} col
	 * @param {number} index
	 * @param {boolean} isHeader
	 * @returns {string}
	 */
	function getStickyStyle(col, index, isHeader = false) {
		let styles = [];
		
		// Headers always need top: 0 for vertical sticking
		if (isHeader) {
			styles.push('top: 0');
		}
		
		// Handle horizontal sticking for both headers and body cells
		if (col.sticky) {
			const visibleColumns = columns.filter(c => c.visible !== false);
			// find visible index for this column
			const visIndex = visibleColumns.indexOf(col);
			if (visIndex === -1) return styles.length > 0 ? styles.join('; ') : '';

			if (col.sticky === 'left') {
				const left = (colLeftOffsets[visIndex] || 0);
				styles.push(`left: ${left}px`);
			} else if (col.sticky === 'right') {
				// compute right offset from measured table scroll width
				const tableWidth = tableEl ? (tableEl.scrollWidth || tableEl.offsetWidth) : totalTableWidth();
				const left = (colLeftOffsets[visIndex] || 0);
				const width = (colWidths[visIndex] || 0);
				const right = Math.max(0, Math.round(tableWidth - (left + width)));
				styles.push(`right: ${right}px`);
			}
		}
		
		return styles.length > 0 ? styles.join('; ') : '';
	}

	/**
	 * Start resizing a column (pointerdown on the resizer)
	 * @param {number} index
	 * @param {PointerEvent} e
	 */
	function startResize(index, e) {
		// Only start on primary button
		if (e.button && e.button !== 0) return;
		if (!scrollContainer) return;
		isResizing = true;
		resizeIndex = index;
		startX = e.clientX || 0;
		// save starting widths in pixels
		startWidths = columns.map((c) => c.width);

		// attempt to capture pointer on the element that received the event
		try {
			const target = /** @type {Element & { setPointerCapture?: (id:number)=>void } } */ (e.currentTarget);
			target && target.setPointerCapture && target.setPointerCapture(e.pointerId);
		} catch (err) {}
	}

	/** @param {PointerEvent} e */
	function onPointerMove(e) {
		if (!isResizing) return;
		const dx = (e.clientX || 0) - startX;

		// Only resize the column being dragged - don't affect other columns
		const newWidth = Math.max(MIN_PX, startWidths[resizeIndex] + dx);
		
		columns = columns.map((c, i) => {
			if (i === resizeIndex) return { ...c, width: newWidth };
			return c;
		});
	}

	/** @param {PointerEvent} e */
	function endResize(e) {
		if (!isResizing) return;
		isResizing = false;
		resizeIndex = -1;
		startWidths = [];
		try {
			const target = /** @type {Element & { releasePointerCapture?: (id:number)=>void } } */ (e.currentTarget);
			target && target.releasePointerCapture && target.releasePointerCapture(e.pointerId);
		} catch (err) {}
	}

	// Attach pointermove/up listeners globally while component is mounted
	$effect(() => {
		window.addEventListener('pointermove', onPointerMove);
		window.addEventListener('pointerup', endResize);
		window.addEventListener('pointercancel', endResize);
		return () => {
			window.removeEventListener('pointermove', onPointerMove);
			window.removeEventListener('pointerup', endResize);
			window.removeEventListener('pointercancel', endResize);
		};
	});

	// Context menu state
	let contextOpen = $state(false);
	let contextColumnIndex = $state(-1);

	import { createPopperActions } from 'svelte-popperjs';
	

	// Add offset + preventOverflow so the menu doesn't get clipped under the sticky header
	const [referenceAction, contentAction] = createPopperActions({ placement: 'bottom-start', modifiers: [
		{ name: 'offset', options: { offset: [0, 8] } },
		{ name: 'preventOverflow', options: { padding: 8 } }
	] });

	// We attach the popper reference action to an invisible DOM anchor element
	// so the action is created during component init (avoids lifecycle_outside_component).
	let virtualAnchor = $state();

	/**
	 * @param {number} index
	 * @param {MouseEvent} event
	 */
	function openContextMenu(index, event) {
		event.preventDefault();
		contextColumnIndex = index;
		contextOpen = true;
		// position the invisible anchor at the mouse coordinates so popper appears at the click origin
		const x = (/** @type {MouseEvent} */ (event)).clientX || 0;
		const y = (/** @type {MouseEvent} */ (event)).clientY || 0;
		if (virtualAnchor) {
			virtualAnchor.style.left = x + 'px';
			virtualAnchor.style.top = y + 'px';
			virtualAnchor.style.display = 'block';
		}
	}

	function closeContextMenu() {
		contextOpen = false;
		contextColumnIndex = -1;
		if (virtualAnchor) {
			virtualAnchor.style.display = 'none';
			virtualAnchor.style.left = '-9999px';
			virtualAnchor.style.top = '-9999px';
		}
	}

	/** @param {number} index */
	function toggleColumnVisibility(index) {
		columns = columns.map((c, i) => i === index ? { ...c, visible: !c.visible } : c);
		closeContextMenu();
	}

	/**
	 * @param {number} index
	 * @param {boolean} visible
	 */
	function setColumnVisibility(index, visible) {
		columns = columns.map((c, i) => i === index ? { ...c, visible } : c);
	}

	/**
	 * Toggle sticky position for a column
	 * @param {number} index
	 * @param {'left'|'right'|null} position
	 */
	function setColumnSticky(index, position) {
		columns = columns.map((c, i) => i === index ? { ...c, sticky: position } : c);
	}

	/**
	 * Move column left in the order
	 * @param {number} index
	 */
	function moveColumnLeft(index) {
		if (index <= 0) return; // Can't move first column left
		
		const newColumns = [...columns];
		const temp = newColumns[index];
		newColumns[index] = newColumns[index - 1];
		newColumns[index - 1] = temp;
		columns = newColumns;
		
		// Update context menu index to follow the moved column
		contextColumnIndex = index - 1;
		closeContextMenu();
	}

	/**
	 * Move column right in the order
	 * @param {number} index
	 */
	function moveColumnRight(index) {
		if (index >= columns.length - 1) return; // Can't move last column right
		
		const newColumns = [...columns];
		const temp = newColumns[index];
		newColumns[index] = newColumns[index + 1];
		newColumns[index + 1] = temp;
		columns = newColumns;
		
		// Update context menu index to follow the moved column
		contextColumnIndex = index + 1;
		closeContextMenu();
	}

	/**
	 * Check if column can move left
	 * @param {number} index
	 * @returns {boolean}
	 */
	function canMoveLeft(index) {
		return index > 0;
	}

	/**
	 * Check if column can move right
	 * @param {number} index
	 * @returns {boolean}
	 */
	function canMoveRight(index) {
		return index < columns.length - 1;
	}

	// Row selection functions
	
	/**
	 * Handle row click with modifier keys for selection
	 * @param {MouseEvent} event
	 * @param {any} item
	 */
	function handleRowClick(event, item) {
		const rowId = g(item, 'id');
		const isCtrlOrCmd = isMac ? event.metaKey : event.ctrlKey;
		const isShift = event.shiftKey;
		
		if (isShift && lastSelectedRow !== null) {
			// Shift+Click: Range selection
			selectRowRange(lastSelectedRow, rowId);
		} else if (isCtrlOrCmd) {
			// Ctrl/Cmd+Click: Toggle selection
			toggleRowSelection(rowId);
		} else {
			// Normal click: Single selection
			selectSingleRow(rowId);
		}
	}

	/**
	 * Select a single row, clearing all others
	 * @param {string|number} rowId
	 */
	function selectSingleRow(rowId) {
		selectedRows = new Set([rowId]);
		lastSelectedRow = rowId;
	}

	/**
	 * Toggle selection of a single row
	 * @param {string|number} rowId
	 */
	function toggleRowSelection(rowId) {
		const newSelection = new Set(selectedRows);
		if (newSelection.has(rowId)) {
			newSelection.delete(rowId);
			lastSelectedRow = newSelection.size > 0 ? rowId : null;
		} else {
			newSelection.add(rowId);
			lastSelectedRow = rowId;
		}
		selectedRows = newSelection;
	}

	/**
	 * Select range of rows between two row IDs
	 * @param {string|number} startRowId
	 * @param {string|number} endRowId
	 */
	function selectRowRange(startRowId, endRowId) {
		// Find indices of start and end rows in the full items array
		const startIndex = items.findIndex(item => g(item, 'id') === startRowId);
		const endIndex = items.findIndex(item => g(item, 'id') === endRowId);
		
		if (startIndex === -1 || endIndex === -1) return;
		
		// Determine range boundaries
		const minIndex = Math.min(startIndex, endIndex);
		const maxIndex = Math.max(startIndex, endIndex);
		
		// Select all rows in the range
		const newSelection = new Set(selectedRows);
		for (let i = minIndex; i <= maxIndex; i++) {
			const rowId = g(items[i], 'id');
			newSelection.add(rowId);
		}
		
		selectedRows = newSelection;
		lastSelectedRow = endRowId;
	}

	/**
	 * Clear all row selections
	 */
	function clearSelection() {
		selectedRows = new Set();
		lastSelectedRow = null;
	}

	/**
	 * Check if a row is selected
	 * @param {any} item
	 * @returns {boolean}
	 */
	function isRowSelected(item) {
		return selectedRows.has(g(item, 'id'));
	}

	/**
	 * Get count of selected rows
	 * @returns {number}
	 */
	function getSelectedCount() {
		return selectedRows.size;
	}

	/**
	 * Safely handle checkbox change events from the control panel
	 * @param {number} index
	 * @param {Event} e
	 */
	function onColumnCheckboxChange(index, e) {
		const target = /** @type {HTMLInputElement | null} */ (e && e.target ? e.target : null);
		const checked = !!(target && target.checked);
		setColumnVisibility(index, checked);
	}

	// Calculate visible range based on scroll position
	const visibleRange = $derived(() => {
		const start = Math.floor(scrollTop / itemHeight);
		const visibleCount = Math.ceil(containerHeight / itemHeight);
		const end = Math.min(start + visibleCount + 2, items.length); // +2 for buffer
		return { start: Math.max(0, start - 1), end }; // -1 for buffer
	});

	// Get only the visible items
	const visibleItems = $derived(() => {
		const { start, end } = visibleRange();
		return items.slice(start, end).map((item, index) => ({
			...item,
			virtualIndex: start + index
		}));
	});

	// Total height of all items
	const totalHeight = $derived(() => items.length * itemHeight);

	// Offset for the visible items container
	const offsetY = $derived(() => visibleRange().start * itemHeight);

	/** @param {Event} e */
	function handleScroll(e) {
		if (e.target && 'scrollTop' in e.target) {
			scrollTop = Number(e.target.scrollTop) || 0;
		}
		if (e.target && 'scrollLeft' in e.target) {
			scrollLeft = Number(e.target.scrollLeft) || 0;
		}
	}

	function handleResize() {
		if (scrollContainer) {
			containerHeight = scrollContainer.clientHeight;
		}
	}

	// Update container height when mounted and on resize
	$effect(() => {
		if (scrollContainer) {
			// Set initial height
			containerHeight = scrollContainer.clientHeight;
			
			// Set up ResizeObserver for responsive behavior
			const resizeObserver = new ResizeObserver(() => {
				if (scrollContainer) {
					containerHeight = scrollContainer.clientHeight;
				}
			});
			resizeObserver.observe(scrollContainer);
			
			return () => resizeObserver.disconnect();
		}
	});

	// Measure column positions and widths to compute sticky offsets
	function computeOffsets() {
		if (!tableEl) return;
		const headerRow = tableEl.querySelector('thead tr');
		if (!headerRow) return;
		const ths = Array.from(headerRow.querySelectorAll('th'))
			.filter((th) => th.offsetParent !== null); // only visible

		const lefts = ths.map((th) => th.offsetLeft);
		const widths = ths.map((th) => th.offsetWidth);
		colLeftOffsets = lefts;
		colWidths = widths;
	}

	// Recompute offsets when table mounts and when columns or container resize
	$effect(() => {
		computeOffsets();
		const roTargets = [];
		if (tableEl) {
			const ro = new ResizeObserver(() => computeOffsets());
			ro.observe(tableEl);
			roTargets.push(ro);
		}
		if (scrollContainer) {
			const ro2 = new ResizeObserver(() => computeOffsets());
			ro2.observe(scrollContainer);
			roTargets.push(ro2);
		}

		return () => {
			roTargets.forEach(r => r.disconnect && r.disconnect());
		};
	});

	// Recompute offsets when columns metadata changes (visibility, widths, order)
	// Use $effect with a single function argument and reference `columns` so
	// Svelte/Rune tracks the dependency correctly. Schedule measurement on
	// next microtask so DOM updates finish first.
	$effect(() => {
		// reference columns to ensure this effect reruns when it changes
		const _cols = columns;
		Promise.resolve().then(() => computeOffsets());
	});
</script>

<!-- Control panel: re-enable hidden columns -->
<div class="virtual-table-wrapper">
	<div class="table-controls">
		<div class="columns-toggle">
			{#each columns as col, i}
				<label class="col-toggle">
		<input type="checkbox" checked={col.visible !== false} onchange={(e) => onColumnCheckboxChange(i, e)} />
					{col.label}
				</label>
			{/each}
			<button class="show-all" onclick={() => columns = columns.map(c => ({ ...c, visible: true }))}>Show all</button>
		</div>
		<div class="selection-info">
			{#if getSelectedCount() > 0}
				<span class="selected-count">{getSelectedCount()} row{getSelectedCount() === 1 ? '' : 's'} selected</span>
				<button class="clear-selection" onclick={clearSelection}>Clear Selection</button>
			{:else}
				<span class="selection-hint">
					Click to select • {isMac ? 'Cmd' : 'Ctrl'}+Click to multi-select • Shift+Click for range
				</span>
			{/if}
		</div>
		<div class="debug-info">
			Total table width: {totalTableWidth()}px
		</div>
	</div>

	<!-- Invisible anchor used as popper reference; kept in the DOM so the action is initialized during component setup -->
	<div bind:this={virtualAnchor} use:referenceAction class="virtual-anchor" aria-hidden="true" style="display:none; position:absolute; left:-9999px; top:-9999px; width:0; height:0;"></div>

	<div class="virtual-table-container" bind:this={scrollContainer} onscroll={handleScroll}>
	<!-- Single table with CSS sticky columns -->
	<table class="sticky-table" bind:this={tableEl}>
		<colgroup>
			{#each columns as col}
				{#if col.visible !== false}
					<col style="width: {col.width}px;" />
				{/if}
			{/each}
		</colgroup>
		
		<!-- Sticky header -->
		<thead class="table-header">
			<tr style="height: {itemHeight}px;">			<!-- Match the item height -->
				{#each columns as col, i}
					{#if col.visible !== false}
						<th 
							class="resizable-header" 
							class:sticky-left={col.sticky === 'left'}
							class:sticky-right={col.sticky === 'right'}
							style={getStickyStyle(col, i, true)}
							oncontextmenu={(e) => openContextMenu(i, e)}
						>
							<button onclick={() => sortBy(col.key)}>{col.label} {getSortIndicator(col.key)}</button>
							<div class="col-resizer" role="separator" aria-orientation="vertical" tabindex="-1" onpointerdown={(e) => startResize(i, e)}></div>
						</th>
					{/if}
				{/each}
			</tr>
		</thead>
		
		<!-- Scrollable body content with virtualization -->
		<tbody class="table-body">
			{#if offsetY() > 0}
				<tr class="spacer-row" style="height: {offsetY()}px;">
					{#each columns as col}
						{#if col.visible !== false}
							<td></td>
						{/if}
					{/each}
				</tr>
			{/if}
			{#each visibleItems() as item (g(item, 'id'))}
				<tr 
					style="height: {itemHeight}px;" 
					class:selected={isRowSelected(item)}
					onclick={(e) => handleRowClick(e, item)}
					aria-selected={isRowSelected(item)}
					tabindex="0"
				>
					{#each columns as col, i}
						{#if col.visible !== false}
							<td 
								class:sticky-left={col.sticky === 'left'}
								class:sticky-right={col.sticky === 'right'}
								style={getStickyStyle(col, i)}
							>
								{g(item, col.key)}
							</td>
						{/if}
					{/each}
				</tr>
			{/each}
		</tbody>
	</table>
</div>
</div>

{#if contextOpen}
	<div class="context-portal" use:contentAction onmouseleave={closeContextMenu} aria-hidden={!contextOpen}>
		<div class="context-menu" role="menu">
			<button role="menuitem" onclick={() => toggleColumnVisibility(contextColumnIndex)}>
				{columns[contextColumnIndex] && columns[contextColumnIndex].visible !== false ? 'Hide' : 'Show'} {columns[contextColumnIndex] && columns[contextColumnIndex].label}
			</button>
			<hr />
			<!-- Column Reordering -->
			{#if canMoveLeft(contextColumnIndex)}
				<button role="menuitem" onclick={() => moveColumnLeft(contextColumnIndex)}>
					Move Left
				</button>
			{/if}
			{#if canMoveRight(contextColumnIndex)}
				<button role="menuitem" onclick={() => moveColumnRight(contextColumnIndex)}>
					Move Right
				</button>
			{/if}
			{#if canMoveLeft(contextColumnIndex) || canMoveRight(contextColumnIndex)}
				<hr />
			{/if}
			<!-- Sticky Column Controls -->
			<button role="menuitem" onclick={() => setColumnSticky(contextColumnIndex, 'left')}>
				Pin to Left
			</button>
			<button role="menuitem" onclick={() => setColumnSticky(contextColumnIndex, 'right')}>
				Pin to Right
			</button>
			{#if columns[contextColumnIndex] && columns[contextColumnIndex].sticky}
				<button role="menuitem" onclick={() => setColumnSticky(contextColumnIndex, null)}>
					Unpin Column
				</button>
			{/if}
			<hr />
			<button role="menuitem" onclick={closeContextMenu}>Close</button>
		</div>
	</div>
{/if}
