<script>
    import { onMount, onDestroy } from "svelte";
    import Protools from "../assets/icons/Protools.svelte";
    import Reaper from "../assets/icons/Reaper.svelte";
    import { invoke } from '@tauri-apps/api/tauri';

    let intervalID;

    async function scan_all_daws(){
        let daws = await invoke('scan_all_daws');
        // extract the daws from the response
        console.log(daws);
    }

    onMount(() => {
        scan_all_daws();
        intervalID = setInterval(() => {
            scan_all_daws();
        }, 5000);
    });

    onDestroy(() => {
        clearInterval(intervalID);
    });
</script>

<div class="container">
    <button class="pill">
        <Protools fill="#ffffff" />
        <span>Pro Tools</span>
    </button>
    <button class="pill">
        <Reaper fillPrimary="#ffffff" fillSecondary="#061522"/> 
        <span>Reaper</span>
    </button>
</div>

<style>
.container{
    display: flex;
    justify-content: center;
    align-items: center;
    gap: var(--size-2); 
}
.pill{
    display:flex;
    justify-content:center;
    align-items:center;
    padding: var(--size-1) var(--size-2);
    border-radius: var(--size-1);
}
.pill:nth-of-type(1){
    background-color: var(--opacity-2);
}    
.pill:nth-of-type(2){
    background-color: var(--opacity-2);
}
.pill span{
    color: white;
    font-size: var(--size-4);
    margin-left: var(--size-1);
    font-weight: 500;
}
</style>