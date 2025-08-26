<script>
    import SidebarSimple from "../assets/icons/SidebarSimple.svelte";
    import SidebarSimpleFilled from "../assets/icons/SidebarSimpleFilled.svelte";
    import SquareHalf from "../assets/icons/SquareHalf.svelte";
    import SquareHalfFilled from "../assets/icons/SquareHalfFilled.svelte";
    import SquareHalfBottom from "../assets/icons/SquareHalfBottom.svelte";
    import SquareHalfBottomFilled from "../assets/icons/SquareHalfBottomFilled.svelte";

    let isSidebarOpen = $state(false);
    let isSecondarySidebarOpen = $state(false);
    let isPanelOpen = $state(false);

    function toggleSidebar() {
        isSidebarOpen = !isSidebarOpen;
    }

    function toggleSecondarySidebar() {
        isSecondarySidebarOpen = !isSecondarySidebarOpen;
    }

    function togglePanel() {
        isPanelOpen = !isPanelOpen;
    }
</script>

<div>

    <button
        class="sidebar-toggle-btn"
        aria-pressed={isSidebarOpen}
        onclick={toggleSidebar}
        title="Toggle sidebar"
    >
        {#if isSidebarOpen}
            <SidebarSimpleFilled fill="var(--fg)" size={"var(--size-4-5)"} />
        {:else}
            <SidebarSimple fill="var(--fg)" size={"var(--size-4-5)"} />
        {/if}

    </button>

    <button
        class="sidebar-toggle-btn"
        aria-pressed={isSecondarySidebarOpen}
        onclick={toggleSecondarySidebar}
        title="Toggle secondary sidebar"
    >

        {#if isSecondarySidebarOpen}
            <SquareHalfFilled fill="var(--fg)" size={"var(--size-4-5)"} />
        {:else}
            <SquareHalf fill="var(--fg)" size={"var(--size-4-5)"} />
        {/if}

    </button>

    <button
        class="sidebar-toggle-btn"
        aria-pressed={isPanelOpen}
        onclick={togglePanel}
        title="Toggle panel"
    >

        {#if isPanelOpen}
            <SquareHalfBottomFilled fill="var(--fg)" size={"var(--size-4-5)"} />
        {:else}
            <SquareHalfBottom fill="var(--fg)" size={"var(--size-4-5)"} />
        {/if}

    </button>
</div>

<style>
.sidebar-toggle-btn {
    background: none;
    border: none;
    padding: 0.25em;
    cursor: pointer;
    display: inline-flex;
    align-items: center;
    justify-content: center;
}
</style>