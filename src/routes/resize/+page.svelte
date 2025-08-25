<script lang="ts">
    import AppResizable from "$lib/AppResizable.svelte";

    let appResizable: AppResizable;

</script>


<div class="button-bar">
    <button on:click={() => appResizable?.collapsePrimarySidebar()}>Collapse Primary Sidebar</button>
    <button on:click={() => appResizable?.expandPrimarySidebar()}>Expand Primary Sidebar</button>
    <button on:click={() => appResizable?.collapseSecondarySidebar()}>Collapse Secondary Sidebar</button>
    <button on:click={() => appResizable?.expandSecondarySidebar()}>Expand Secondary Sidebar</button>
    <button on:click={() => appResizable?.collapsePanel()}>Collapse Panel</button>
    <button on:click={() => appResizable?.expandPanel()}>Expand Panel</button>
</div>

<div class="resize-root">
	<AppResizable bind:this={appResizable}/>
</div>

<style>
.button-bar {
	display: flex;
	gap: 0.5rem;
	padding: 1rem;
	background: #f0f0f0;
	border-bottom: 1px solid #ddd;
}
.resize-root {
	width: 100vw;
	height: calc(100vh - 56px);
	min-width: 0;
	min-height: 0;
	overflow: hidden;
}
</style>

