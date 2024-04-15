<script>
    import colormap from 'colormap';
    import WaveSurfer from 'wavesurfer.js';
    import Regions from 'wavesurfer.js/dist/plugins/regions.esm.js'
    import Spectrogram from 'wavesurfer.js/dist/plugins/spectrogram.esm.js'
    import TimelinePlugin from 'wavesurfer.js/dist/plugins/timeline.esm.js'
    import Hover from 'wavesurfer.js/dist/plugins/hover.esm.js'

    import { onMount } from 'svelte';
    import { BaseDirectory } from '@tauri-apps/api/fs';
    import { convertFileSrc } from '@tauri-apps/api/tauri';
    const opacity_9 = "rgba(255, 255, 255, 0.875)";  // opacity-9 from the theme
    const opacity_7 = "rgba(255, 255, 255, 0.625)";  // opacity-7 frpm the theme
    const opacity_4 = "rgba(255, 255, 255, 0.25)";   // opacity-4 from the theme

    let colors = colormap({
        colormap: 'inferno',
        nshades: 256,
        format: 'float',
    });

    let waveform_container = null;  // reference to the div container
    let wavesurfer = null;          // reference to the wavesurfer instance
    let wavesurfer_regions = null;  // reference to the wavesurfer regions plugin instance  
    let wavesurfer_spectogram = null; // reference to the wavesurfer spectogram plugin instance

    let isRegionCreated = false;      // flag to check if a region is created
    let currentSelectedRegion = null; // reference to the current selected region

    onMount(async () => {
        wavesurfer = WaveSurfer.create({
            container: document.getElementById('waveform-container'),
            waveColor: opacity_9,
            progressColor: opacity_7,
            backend: "MediaElement",
        });
    
        // plugins
        wavesurfer_regions = wavesurfer.registerPlugin(Regions.create());
        wavesurfer_spectogram = wavesurfer.registerPlugin(Spectrogram.create(
            {
                colorMap: colors,
                // @ts-ignore
                container: document.querySelector('.spectogram-container'),
            }
        ));
        
        // play sound on interaction
        // @ts-ignore
        wavesurfer.on('interaction', (region) =>{
            wavesurfer.play();
        });

        // wavesurfer_regions plugin
        wavesurfer_regions.enableDragSelection({
            color: opacity_4,
        })
        // wavesurfer_regions 
        wavesurfer_regions.on('region-created', (region) => {
            if (isRegionCreated){
                currentSelectedRegion.remove();
            }
            isRegionCreated = true;
            currentSelectedRegion = region;
        });

    });

    async function loadSound(){
        let filePath = "C:\\Users\\adams\\Music\\Spotify\\Always in My Head.mp3";
        const assetURL = convertFileSrc(filePath);
        /** @type {HTMLMediaElement}*/
        // @ts-ignore
        const audio_asset = document.getElementById('audio_asset');
        audio_asset.src = assetURL;
        audio_asset.load();
        wavesurfer.load(audio_asset.src);
    }

    // remove the current selected region on mouse down so that a new region can be created
    function handleMouseDown(){ 
        if (isRegionCreated){
            currentSelectedRegion.remove();
            isRegionCreated = false;
        }
    }

</script>

<div class="container">
    <button type="button" on:click={loadSound}>load sound</button>
    
    <div class="waveform-spectogram-container" on:mousedown={handleMouseDown}>
        <div bind:this={waveform_container} id="waveform-container" class="waveform-container">
            <!-- Waveform is rendered here -->
        </div>
    
        <div class="spectogram-container">
            <!-- Spectogram is rendered here -->
        </div>
    </div>
    
    <!-- Audio element to quickly load and use local audio files using the tauri api -->
    <audio id="audio_asset"></audio>
</div>

<style>
.container{
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100%;
    width: 100%;
}
#waveform-container{
    width: 100%;
    height: 100%;
}
.spectogram-container{
    width: 100%;
    height: 100%;
}
.waveform-spectogram-container{
    display: grid;
    grid-template: 1fr / 1fr;
    place-items: center;
    width: 100%;
}
.waveform-spectogram-container > * {
    grid-column: 1 / 1;
    grid-row: 1 / 1;
    align-self: center;
    justify-self: center;
}
.waveform-spectogram-container .spectogram-container{
    height: 100%;
    z-index: 1;
}
.waveform-spectogram-container .waveform-container{
    height: 100%;
    z-index: 2;
}

</style>