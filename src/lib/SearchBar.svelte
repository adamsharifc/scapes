<script>
	import MagnifyingGlassFill from "../assets/icons/MagnifyingGlassFill.svelte";
	import MagnifyingGlass from "../assets/icons/MagnifyingGlass.svelte";
	
	let isFocused = $state(false);

	function onFocus() { isFocused = true; }
	function onBlur() { isFocused = false;}

</script>

<div class="container" class:focused={isFocused}>
	<input type="search" placeholder="Search"   name="" id="" class="inputElement" tabindex="0" onfocus={onFocus} onblur={onBlur}>
	{#if isFocused}
		<MagnifyingGlassFill fill="var(--fg-70)" size="var(--size-4-5)" />
	{:else}
		<MagnifyingGlass fill="var(--fg-70)" size="var(--size-4-5)" />
	{/if}

</div>

<style>
.container{
	display: flex;
	justify-content: center; 
	align-items: center;
	background-color: var(--fg-3);
	width: 100%;
	height: calc(100% - var(--size-1));	/* var(--size-1) being double var(--size-0-5) */
	margin: var(--size-0-5) 0;
	padding: 0 var(--size-2-5);
	border-radius: var(--size-1-5);
	max-width: var(--size-14-5);
	box-sizing: border-box;
	transition: background-color 0.2s ease-out;
}
.container.focused{
	background-color: var(--fg-7);
}
.inputElement{
	display: flex;
	flex-direction: row;
	justify-content: flex-start;
	max-width: 100%;
	width: 100%;
	height: 100%;
	background-color: transparent;
	color: var(--fg-70);
	border: none;
	font-family: var(--font-sans);
	font-size: var(--size-4);
	font-weight: 450;
}
.inputElement:focus{
	outline: none;
	color: var(--fg-90);
}
.inputElement::-webkit-search-cancel-button{
	display: none;
}
.inputElement::placeholder{
	color: var(--opacity-7);
}

</style>