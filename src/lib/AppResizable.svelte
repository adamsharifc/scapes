<script lang="ts">
    import { PaneGroup, Pane, PaneResizer } from "paneforge";

    let primarySidebar: ReturnType<typeof Pane>;
    let secondarySidebar: ReturnType<typeof Pane>;
    let panel: ReturnType<typeof Pane>;

    export function collapsePrimarySidebar() {
        primarySidebar?.collapse();
    }
    export function expandPrimarySidebar() {
        primarySidebar?.expand();
    }
    export function collapseSecondarySidebar() {
        secondarySidebar?.collapse();
    }
    export function expandSecondarySidebar() {
        secondarySidebar?.expand();
    }
    export function collapsePanel() {
        panel?.collapse();
    }
    export function expandPanel() {
        panel?.expand();
    }
</script>


<div class="pane-group">
    <PaneGroup direction="horizontal" class="pane-group" style="box-sizing: border-box;">
        <!-- Primary Sidebar (collapsible) -->
        <Pane
            defaultSize={15}
            collapsedSize={0}
            collapsible={true}
            minSize={0}
            bind:this={primarySidebar}
            style="display:flex; justify-content: center; align-items:center; min-width:0;"
        >
            <div style="flex: 1; display: flex; flex-direction: column; align-items: center; justify-content: center; color: white; background: #7C24E4; height: 100%; width: 100%;">
                <h2>Primary Sidebar</h2>
                <p>Navigation</p>
            </div>
        </Pane>

        <PaneResizer>
            <div class="pane-resizer-vertical"></div>
        </PaneResizer>

        <!-- Main Content Area (split into two rows) -->
        <Pane defaultSize={70} style="min-width:0; min-height:0; display:flex; flex-direction:column;">
            <PaneGroup direction="vertical" style="height:100%; min-height:0;">
                <!-- View (top row, not collapsible) -->
                    <Pane defaultSize={65} style="display:flex; justify-content: center; align-items:center; min-height:0;">
                            <div style="flex: 1; display: flex; flex-direction: column; align-items: center; justify-content: center; color: #202020; background: #f8f8f8; height: 100%; width: 100%;">
                                    <h1>View</h1>
                                    <p>Main workspace area</p>
                                    <div class="button-bar">
                                        <button on:click={collapsePrimarySidebar}>Collapse Primary Sidebar</button>
                                        <button on:click={expandPrimarySidebar}>Expand Primary Sidebar</button>
                                        <button on:click={collapseSecondarySidebar}>Collapse Secondary Sidebar</button>
                                        <button on:click={expandSecondarySidebar}>Expand Secondary Sidebar</button>
                                        <button on:click={collapsePanel}>Collapse Panel</button>
                                        <button on:click={expandPanel}>Expand Panel</button>
                                    </div>
                            </div>
                    </Pane>

                <PaneResizer>
                    <div class="pane-resizer-horizontal"></div>
                </PaneResizer>

                <!-- Panel (bottom row, collapsible) -->
                <Pane
                    defaultSize={35}
                    collapsedSize={0}
                    collapsible={true}
                    minSize={0}
                    bind:this={panel}
                    style="display:flex; justify-content: center; align-items:center; min-height:0;"
                >
                    <div style="flex: 1; display: flex; flex-direction: column; align-items: center; justify-content: center; color: #fff; background: #202020; height: 100%; width: 100%;">
                        <h3>Panel</h3>
                        <p>Status, logs, or other info</p>
                    </div>
                </Pane>
            </PaneGroup>
        </Pane>

        <PaneResizer>
            <div class="pane-resizer-vertical"></div>
        </PaneResizer>

        <!-- Secondary Sidebar (collapsible) -->
        <Pane
            defaultSize={15}
            collapsedSize={0}
            collapsible={true}
            minSize={0}
            bind:this={secondarySidebar}
            style="display:flex; justify-content: center; align-items:center; min-width:0;"
        >
            <div style="flex: 1; display: flex; flex-direction: column; align-items: center; justify-content: center; color: white; background: #3B7C2C; height: 100%; width: 100%;">
                <h2>Secondary Sidebar</h2>
                <p>Tools</p>
            </div>
        </Pane>
    </PaneGroup>
</div>

<style>
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
    .button-bar {
        display: flex;
        flex-wrap: wrap;
        gap: 0.5rem;
        margin-top: 1rem;
        justify-content: center;
    }
</style>