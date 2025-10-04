<script>
    import Minus from '../assets/icons/Minus.svelte';
    import Rectangle from '../assets/icons/Rectangle.svelte';
    import X from '../assets/icons/X.svelte';

    import CloseHoverMacos from '../assets/icons/CloseHoverMacos.svelte';
    import CloseNormalMacos from '../assets/icons/CloseNormalMacos.svelte';
    import MaximizeHoverMacos from '../assets/icons/MaximizeHoverMacos.svelte';
    import MaximizeNormalMacos from '../assets/icons/MaximizeNormalMacos.svelte';
    import MinimizeHoverMacos from '../assets/icons/MinimizeHoverMacos.svelte';
    import MinimizeNormalMacos from '../assets/icons/MinimizeNormalMacos.svelte';

    // Svelte 5 pattern for props
    /** @type {{ platform?: string }} */
    let { platform = 'unknown' } = $props();

    // macOS icon hover state for the whole container
    let macosHovered = $state(false);
</script>

{#if platform === "windows"}

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
        <Rectangle fill="var(--fg)" size={"var(--size-4-5)"} />
    </button>

    <button 
        class="titlebar-button" 
        id="titlebar-close" 
        title="Close" 
        aria-label="Close"
    >
        <X fill="var(--fg)" size={"var(--size-4-5)"} />
    </button>

{:else if platform === "macos"}
    <div class="macos-controls"
        role="group"
        onmouseenter={() => {macosHovered = true}}
        onmouseleave={() => {macosHovered = false}}
    >
        <button
            class="macos-button"
            title="Close"
            aria-label="Close"
        >
            {#if macosHovered}
                <CloseHoverMacos size={"var(--size-3-5)"} />
            {:else}
                <CloseNormalMacos size={"var(--size-3-5)"} />
            {/if}
        </button>

        <button
            class="macos-button"
            title="Minimize"
            aria-label="Minimize"
        >
            {#if macosHovered}
                <MinimizeHoverMacos size={"var(--size-3-5)"} />
            {:else}
                <MinimizeNormalMacos size={"var(--size-3-5)"} />
            {/if}
        </button>

        <button
            class="macos-button"
            title="Maximize"
            aria-label="Maximize"
        >
            {#if macosHovered}
                <MaximizeHoverMacos size={"var(--size-3-5)"} />
            {:else}
                <MaximizeNormalMacos size={"var(--size-3-5)"} />
            {/if}
        </button>
    </div>

{:else if platform === "linux"}

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
.macos-button{
    display: inline-flex;
    justify-content: center;
    align-items: center;
    height: 100%;
    background: none;
    border: none;
    cursor: pointer;
}
.macos-controls{
    display: inline-flex;
}
</style>