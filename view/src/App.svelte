<script>
    import DesktopTitleBar from "./lib/DesktopTitleBar.svelte";
    import AppResizable from "./lib/AppResizable.svelte";

    import { onMount, onDestroy } from "svelte";
    import { createEventDispatcher } from "svelte";
    
    let keydownHandler;
    const dispatch = createEventDispatcher();


    onMount(() => {
        keydownHandler = function(event) {
            // Check if the active element is not an input element
            if (!(document.activeElement instanceof HTMLInputElement)) {
                console.log(`Key pressed: ${event.key}`);

                if (event.key === "f" || event.key === "F") {
                    console.log("F key pressed");
                    // call a function to focus on the search bar
                    dispatch("focus-search-bar");
                }
            }
        };

        window.addEventListener('keydown', keydownHandler);
    });

    onDestroy(() => {
        window.removeEventListener('keydown', keydownHandler);
    });

</script>

<div class="container">

    <DesktopTitleBar />
    <div class="app">
        <AppResizable/>
    </div>
</div>

<style>
.container{
    width: 100vw;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    gap: 2rem;
    margin: 0;
    background-color: var(--shade-0);
    color: white;
    justify-content: flex-start;
    align-items: center;
    box-sizing: border-box;
}
.app{
    margin-top: var(--size-6-2-5);
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    /* border: 2px solid red; */
    width: 100%;
    box-sizing: border-box;
    height: calc(100vh - var(--size-6-2-5));
}

</style>
