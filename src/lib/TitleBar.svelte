<script>

	import Scribble from '../assets/icons/Scribble.svelte';
	import MinusBold from '../assets/icons/MinusBold.svelte';
	import SquareBold from '../assets/icons/SquareBold.svelte';
	import XBold from '../assets/icons/XBold.svelte';
	import PrimarySideBar from '../assets/icons/PrimarySideBar.svelte';
	import Panel from '../assets/icons/Panel.svelte';
	import SecondarySideBar from '../assets/icons/SecondarySideBar.svelte';
	import SearchBar from './SearchBar.svelte';

	// Read build platform from Vite-injected env (from .env at project root)
	// Values: "windows", "macos", "linux"
	const buildPlatform = import.meta.env.VITE_BUILD_PLATFORM ?? 'unknown';

</script>

<div class="titlebar" data-build-platform={buildPlatform}>

	<div class="titlebar-title">
		<Scribble fill="var(--fg)" />
		&nbsp; scapes
	</div>

	<div class="search-bar">
		<SearchBar />
	</div>

	<div class="titlebar-buttons">
		<div class="titlebar-panel-button">
			<PrimarySideBar fill="var(--fg)" />
		</div>

		<div class="titlebar-panel-button">
			<SecondarySideBar fill="var(--fg)" />
		</div>

		<div class="titlebar-panel-button">
			<Panel fill="var(--fg)" />
		</div>

		<div style="padding-left: var(--size-2); display: inline-flex; "></div>

		<div class="titlebar-button" id="titlebar-minimize">
			<MinusBold fill="var(--fg)" />
		</div>
		<div class="titlebar-button" id="titlebar-maximize">
			<SquareBold fill="var(--fg)" />
		</div>
		<div class="titlebar-button" id="titlebar-close">
			<XBold fill="var(--fg)" />
		</div>
	</div>
</div>


<style>

.titlebar {
	background: transparent;
	user-select: none;
	display: grid;
	grid-template-columns: 1fr 1fr 1fr;
	align-items: center;
	flex-wrap: nowrap;
	position: fixed;
	top: 0;
	left: 0;
	right: 0;
	max-height: var(--size-6);
	box-sizing: border-box;
	width: 100%;
	height: 100%;
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