<script>
    import WaveForm from "./WaveForm.svelte";
    import ToolBar from "./ToolBar.svelte";
</script>


<div class="container">
    <ToolBar/>
    <!-- <span>App Panel</span> -->
    <WaveForm/>
</div>


<style>
.container{
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
}
</style>