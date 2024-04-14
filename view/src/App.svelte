<script>
import SearchBar from "./lib/SearchBar.svelte";
import DataTable from "./lib/DataTable.svelte";
import StatusBar from "./lib/StatusBar.svelte";
import InfoPane from "./lib/InfoPane.svelte";
import DesktopTitleBar from "./lib/DesktopTitleBar.svelte";
import AppResizable from "./lib/AppResizable.svelte";

</script>

<div class="container">

    <DesktopTitleBar />

    <!-- <SearchBar />
    <DataTable />
    <StatusBar />
    <InfoPane /> -->
    <div class="app">
        <AppResizable/>
    </div>
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
    justify-content: flex-start;
    align-items: center;
    box-sizing: border-box;
}
.app{
    margin-top: var(--size-6);
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    /* border: 2px solid red; */
    width: 100%;
    box-sizing: border-box;
    height: calc(100vh - var(--size-6));
}

</style>
