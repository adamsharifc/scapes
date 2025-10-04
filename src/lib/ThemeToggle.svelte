<script>
    $effect(() => {
        const theme = localStorage.getItem("theme") || "light";
        document.body.setAttribute("data-theme", theme);
    });

    function toggleTheme() {
        const currentTheme = document.body.getAttribute("data-theme");
        const newTheme = currentTheme === "light" ? "dark" : "light";
        document.body.setAttribute("data-theme", newTheme);
        localStorage.setItem("theme", newTheme);
    }
</script>

<button onclick={toggleTheme}>Toggle Theme</button>