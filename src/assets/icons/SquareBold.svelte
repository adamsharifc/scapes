<script>
    /**
     * @typedef {Object} Props
     * @property {string} [fill]
     * @property {string} [size]
     */

    /** @type {Props} */
    let { fill = "#000000", size = "24" } = $props();
</script>


<svg xmlns="http://www.w3.org/2000/svg" width={size} height={size} viewBox="0 0 256 256"><path fill={fill} d="M208 28H48a20 20 0 0 0-20 20v160a20 20 0 0 0 20 20h160a20 20 0 0 0 20-20V48a20 20 0 0 0-20-20m-4 176H52V52h152Z"/></svg>