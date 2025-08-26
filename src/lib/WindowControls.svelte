<script>
    import Minus from '../assets/icons/Minus.svelte';
    import Square from '../assets/icons/Square.svelte';
    import X from '../assets/icons/X.svelte';

    import CloseHoverMacos from '../assets/icons/CloseHoverMacos.svelte';
    import CloseNormalMacos from '../assets/icons/CloseNormalMacos.svelte';
    import MaximizeHoverMacos from '../assets/icons/MaximizeHoverMacos.svelte';
    import MaximizeNormalMacos from '../assets/icons/MaximizeNormalMacos.svelte';
    import MinimizeHoverMacos from '../assets/icons/MinimizeHoverMacos.svelte';
    import MinimizeNormalMacos from '../assets/icons/MinimizeNormalMacos.svelte';

    const buildPlatform = import.meta.env.VITE_BUILD_PLATFORM ?? 'unknown';
</script>

{#if buildPlatform === "windows"}

    <button 
        class="titlebar-button" 
        id="titlebar-minimize" 
        title="Minimize" 
        aria-label="Minimize"
    >
        <Minus fill="var(--fg)" size={"var(--size-4-5)"} />
    </button>
   
    <button 
        class="titlebar-button" 
        id="titlebar-maximize" 
        title="Maximize" 
        aria-label="Maximize"
    >
        <Square fill="var(--fg)" size={"var(--size-4-5)"} />
    </button>

    <button 
        class="titlebar-button" 
        id="titlebar-close" 
        title="Close" 
        aria-label="Close"
    >
        <X fill="var(--fg)" size={"var(--size-4-5)"} />
    </button>

{:else if buildPlatform === "macos"}



{:else if buildPlatform === "linux"}

{:else}

{/if}

<style>
.titlebar-button {
    display: inline-flex;
    justify-content: center;
    align-items: center;
    padding: var(--size-0-5) var(--size-3);
    border-radius: var(--size-1);
    height: 100%;
    background: none;
    border: none;
    cursor: pointer;
}
.titlebar-button:hover {
    background: #5bbec3;
}
#titlebar-minimize:hover {
    background: var(--opacity-3);
}
#titlebar-maximize:hover {
    background: var(--opacity-3);
}
#titlebar-close:hover {
    background: var(--std-red);
}
</style>