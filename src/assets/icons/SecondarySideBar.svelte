<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "#000000", size = "24" } = $props();
</script>

<svg xmlns="http://www.w3.org/2000/svg" width={size} height={size} viewBox="0 0 256 256"><g style="transform: rotate(180deg); transform-origin: 128px 128px; transform-box: fill-box;"><path fill={fill} d="M216 40H40a16 16 0 0 0-16 16v144a16 16 0 0 0 16 16h176a16 16 0 0 0 16-16V56a16 16 0 0 0-16-16m0 160H88V56h128z"/></g></svg>