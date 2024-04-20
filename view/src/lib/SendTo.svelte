<script>
    import { onMount, onDestroy } from "svelte";
    import PhProtools from "../assets/icons/PhProtools.svelte";
    import PhReaper from "../assets/icons/PhReaper.svelte";
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
        <PhProtools fill="#ffffff" />
        <span>Pro Tools</span>
    </button>
    <button class="pill">
        <PhReaper fillPrimary="#ffffff" fillSecondary="#061522"/> 
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