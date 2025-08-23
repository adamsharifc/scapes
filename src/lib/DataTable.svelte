<script>

let data = {
    files: [
        {id: 1, name: "File 1", description: "Description 1", channels: "Channels 1", bitrate: "Bitrate 1"},
        {id: 2, name: "File 2", description: "Description 2", channels: "Channels 2", bitrate: "Bitrate 2"},
        {id: 3, name: "File 3", description: "Description 3", channels: "Channels 3", bitrate: "Bitrate 3"},
        {id: 4, name: "File 4", description: "Description 4", channels: "Channels 4", bitrate: "Bitrate 4"}
    ]
}
let selectedId = null;

function selectRow(id){
    selectedId = id;
}

$: {
    const rows = document.querySelectorAll('.table tbody tr');
    rows.forEach(row => {
        const id = parseInt(row.id);
        if (id === selectedId) {
            row.classList.add('selected');
        } else {
            row.classList.remove('selected');
        }
    });
}


</script>

<div class="container">
    <div class="table-wrapper">
        <table class="table sticky">
            <thead>
                <tr>
                    <th>File Name</th>
                    <th>Description</th>
                    <th>Channels</th>
                    <th>Bitrate</th>
                </tr>
            </thead>
            <tbody>
                {#each data.files as file (file.id)}
                    <tr data-id={file.id} on:click={() => selectRow(file.id)}>
                        <td>{file.name}</td>
                        <td>{file.description}</td>
                        <td>{file.channels}</td>
                        <td>{file.bitrate}</td>
                    </tr>
                {/each}
            </tbody>
        </table>
    </div>
</div>


<style>
.container{
    display: flex;
    min-height: var(--size-13);
    min-width: var(--size-15);
    box-sizing: border-box;
    background-color: var(--opacity-1);
    border-radius: var(--size-1);
}
.table-wrapper{
    max-height: var(--size-13);
    min-width: 100%;
    overflow-y: auto;
    scroll-snap-type: y proximity;
    font-size: var(--size-3);
}
.table{
    border-collapse: collapse;
    width: 100%;
}
.table th, .table td{
    padding: var(--size-1) var(--size-2);
    text-align: left;
}
.table th{
    background: transparent;
    font-weight: 600;
    color: var(--opacity-7);
    user-select: none;
}
.table tbody tr{
    scroll-snap-align: end;
}
table tbody tr:nth-child(odd){
    background-color: var(--opacity-0);
    color: var(--opacity-6);
    font-weight: medium;
}
table tbody tr:nth-child(even){
    background-color: transparent;
    color: var(--opacity-6);
    font-weight: medium;
}
table tbody tr:hover{
    background-color: var(--opacity-2);
    color: var(--opacity-7);
}
tr.selected{
    color: blue;
}
.table.sticky th{
    position: sticky;
    top: 0;
}
/** bottom border */
.table.sticky th::after{
    content: "";
    width: 100%;
    height: 2px;
    position: absolute;
    bottom: 0;
    left: 0;
    background: transparent;
}
</style>