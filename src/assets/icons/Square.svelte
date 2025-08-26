<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "#000000", size = "24" } = $props();
</script>

<svg xmlns="http://www.w3.org/2000/svg" width={size} height={size} fill={fill} viewBox="0 0 256 256"><path d="M208,32H48A16,16,0,0,0,32,48V208a16,16,0,0,0,16,16H208a16,16,0,0,0,16-16V48A16,16,0,0,0,208,32Zm0,176H48V48H208V208Z"></path></svg>