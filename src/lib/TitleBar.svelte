<script>
	import SearchBar from './SearchBar.svelte';
	import LayoutControls from './LayoutControls.svelte';
	import WindowControls from './WindowControls.svelte';
	import Logo from './Logo.svelte';
	import ThemeToggle from './ThemeToggle.svelte';

	// Read build platform from Vite-injected env (from .env at project root)
	// Values: "windows", "macos", "linux"
	const buildPlatform = import.meta.env.VITE_BUILD_PLATFORM ?? 'unknown';

</script>

<div class="titlebar" data-build-platform={buildPlatform}>
{#if buildPlatform === "windows"}

	<div class="left-box">
		<Logo />
	</div>
	
	<div class="center-box">
		<SearchBar />
	</div>

	<div class="right-box">
		<!-- <ThemeToggle /> -->
		<LayoutControls />
		<WindowControls platform={buildPlatform} />
	</div>

{:else if buildPlatform === "macos"}

	<div class="left-box">
		<div class="macos-spacer"></div>
		<WindowControls platform={buildPlatform} />
		<Logo />
	</div>

	<div class="center-box">
		<SearchBar />
	</div>

	<div class="right-box">
		<LayoutControls />
		<div class="macos-spacer"></div>
	</div>

{/if}
</div>

<style>
.titlebar {
	display: grid;
	grid-template-columns: auto 1fr auto;
	width: 100%;
	justify-content: space-between;
	align-items: center;
	position: fixed;
	top: 0; left: 0; right: 0;
	box-sizing: border-box;
	z-index: 2;
	height: clamp(var(--titlebar-height), var(--titlebar-height), var(--titlebar-height));
}
.left-box,
.center-box,
.right-box{
	display: flex;
	height: 100%;
	align-items: center;
	box-sizing: border-box;
	flex-wrap: nowrap;
}
.left-box{
	justify-content: flex-start;
}
.center-box{
	justify-content: center;
}
.right-box{
	justify-content: flex-end;
}
.macos-spacer{
	padding-left: var(--size-2);
	display: inline-flex;
}
</style>