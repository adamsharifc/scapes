<script>
  // Import the factory function from our logic file
  import { createTableLogic } from './functions.svelte.js';
  import VirtualTable from './VirtualTable.svelte';

  // Create an instance of our table logic.
  // This gives us access to the reactive state and functions.
  const { sortedUsers, pagedUsers, sortKey, sortDirection, sortBy, addUser, page, pageSize, pageOptions, totalCount, totalPages, setPageSize, setPage, nextPage, prevPage } = createTableLogic();

  // Helper function to get the sort indicator for a column header
  /**
   * @param {string} key
   */
  function getSortIndicator(key) {
    if (sortKey() !== key) return '';
    return sortDirection() === 'asc' ? '▲' : '▼';
  }

  /**
   * Safe getter to avoid TS template complaints
   * @param {any} obj
   * @param {string} key
   */
  function g(obj, key) { return obj ? obj[key] : undefined; }

  // Keyboard navigation for pagination and sorting
  /** @param {KeyboardEvent} e */
  function handleKeydown(e) {
    // left/right for prev/next, pageup/pagedown for prev/next page, home/end for first/last
    if (e.key === 'ArrowRight') { nextPage(); }
    else if (e.key === 'ArrowLeft') { prevPage(); }
    else if (e.key === 'PageDown') { nextPage(); }
    else if (e.key === 'PageUp') { prevPage(); }
    else if (e.key === 'Home') { setPage(1); }
    else if (e.key === 'End') { setPage(totalPages()); }
  }

  import { onMount, onDestroy } from 'svelte';
  onMount(() => window.addEventListener('keydown', handleKeydown));
  onDestroy(() => window.removeEventListener('keydown', handleKeydown));
</script>

<main>
  <h1>User Table</h1>

  <div role="region" aria-label="Table controls" style="display:flex;gap:1rem;align-items:center;justify-content:space-between;">
    <div>
      <button onclick={() => addUser()}>Add User</button>
    </div>
    <div>
      <label>
        Page size:
        <select onchange={(e) => setPageSize(e.currentTarget.value)} aria-label="Page size">
          {#each pageOptions as opt}
            <option value={opt} selected={String(opt) === String(pageSize())}>{opt}</option>
          {/each}
        </select>
      </label>
      <label style="margin-left:1rem">
        Page:
        <input type="number" min="1" max={totalPages()} value={page()} onchange={(e) => setPage(e.currentTarget.value)} style="width:4rem" aria-label="Page number" />
      </label>
      <button onclick={() => prevPage()} style="margin-left:0.5rem">Prev</button>
      <button onclick={() => nextPage()} style="margin-left:0.25rem">Next</button>
      <span style="margin-left:1rem">{page()} / {totalPages()} — {totalCount()} rows</span>
    </div>
  </div>

  <!-- Use VirtualTable component for efficient rendering -->
  <div class="table-wrapper">
    <VirtualTable 
      items={pagedUsers()} 
      itemHeight={40} 
      {sortBy} 
      {getSortIndicator} 
      {g} 
    />
  </div>
</main>

<style>
  main {
    font-family: sans-serif;
    text-align: center;
    display: flex;
    flex-direction: column;
    height: 100%;
    width: 100%;
    box-sizing: border-box;
  }
  
  .table-wrapper {
    flex: 1;
    min-height: 400px;
    margin-top: 1rem;
    width: 100%;
    box-sizing: border-box;
  }
  
  button {
    margin-bottom: 1rem;
    padding: 0.5rem 1rem;
    cursor: pointer;
    background-color: #007bff;
    color: white;
    border: none;
    border-radius: 4px;
  }
  button:hover {
    background-color: #0056b3;
  }
</style>