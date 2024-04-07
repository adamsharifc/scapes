import sqlite3, json

DATABASE = 'data.db'
TABLE = 'test'
def connect():
    return sqlite3.connect(DATABASE)

def create_db():
    conn = connect()
    c = conn.cursor()

def create_table(table_name: str):
    conn = connect()
    c = conn.cursor()
    c.execute(f'CREATE TABLE {table_name} (id INTEGER PRIMARY KEY, data TEXT)')   

def insert_(table_name: str, data: str):
    conn = connect()
    c = conn.cursor()
    json_data = json.dumps(data)
    c.execute(f'INSERT INTO {table_name} (data) VALUES (?)', (json_data,))
    conn.commit()

def insert__(table_name: str, data: str):
    conn = connect()
    c = conn.cursor()
    json_data = json.dumps(data)
    c.execute(f"INSERT INTO {table_name} (data) VALUES (jsonb(?))", (json_data,))
    conn.commit()
#     insert_(TABLE, json_data)

def get_(table_name: str, where: str, equals_to: str):
    conn = connect()
    c = conn.cursor()
    FIELD_TO_GET = 'first_name'
    SELECTOR_FIELD = 'gender'
    SELCTOR_VALUE = '"Male"'
    query = f"SELECT json_extract ( data, '$.{FIELD_TO_GET}' ) FROM {table_name} WHERE data -> '$.{SELECTOR_FIELD}' = '{SELCTOR_VALUE}'"
    print(query)
    c.execute(query)
    print(c.fetchall()) 
    # return c.fetchall()

# get_(TABLE, 'first_name', 'John')
def show_tables():
    conn = connect()
    c = conn.cursor()
    c.execute("SELECT name FROM sqlite_master WHERE type='table';")
    print(c.fetchall())

# show_tables()

def mock_jsons():
    with open('mockdata.json') as f:
        data = json.loads(f.read())

    for i in range(1,51):
        with open(f'{i}.json', 'w') as f:
            f.write(json.dumps(data[i-1]))


# mock_jsons()
print(sqlite3.sqlite_version)
create_db()

for i in range(1,51):
    with open(f'{i}.json') as f:
        data = json.loads(f.read())
        print(data)
        insert__(TABLE, data)
