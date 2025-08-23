<script>
//    import { open } from '@tauri-apps/api/dialog';
   import PhFolders from '../assets/icons/PhFolders.svelte';
   import { invoke } from '@tauri-apps/api/tauri';
    // Open a selection dialog for image files
    
    async function openFolderDialog(){
        // const selected = await open({
        // multiple: true,
        // filters: [{
        //     name: 'Image',
        //     extensions: ['png', 'jpeg']
        // }]
        // });
        // if (Array.isArray(selected)) {
        // // user selected multiple files
        // } else if (selected === null) {
        // // user cancelled the selection
        // } else {
        // // user selected a single file
        // }
        invoke('my_custom_command');
    }


    let fill = "rgba(255,255,255,0.75)";

</script>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<!-- svelte-ignore a11y-click-events-have-key-events -->
<div class="container" on:click={openFolderDialog}>
    <PhFolders/>
    <span class="text-container">Import Files</span>
</div>


<style>
.container{
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    cursor: pointer;
}
.text-container{
    color: var(--opacity-6);
    font-size: var(--size-4-5);
}
</style>