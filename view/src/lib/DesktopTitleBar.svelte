<script>
import { appWindow } from '@tauri-apps/api/window';
import { onMount } from 'svelte';

import PhMinusBold from '../assets/icons/PhMinusBold.svelte'
import PhSquareBold from '../assets/icons/PhSquareBold.svelte'
import PhXBold from '../assets/icons/PhXBold.svelte'


onMount(()=> {
    document.getElementById('titlebar-minimize').addEventListener('click', () => appWindow.minimize())
    document.getElementById('titlebar-maximize').addEventListener('click', () => appWindow.toggleMaximize())
    document.getElementById('titlebar-close').addEventListener('click', () => appWindow.close())
})

</script>

<div data-tauri-drag-region class="titlebar">

    <div style="height: 100%;" class="titlebar-title">
        scapes
    </div>
    <div style="height: 100%;">

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
    display: inline-flex;
    justify-content: space-between;
    align-items: center;
    flex-wrap: nowrap;
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    height: var(--size-5);
}
.titlebar-button {
  display: inline-flex;
  justify-content: center;
  align-items: center;
  padding: var(--size-1) var(--size-3);
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
    padding: var(--size-2) var(--size-3);
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
}


</style>