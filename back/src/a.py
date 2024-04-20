import sqlite3
import json

db = sqlite3.connect('test2.db')
cursor = db.cursor()


# cursor.execute("CREATE TABLE IF NOT EXISTS labrat (id INTEGER PRIMARY KEY, data JSON)")
# # split the data into multiple rows
# data = json.load(open('data.json', "r"))

# for i in range(len(data)):
#     print(data[i])
#     cursor.execute("INSERT INTO labrat (data) VALUES (?)", (json.dumps(data[i]),))

# db.commit()
# reference example
# SELECT
#     json_extract ( data, '$.Name' )
# FROM
#     movies
# WHERE
#     json_extract ( data, '$.ID' ) = 1;

query2 = "SELECT * FROM labrat WHERE json_extract ( data, '$.first_name' ) = 'Dorian';"
cursor.execute(query2)
results = cursor.fetchall()

if results:
    for row in results:
        print(row)
else:
    print("None")


json_string = '{"gender": "Male", "first_name": "Erich"}'
json_obj = json.loads(json_string)

query = "SELECT * FROM labrat WHERE " + " AND ".join([f"json_extract(data, '$.{k}') = '{v}'" for k, v in json_obj.items()])
cursor.execute(query)
results = cursor.fetchall()

if results:
    for row in results:
        print(row)
else:
    print("None")