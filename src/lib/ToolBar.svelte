<script>
import Protools from "../assets/icons/Protools.svelte";
import Reaper from "../assets/icons/Reaper.svelte";
import SendTo from "./SendTo.svelte";

</script>

<div class="container">
    <SendTo/>
</div>


<style>
.container {
    display: flex;
    justify-content: flex-end;
    align-items: center;
    width: 100%;
    box-sizing: border-box;
    padding: var(--size-1) var(--size-2);
}
</style>