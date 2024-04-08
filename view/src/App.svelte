<script>
import SearchBar from "./lib/SearchBar.svelte";
import DataTable from "./lib/DataTable.svelte";
import StatusBar from "./lib/StatusBar.svelte";
</script>

<div class="container">
    <SearchBar />
    <DataTable />
    <StatusBar />
</div>

<style>
.container{
    width: 100vw;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    gap: 2rem;
    margin: 0;
    background-color: var(--shade-0);
    color: white;
    justify-content: center;
    align-items: center;
}


</style>
