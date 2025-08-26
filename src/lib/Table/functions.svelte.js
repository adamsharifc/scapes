import csvData from '../../assets/MOCK_DATA.csv?raw';

/**
 * Parse CSV function
 * @param {string} csvText - The raw CSV text to parse
 * @param {{forceNumeric?: boolean, numericColumns?: string[]}} [options]
 *   - forceNumeric: if true, attempt to parse any leading number in a cell (e.g. "48000 kbps" -> 48000)
 *   - numericColumns: list of header names to force numeric parsing for (overrides forceNumeric for those columns)
 * @returns {Object[]} Array of parsed objects
 */
function parseCSV(csvText, options) {
    const opts = options || {};
    const forceNumeric = !!opts.forceNumeric;
    const numericColumns = Array.isArray(opts.numericColumns) ? opts.numericColumns : [];

    const lines = csvText.trim().split('\n');
    const headers = lines[0].split(',').map(h => h.trim());

    // regex to capture a leading integer/float (handles negative and decimals)
    const leadingNumberRE = /-?\d+(\.\d+)?/;

    return lines.slice(1).map((line, index) => {
        const values = line.split(',').map(v => v.trim());
        /** @type {Object.<string, any>} */
        const obj = { id: index + 1 }; // Add an ID field

        headers.forEach((header, i) => {
            let value = values[i] || '';
            // Remove surrounding quotes if present
            value = value.replace(/^"|"$/g, '');

            // If this column is requested to be numeric or global forceNumeric is enabled,
            // try to extract a leading number and keep original in a *_raw field.
            if (forceNumeric || numericColumns.includes(header)) {
                const m = value.match(leadingNumberRE);
                if (m) {
                    // store parsed numeric value
                    obj[header] = parseFloat(m[0]);
                    // keep the original string too in case UI needs it
                    obj[header + '_raw'] = value;
                    return;
                }
            }

            // Fallback: if it's a plain numeric string, convert to number
            if (!Number.isNaN(Number(value)) && value !== '') {
                obj[header] = parseFloat(value);
            } else {
                obj[header] = value;
            }
        });

        return obj;
    });
}

// Parse the CSV data
const csvUsers = parseCSV(csvData);

/**
 * Creates and manages the state for a sortable table.
 * @param {{forceNumeric?: boolean, numericColumns?: string[]}} [options]
 */
