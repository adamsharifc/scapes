<script>

	import Scribble from '../assets/icons/Scribble.svelte';
	import SearchBar from './SearchBar.svelte';
	import LayoutControls from './LayoutControls.svelte';
	import WindowControls from './WindowControls.svelte';

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

		<WindowControls platform={buildPlatform} />
	</div>
</div>
{:else if buildPlatform === "macos"}
<div class="titlebar" data-build-platform={buildPlatform}>
	<div style="display: inline-flex;">
		<WindowControls platform={buildPlatform} />
		
		<div class="titlebar-title">
			<Scribble fill="var(--fg)" size={"var(--size-6)"} />
			&nbsp; scapes
		</div>
	</div>

	<div class="search-bar">
		<SearchBar />
	</div>

	<div class="titlebar-buttons">
		<LayoutControls />

		<div style="padding-left: var(--size-2); display: inline-flex; "></div>


	</div>

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