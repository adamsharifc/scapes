<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "currentColor", size = "24" } = $props();
</script>

<svg xmlns="http://www.w3.org/2000/svg" style="width: {size}; height: {size};" fill={fill} viewBox="0 0 256 256"><path d="M216,40H40A16,16,0,0,0,24,56V200a16,16,0,0,0,16,16H216a16,16,0,0,0,16-16V56A16,16,0,0,0,216,40Zm0,160H40V56H216V200Z"></path></svg>