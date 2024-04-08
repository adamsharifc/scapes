<script>

  import PhMagnifyingGlassFill from "../assets/icons/PhMagnifyingGlassFill.svelte";
  import PhMagnifyingGlass from "../assets/icons/PhMagnifyingGlass.svelte";

  let inputElement;
  let isFocused = false;

  function onFocus() {
    isFocused = true;
  }

  function onBlur() {
    isFocused = false;
  }
</script>

<div class="container">
  <input type="search" placeholder="Search"   name="" id="" class="inputElement" bind:this={inputElement} tabindex="0" on:focus={onFocus} on:blur={onBlur}>
  {#if isFocused}
    <PhMagnifyingGlassFill fill="var(--white)" />
  {:else}
    <PhMagnifyingGlass fill="var(--white)" />
  {/if}

</div>

<style>
.container{
  display: flex;
  justify-content: center; 
  align-items: center;
  background-color: var(--opacity-2);
  padding-right: var(--size-1);
  padding-left: var(--size-1);
  border-radius: var(--size-1);
}


.inputElement{
    display: flex;
    flex-direction: row;
    justify-content: flex-start;
    min-width: var(--size-14);
    min-height: var(--size-5);
    background-color: transparent;
    color: var(--white);
    border: none;
    font-size: var(--size-3);
}
.inputElement:focus{
    outline: none;
}

.inputElement::-webkit-search-cancel-button{
    display: none;
}
.inputElement::placeholder{
    color: var(--opacity-7);
}

</style>