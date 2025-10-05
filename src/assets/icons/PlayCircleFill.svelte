<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "currentColor", size = "24" } = $props();
</script>

<svg xmlns="http://www.w3.org/2000/svg" style="width: {size}; height: {size};" viewBox="0 0 256 256"><path fill={fill} d="M128 24a104 104 0 1 0 104 104A104.13 104.13 0 0 0 128 24m36.44 110.66l-48 32A8.05 8.05 0 0 1 112 168a8 8 0 0 1-8-8V96a8 8 0 0 1 12.44-6.66l48 32a8 8 0 0 1 0 13.32"/></svg>