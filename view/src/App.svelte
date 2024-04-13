<script>
import SearchBar from "./lib/SearchBar.svelte";
import DataTable from "./lib/DataTable.svelte";
import StatusBar from "./lib/StatusBar.svelte";
import InfoPane from "./lib/InfoPane.svelte";
import { appWindow } from '@tauri-apps/api/window';
import { onMount } from 'svelte';

onMount(()=> {
    document.getElementById('titlebar-minimize').addEventListener('click', () => appWindow.minimize())
    document.getElementById('titlebar-maximize').addEventListener('click', () => appWindow.toggleMaximize())
    document.getElementById('titlebar-close').addEventListener('click', () => appWindow.close())
})

</script>

<div class="container">
    <div data-tauri-drag-region class="titlebar">
        <div class="titlebar-button" id="titlebar-minimize">
          <img
            src="https://api.iconify.design/mdi:window-minimize.svg"
            alt="minimize"
          />
        </div>
        <div class="titlebar-button" id="titlebar-maximize">
          <img
            src="https://api.iconify.design/mdi:window-maximize.svg"
            alt="maximize"
          />
        </div>
        <div class="titlebar-button" id="titlebar-close">
          <img src="https://api.iconify.design/mdi:close.svg" alt="close" />
        </div>
    </div>
    <div style="padding-bottom: var(--size-2);"></div>

    <SearchBar />
    <DataTable />
    <StatusBar />
    <InfoPane />
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
