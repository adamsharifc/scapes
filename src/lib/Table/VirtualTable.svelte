<script>
  /**
   * Virtual table component that renders only visible rows for performance
   * @param {any[]} items - Array of data items to display
   * @param {number} itemHeight - Height of each row in pixels
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

  // Calculate total table width
  const totalTableWidth = $derived(() => {
    return columns
      .filter(col => col.visible !== false)
      .reduce((sum, col) => sum + col.width, 0);
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
      let offset = 0;
      const visibleColumns = columns.filter(c => c.visible !== false);
      
      if (col.sticky === 'left') {
        // Calculate left offset for this column
        for (let i = 0; i < visibleColumns.length; i++) {
          if (visibleColumns[i] === col) break;
          if (visibleColumns[i].sticky === 'left') {
            offset += visibleColumns[i].width;
          }
        }
        styles.push(`left: ${offset}px`);
      } else if (col.sticky === 'right') {
        // Calculate right offset for this column  
        for (let i = visibleColumns.length - 1; i >= 0; i--) {
          if (visibleColumns[i] === col) break;
          if (visibleColumns[i].sticky === 'right') {
            offset += visibleColumns[i].width;
          }
        }
        styles.push(`right: ${offset}px`);
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
</script>

<!-- Control panel: re-enable hidden columns -->
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
  <div class="debug-info">
    Total table width: {totalTableWidth()}px
  </div>
</div>

<!-- Invisible anchor used as popper reference; kept in the DOM so the action is initialized during component setup -->
<div bind:this={virtualAnchor} use:referenceAction class="virtual-anchor" aria-hidden="true" style="display:none; position:absolute; left:-9999px; top:-9999px; width:0; height:0;"></div>

<div class="virtual-table-container" bind:this={scrollContainer} onscroll={handleScroll}>
  <!-- Single table with CSS sticky columns -->
  <table class="sticky-table" style="width: {totalTableWidth()}px;">
    <colgroup>
      {#each columns as col}
        {#if col.visible !== false}
          <col style="width: {col.width}px;" />
        {/if}
      {/each}
    </colgroup>
    
    <!-- Sticky header -->
    <thead class="table-header">
      <tr>
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
        <tr style="height: {itemHeight}px;">
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

{#if contextOpen}
  <div class="context-portal" use:contentAction onmouseleave={closeContextMenu} aria-hidden={!contextOpen}>
    <div class="context-menu" role="menu">
      <button role="menuitem" onclick={() => toggleColumnVisibility(contextColumnIndex)}>
        {columns[contextColumnIndex] && columns[contextColumnIndex].visible !== false ? 'Hide' : 'Show'} {columns[contextColumnIndex] && columns[contextColumnIndex].label}
      </button>
      <hr />
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

<style>
    .virtual-table-container {
    width: 100%;
    height: 100%;
    overflow: auto; /* Enable both horizontal and vertical scrolling */
    border: 1px solid #ddd;
    position: relative;
    box-sizing: border-box;
  }

  .sticky-table {
    border-collapse: collapse;
    table-layout: fixed;
    width: auto; /* Allow table to size based on column widths */
  }

  .table-header {
    /* Remove sticky positioning from thead - individual th elements will handle it */
    background: white;
    border-bottom: 2px solid #ddd;
  }

  .table-header th {
    /* All header cells are sticky vertically, some also horizontally */
    position: sticky;
    background: white;
    z-index: 10;
  }

  .table-body {
    /* Remove any positioning that could cause gaps */
  }

  .sticky-left {
    position: sticky;
    z-index: 5;
    background: white;
    box-shadow: 2px 0 4px rgba(0,0,0,0.1);
  }

  .sticky-right {
    position: sticky;
    z-index: 5;
    background: white;
    box-shadow: -2px 0 4px rgba(0,0,0,0.1);
  }

  /* Header sticky columns should be above body sticky columns */
  .table-header .sticky-left,
  .table-header .sticky-right {
    z-index: 15;
  }

  .spacer-row {
    border: none;
    background: transparent;
  }

  .spacer-row td {
    padding: 0;
    border: none;
    height: inherit;
    border-bottom: none;
    border-top: none;
  }

  .sticky-table th,
  .sticky-table td {
    border: 1px solid #ddd;
    padding: 8px;
    text-align: left;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    box-sizing: border-box;
  }

  .sticky-table th {
    background-color: #f2f2f2;
    font-weight: bold;
  }

  .sticky-table th button {
    background: none;
    border: none;
    font: inherit;
    cursor: pointer;
    width: 100%;
    text-align: left;
    padding: 0;
    margin: 0;
    font-weight: bold;
  }

  .sticky-table th button:hover {
    background-color: rgba(0, 0, 0, 0.1);
  }

  .sticky-table tbody tr:hover {
    background-color: #f8f9fa;
  }

  /* Resizer handle styles */
  .resizable-header {
    position: relative;
    padding-right: 8px;
  }

  .col-resizer {
    position: absolute;
    top: 0;
    right: -6px;
    width: 12px;
    height: 100%;
    cursor: col-resize;
    touch-action: none;
    background: transparent;
  }

  .col-resizer:active {
    background: rgba(0,0,0,0.05);
  }

  .context-menu {
    background: white;
    box-shadow: 0 6px 18px rgba(0,0,0,0.12);
    border: 1px solid #ddd;
    padding: 0.25rem;
    display: flex;
    flex-direction: column;
    gap: 0.25rem;
  }
  .context-menu button {
    background: none;
    border: none;
    padding: 0.5rem 0.75rem;
    text-align: left;
    cursor: pointer;
  }
  .context-menu button:hover {
    background: #f2f2f2;
  }

  .context-menu hr {
    margin: 0.25rem 0;
    border: none;
    border-top: 1px solid #eee;
  }

  /* Portal wrapper should sit above the sticky header */
  .context-portal {
    position: absolute;
    z-index: 9999;
    pointer-events: auto;
  }

  /* Control panel above the table for toggling columns */
  .table-controls {
    padding: 0.5rem;
    border: 1px solid #eee;
    background: #fafafa;
    display: flex;
    align-items: center;
    gap: 1rem;
    flex-wrap: wrap;
  }
  .columns-toggle {
    display: flex;
    gap: 0.5rem;
    align-items: center;
  }
  .col-toggle {
    display: inline-flex;
    gap: 0.25rem;
    align-items: center;
    font-size: 0.9rem;
  }
  .show-all {
    margin-left: 0.5rem;
    padding: 0.25rem 0.5rem;
    border: 1px solid #ddd;
    background: white;
    cursor: pointer;
  }

  /* Invisible anchor used for popper reference */
  .virtual-anchor {
    position: absolute;
    width: 0;
    height: 0;
    pointer-events: none;
  }
</style>
