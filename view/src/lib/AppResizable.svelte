<script lang="ts">
	import { PaneGroup, Pane, PaneResizer } from "paneforge";
</script>

<div class="pane-group">
    <PaneGroup direction="vertical" class="pane-group" style="box-sizing: border-box;">

        <Pane defaultSize={65} >
            <PaneGroup direction="horizontal">
                
                <Pane defaultSize={23} style="display:flex; justify-content: center; align-items:center; background-color: var(--opacity-1); border-radius:var(--size-2);">
                    <span class="font-semibold">Primary Sidebar</span>
                </Pane>
        
                <PaneResizer>
                    <div class="pane-resizer-vertical">
                    </div>
                </PaneResizer>
        
                
                <Pane defaultSize={77} style="display:flex; justify-content: center; align-items:center; background-color: var(--opacity-2); border-radius:var(--size-2);">
                    <span class="font-semibold">Primary View</span>
                </Pane>
        
            </PaneGroup>
        </Pane>

        <PaneResizer>
            <div class="pane-resizer-horizontal">
            </div>
        </PaneResizer>

        <Pane defaultSize={35} style="display:flex; justify-content: center; align-items:center; background-color: var(--opacity-0); border-radius:var(--size-2);">
            <span style="font-weight: 600;">Panel</span>
        </Pane>
        
    </PaneGroup>
</div>

<style>

:root{
    --panel-color: var(--opacity-0);
    --primary-sidebar-color: var(--opacity-1);
    --primary-view-color: var(--opacity-2);
}

.pane-resizer-vertical{
    width: var(--resizer-width);
    height: 99%;
    background-color: transparent;
}
.pane-resizer-vertical:hover{
    background-color: var(--opacity-4);
    margin-top: var(--size-1);
    margin-bottom: var(--size-1);
    border-radius: var(--size-5);
    box-sizing: border-box;
}
.pane-resizer-horizontal{
    width: 99.5%;
    height: var(--resizer-width);
    background-color: transparent;
}
.pane-resizer-horizontal:hover{
    background-color: var(--opacity-4);
    margin-right: var(--size-1);
    margin-left: var(--size-1);
    border-radius: var(--size-5);
    box-sizing: border-box;
}
.pane-group{
    width: 100%;
    height: 100%;
    box-sizing: border-box;
}
</style>