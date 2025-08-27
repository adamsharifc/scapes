<script>
    import "../app.css";
    import TitleBar from "$lib/TitleBar.svelte";
    let { children } = $props();
</script>

<TitleBar />
{@render children()}
