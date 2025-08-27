<script>
    import AppResizable from "$lib/AppResizable.svelte";
    let appResizable;
</script>

<div class="container">
    <!-- <AppResizable bind:this={appResizable}/> -->
</div>

<style>
.container{
    display: flex;
    flex-direction: column;
    justify-content: flex-start;
    align-items: flex-start;
    box-sizing: border-box;
    width: 100vw;
	height: calc(100vh - 56px);
	min-width: 0;
	min-height: 0;
	overflow: hidden;
}
</style>
