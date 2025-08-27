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
}
.main-content {
    flex: 1 1 auto;
    min-height: 0;
    overflow: auto;
    border: 1px solid red;
    box-sizing: border-box;
}
</style>
