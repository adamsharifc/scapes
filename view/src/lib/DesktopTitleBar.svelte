<script>
import { appWindow } from '@tauri-apps/api/window';
import { onMount } from 'svelte';

import PhMinusBold from '../assets/icons/PhMinusBold.svelte';
import PhSquareBold from '../assets/icons/PhSquareBold.svelte';
import PhXBold from '../assets/icons/PhXBold.svelte';
import PhPrimarySideBar from '../assets/icons/PhPrimarySideBar.svelte';
import PhPanel from '../assets/icons/PhPanel.svelte';
import PhSecondarySideBar from '../assets/icons/PhSecondarySideBar.svelte';
import SearchBar from './SearchBar.svelte';

onMount(()=> {
    document.getElementById('titlebar-minimize').addEventListener('click', () => appWindow.minimize())
    document.getElementById('titlebar-maximize').addEventListener('click', () => appWindow.toggleMaximize())
    document.getElementById('titlebar-close').addEventListener('click', () => appWindow.close())
})

</script>

<div data-tauri-drag-region class="titlebar">

    <div class="titlebar-title">
        scapes
    </div>

    <div class="search-bar">
        <SearchBar />
    </div>

    <div class="titlebar-buttons">
        <div class="titlebar-panel-button">
            <PhPrimarySideBar fill="white" />
        </div>

        <div class="titlebar-panel-button">
            <PhSecondarySideBar fill="white" />
        </div>

        <div class="titlebar-panel-button">
            <PhPanel fill="white" />
        </div>

        <div style="padding-left: var(--size-2); display: inline-flex; "></div>

        <div class="titlebar-button" id="titlebar-minimize">
            <PhMinusBold fill="white" />
        </div>
        <div class="titlebar-button" id="titlebar-maximize">
            <PhSquareBold fill="white" />
        </div>
        <div class="titlebar-button" id="titlebar-close">
            <PhXBold fill="white" />
        </div>
    </div>
</div>


<style>

.titlebar {
    background: transparent;
    user-select: none;
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    align-items: center;
    flex-wrap: nowrap;
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    max-height: var(--size-6);
    box-sizing: border-box;
}
.titlebar-button {
  display: inline-flex;
  justify-content: center;
  align-items: center;
  padding: var(--size-0-5) var(--size-3);
  border-radius: var(--size-1);
  height: 100%;
}
.titlebar-button:hover {
  background: #5bbec3;
}
#titlebar-minimize:hover{
    background: var(--opacity-3);
}
#titlebar-maximize:hover{
    background: var(--opacity-3);
}
#titlebar-close:hover{
    background: var(--std-red);
}
.titlebar-title{
    font-size: var(--size-4-5);
    background-clip: text;
    font-weight: 500;
    color: white;
    padding-left: var(--size-3);
    padding-top: var(--size-1);
    padding-bottom: var(--size-1);
    box-sizing: border-box;
    height: 100%;
    display: flex;
    justify-content: flex-start;
    align-items: center;
}
.titlebar-panel-button{
    display: inline-flex;
    justify-content: center;
    align-items: center;
    padding: var(--size-0-5) var(--size-2);
    border-radius: var(--size-1);
    height: 100%;
}
.titlebar-panel-button:hover {
    background: var(--opacity-3);
}
.search-bar{
    align-items:center;justify-content:center;
}
.titlebar-buttons{
    display: flex;
    justify-content: flex-end;
    align-items: center;
    height: 100%;
}

</style>