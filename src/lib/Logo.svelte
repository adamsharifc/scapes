<script>
    import Scribble from "../assets/icons/Scribble.svelte";
</script>

<div class="logo">
    <Scribble fill="var(--fg)" size={"var(--size-5)"} />
    <span class="logo-text">scapes</span>
</div>

<style>
.logo{
	display: flex;
	justify-content: flex-start;
	align-items: center;
	color: var(--fg);
	box-sizing: border-box;
	height: 100%;
	white-space: nowrap;
	overflow: hidden;
	text-overflow: ellipsis;
	min-width: 0;
    padding: 0 var(--size-3);
	gap: var(--size-2);
}
.logo-text{
	font-size: var(--size-4-5);
    font-family: var(--font-sans);
	font-weight: 700;
}
</style>