export function createTableLogic(options) {
    // parse CSV with the provided options so numeric coercion can be controlled
    const initialUsers = parseCSV(csvData, options);

    // === STATE ===
    // The raw data for the table
    const users = $state(initialUsers);

    // State to track the current sorting column and direction
    /** @type {string | null} */
    let _sortKey = $state(null);
    /** @type {'asc' | 'desc'} */
    let _sortDirection = $state('asc'); // 'asc' or 'desc'

    // === DERIVED STATE ===
    // A derived value that automatically re-calculates when its dependencies change.
    // This is more efficient than sorting the array in place on every click.
    const sortedUsers = $derived(() => {
        if (!_sortKey) {
            return [...users]; // Return a copy of the original array if no sort key
        }
        const keyString = /** @type {string} */ (_sortKey);
        const sorted = [...users].sort((a, b) => {
            const valA = /** @type {any} */ (a)[keyString];
            const valB = /** @type {any} */ (b)[keyString];

            // numeric compare if both are numbers
            if (typeof valA === 'number' && typeof valB === 'number') {
                return valA - valB;
            }

            // If one of them is numeric and the other is string but there is a *_raw field,
            // try to use numeric fallback from raw (useful when you kept parsed numbers in different properties)
            // (example: column 'rate' vs 'rate_raw')
            if (typeof valA === 'number' && typeof valB !== 'number') return -1;
            if (typeof valB === 'number' && typeof valA !== 'number') return 1;

            // fallback string comparison with numeric option (handles "48000 kbps" vs "42000 kbps" reasonably)
            const aStr = String(valA ?? '');
            const bStr = String(valB ?? '');
            return aStr.localeCompare(bStr, undefined, { numeric: true, sensitivity: 'base' });
        });

            if (_sortDirection === 'desc') {
            sorted.reverse();
        }

        return sorted;
    });

    // === PAGINATION ===
    /** @type {number} */
    let _page = $state(1);
    /** @type {number|string} */
    let _pageSize = $state(25); // allowed: 25,50,75,100 or 'All'

    const pageOptions = [25, 50, 75, 100, 'All'];

    const pagedUsers = $derived(() => {
    const all = sortedUsers();
    // read helpers: page/pageSize may be plain values or function-like $state values
    /** @param {any} v */
    const read = (v) => (typeof v === 'function' ? v() : v);
    const ps = read(_pageSize);
    const pval = Number(read(_page)) || 1;
    if (ps === 'All') return all;
    const size = Number(ps) || 25;
    const start = (Math.max(1, pval) - 1) * size;
    return all.slice(start, start + size);
    });

    const totalCount = $derived(() => sortedUsers().length);

    const totalPages = $derived(() => {
    /** @param {any} v */
    const read = (v) => (typeof v === 'function' ? v() : v);
    const ps = read(_pageSize);
    if (ps === 'All') return 1;
    const size = Number(ps) || 25;
    return Math.max(1, Math.ceil(sortedUsers().length / size));
    });

    /**
     * @param {string|number} size
     */
    /**
     * @param {string|number} size
     */
    function setPageSize(size) {
        _pageSize = size === 'All' ? 'All' : Number(size);
        // reset to first page when page size changes
        _page = 1;
    }

    /**
     * @param {string|number} n
     */
    /**
     * @param {string|number} n
     */
    function setPage(n) {
        /** @param {any} v */
        const read = (v) => (typeof v === 'function' ? v() : v);
        const tp = Number(read(totalPages)) || 1;
        const nn = Math.max(1, Math.min(Number(n) || 1, tp));
        _page = nn;
    }

    function nextPage() {
        /** @param {any} v */
        const read = (v) => (typeof v === 'function' ? v() : v);
        setPage((Number(read(_page)) || 1) + 1);
    }

    function prevPage() {
        /** @param {any} v */
        const read = (v) => (typeof v === 'function' ? v() : v);
        setPage((Number(read(_page)) || 1) - 1);
    }

    // === FUNCTIONS ===
    /** @param {any} v */
    const _read = (v) => (typeof v === 'function' ? v() : v);
    /**
     * Sorts the table by the given key.
     * Toggles direction if the same key is clicked again.
     * @param {string} key - The key of the object to sort by (e.g., 'name', 'email')
     */
    function sortBy(key) {
        if (_sortKey === key) {
            // If clicking the same column, toggle direction
            _sortDirection = _sortDirection === 'asc' ? 'desc' : 'asc';
        } else {
            // If clicking a new column, set it as the key and default to 'asc'
            _sortKey = key;
            _sortDirection = 'asc';
        }
    }

    function addUser() {
        const newUser = {
            id: users.length + 1,
            first_name: 'New',
            last_name: 'User',
            email: `new.user${users.length + 1}@example.com`,
            gender: 'Other',
            ip_address: '192.168.1.' + Math.floor(Math.random() * 255)
        };
        users.push(newUser);
    }

    // Return the state and functions to be used by the component
    return {
        // expose accessors so the consumer can read the live values as functions
        /** @returns {any[]} */
        sortedUsers: () => sortedUsers(),
        /** @returns {any[]} */
        pagedUsers: () => pagedUsers(),
        /** @returns {string|null} */
        sortKey: () => _read(_sortKey),
        /** @returns {'asc'|'desc'} */
        sortDirection: () => _read(_sortDirection),
        sortBy,
        addUser,

        // pagination (expose as functions/values)
    /** @returns {number} */
    page: () => _read(_page),
    /** @returns {number|string} */
    pageSize: () => _read(_pageSize),
        pageOptions,
        totalCount: () => totalCount(),
        totalPages: () => totalPages(),
        setPageSize,
        setPage,
        nextPage,
        prevPage
    };
}