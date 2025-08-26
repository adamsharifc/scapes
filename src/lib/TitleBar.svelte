<script>

	import Scribble from '../assets/icons/Scribble.svelte';
	import Minus from '../assets/icons/Minus.svelte';
	import Square from '../assets/icons/Square.svelte';
	import X from '../assets/icons/X.svelte';
	import SearchBar from './SearchBar.svelte';
	import LayoutControls from './LayoutControls.svelte';

	// Read build platform from Vite-injected env (from .env at project root)
	// Values: "windows", "macos", "linux"
	const buildPlatform = import.meta.env.VITE_BUILD_PLATFORM ?? 'unknown';

</script>

{#if buildPlatform === "windows"}
<div class="titlebar" data-build-platform={buildPlatform}>

	<div class="titlebar-title">
		<Scribble fill="var(--fg)" size={"var(--size-6)"} />
		&nbsp; scapes
	</div>

	<div class="search-bar">
		<SearchBar />
	</div>

	<div class="titlebar-buttons">
		<LayoutControls />

		<div style="padding-left: var(--size-2); display: inline-flex; "></div>

		<div class="titlebar-button" id="titlebar-minimize">
			<Minus fill="var(--fg)" size={"var(--size-4-5)"} />
		</div>
		<div class="titlebar-button" id="titlebar-maximize">
			<Square fill="var(--fg)" size={"var(--size-4-5)"} />
		</div>
		<div class="titlebar-button" id="titlebar-close">
			<X fill="var(--fg)" size={"var(--size-4-5)"} />
		</div>
	</div>
</div>
{:else if buildPlatform === "macos"}
<div class="titlebar" data-build-platform={buildPlatform}>


</div>
{/if}


<style>

.titlebar {
    background: transparent;
    user-select: none;
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    align-items: center;
    flex-wrap: nowrap;
    top: 0;
    left: 0;
    right: 0;
    box-sizing: border-box;
    width: 100%;
    z-index: 1000;
    border: 1px solid red;
    padding: var(--size-1) 0;
}
.titlebar-button {
	display: inline-flex;
	justify-content: center;
	align-items: center;
	padding: var(--size-0-5) var(--size-3);
	border-radius: var(--size-1);
	height: 100%;
}
.titlebar-button:hover {
	background: #5bbec3;
}
#titlebar-minimize:hover{
	background: var(--opacity-3);
}
#titlebar-maximize:hover{
	background: var(--opacity-3);
}
#titlebar-close:hover{
	background: var(--std-red);
}
.titlebar-title{
	font-size: var(--size-4-5);
	background-clip: text;
	font-weight: 700;
	color: var(--fg);
	padding-left: var(--size-3);
	box-sizing: border-box;
	height: 100%;
	display: flex;
	justify-content: flex-start;
	align-items: center;
}
.titlebar-panel-button{
	display: inline-flex;
	justify-content: center;
	align-items: center;
	padding: var(--size-0-5) var(--size-2);
	border-radius: var(--size-1);
	height: 100%;
}
.titlebar-panel-button:hover {
	background: var(--opacity-3);
}
.search-bar{
	align-items:center;
	justify-content:center;
}
.titlebar-buttons{
	display: flex;
	justify-content: flex-end;
	align-items: center;
	height: 100%;
}

</style>