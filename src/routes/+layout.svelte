<script>
    import "../app.css";
    import TitleBar from "$lib/TitleBar.svelte";
    let { children } = $props();
</script>

<div class="app-root">
    <TitleBar />
    <div class="main-content">
        {@render children()}
    </div>
</div>

<style>
.app-root {
    height: 100%;
    margin: 0;
    padding: 0;
    display: flex;
    flex-direction: column;
    height: 100vh;
    box-sizing: border-box;
    width: 100%;    
}
.main-content {
    flex: 1;
    min-height: 0;
    overflow: auto;
    box-sizing: border-box;
    margin-top: var(--titlebar-height);
}
</style>
