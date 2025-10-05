<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "currentColor", size = "24" } = $props();
</script>

<svg xmlns="http://www.w3.org/2000/svg" style="width: {size}; height: {size};" fill={fill} viewBox="0 0 256 256"><path d="M200,40H56A16,16,0,0,0,40,56V200a16,16,0,0,0,16,16H200a16,16,0,0,0,16-16V56A16,16,0,0,0,200,40Zm0,16v72H56V56Z"></path></svg>