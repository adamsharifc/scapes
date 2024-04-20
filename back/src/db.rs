#![allow(dead_code)]
#![allow(unused_imports)]

use rusqlite::{self, params};
use rusqlite::Connection;
use serde_json::{self};
use std::time::SystemTime;
use sha2::Digest;
use std::collections::HashMap;
use std::fs::File;
use std::io::Error;

pub struct Database {
    connection: Connection,
    database_path: String,
}
impl Database {

    fn connect(database_path: &str) -> rusqlite::Connection {
        return Core::connect(database_path);
    }
    fn new(database_path: &str) -> Database {
        let conn = Self::connect(database_path);
        Database {
            connection: conn,
            database_path: database_path.to_string(),
        }
    }
    fn create_collection(&self, collection_name: &str)-> Result<Collection, rusqlite::Error> {
        let query = format!("CREATE TABLE {} (id TEXT PRIMARY KEY, data TEXT)", collection_name);
        let res = self.connection.execute(&query, []);  
        match res {
            Ok(_) => Ok(Collection::new(collection_name, self.database_path.as_str())),
            Err(e) => Err(e)
        }
    }
    fn drop_collection(&self, collection_name: &str){
        let query = format!("DROP TABLE {}", collection_name);
        let res = self.connection.execute(&query, []);
        match res {
            Ok(_) => (),
            Err(e) => println!("Error dropping table {}: {}", collection_name, e)
        }
    }
    fn get_collections(&self) -> Result<Vec<String>, rusqlite::Error> {
        let mut stmt = self.connection.prepare("SELECT name FROM sqlite_schema WHERE type='table'")?;
        let mut rows = stmt.query([])?;
        let mut tables = Vec::new();

        while let Some(row) = rows.next()? {
            tables.push(row.get(0)?);
        }
        return Ok(tables);
    }
    fn show_collections(&self){
        let tables = self.get_collections().unwrap();
        for table in tables {
            println!("{}", table);
        }
    }
    fn select_collection(&self, collection_name: &str) -> Collection {
        return Collection::new(collection_name, &self.database_path);
    }
}
pub struct Collection{
    collection_name: String,
    database_path: String,
    connection: Connection,
}
impl Collection {
    fn new(collection_name: &str, database_path: &str) -> Collection {
        let c = Collection {
            collection_name: collection_name.to_string(),
            database_path: database_path.to_string(),
            connection: Core::connect(database_path),
        };
        return c;
    }
    fn insert_one(&self, doc: Document)-> Result<(), rusqlite::Error>{
        let data = doc.to_string();
        let mut stmt = self.connection.prepare(format!("INSERT OR REPLACE INTO {} (id, data) VALUES (:id, (jsonb(:data)))", self.collection_name).as_str())?;
        stmt.execute(&[(":id", &doc.id), (":data", &data)])?;
        Ok(())
    }
    fn insert_many(&self, docs: Vec<Document>)-> Result<(), rusqlite::Error>{
        for doc in docs {
            let _ = self.insert_one(doc);
        }
        Ok(())
    }
    fn find(&self, json_string: &str) -> Result<(), rusqlite::Error> {
        
        

        let json_obj: serde_json::Value = serde_json::from_str(json_string).map_err(|e| rusqlite::Error::InvalidParameterName(e.to_string()))?;
        let query = Core::generate_query(json_string).unwrap();
        
        // let params from json_string values to params![]







        let mut stmt = self.connection.prepare(&query)?;
        let results = stmt.query_map(params![], |row| {
            Ok(row.get::<_, i64>(0)?)
        })?;
    
        let mut found = false;
        for result in results {
            match result {
                Ok(id) => {
                    println!("Found row with id: {}", id);
                    found = true;
                }
                Err(e) => println!("Failed to get row: {}", e),
            }
        }
    
        if !found {
            println!("None");
        }
    
        Ok(())
    }

    fn update_one(){

    }
    fn update_many(){

    }
    fn replace_one(){

    }
    fn replace_many(){

    }
    fn delete_one(){

    }
    fn delete_many(){

    }
    fn count_documents(){

    }
    fn custom_command(){

    }
}

pub struct Document {
    id: String,
    timestamp: f64,
    data: serde_json::Value,
}
impl Document {

    fn new() -> Document {
        Document {
            id: String::new(),
            timestamp: 0.0,
            data: serde_json::Value::Null,
        }
    }
    fn from_json_file(&mut self, file_path: &str){
        let json_ = Core::read_json(file_path);
        self.data = json_;
        self.timestamp = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs_f64();
        self.id = self.generate_id();
        self.data["__id__"] = self.id.clone().into();
        self.data["__timestamp__"] = self.timestamp.clone().into();
    }
    fn from_json(&mut self, json: serde_json::Value){
        self.data = json;
        self.timestamp = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs_f64();
        self.id = self.generate_id();
        self.data["__id__"] = self.id.clone().into();
        self.data["__timestamp__"] = self.timestamp.clone().into();
    }
    fn from_hashmap(&mut self, hashmap: std::collections::HashMap<String, serde_json::Value>){
        self.data = serde_json::json!(hashmap);
        self.timestamp = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs_f64();
        self.id = self.generate_id();
        self.data["__id__"] = self.id.clone().into();
        self.data["__timestamp__"] = self.timestamp.clone().into();
    }
    fn from_string(&mut self, data: &str){
        self.data = serde_json::from_str(data).unwrap();
        self.timestamp = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs_f64();
        self.id = self.generate_id();
        self.data["__id__"] = self.id.clone().into();
        self.data["__timestamp__"] = self.timestamp.clone().into();
    }
    fn dump_document(&self, file_path: &str){
        Core::write_json(file_path, &self.data);
    }
    fn generate_id(&self) -> String {
        let mut hasher = sha2::Sha256::new();
        hasher.update(self.data.to_string());
        let result = hasher.finalize();
        return format!("{:x}", result);
    }
    fn to_string(&self) -> String {
        return self.data.to_string();
    }

}

pub struct Core{

}
impl Core{
    fn connect(database_path: &str) -> rusqlite::Connection {
        rusqlite::Connection::open(database_path).unwrap()
    }
    fn read_json(file_path: &str)-> serde_json::Value {
        let file = std::fs::File::open(file_path).unwrap();
        let reader = std::io::BufReader::new(file);
        let json: serde_json::Value = serde_json::from_reader(reader).unwrap();
        return json;
    }
    fn write_json(file_path: &str, json: &serde_json::Value){
        let file = std::fs::File::create(file_path).unwrap();
        let writer = std::io::BufWriter::new(file);
        serde_json::to_writer_pretty(writer, json).unwrap();
    }
    fn generate_query(json_string: &str) -> Result<String, Error> {
        let json_obj: serde_json::Value = serde_json::from_str(json_string)?;
    
        let mut query = String::from("SELECT * FROM labrat WHERE ");
        let mut params: Vec<_> = Vec::new();
        let mut count = 1;
        if let Some(obj) = json_obj.as_object() {
            for (k, v) in obj.iter() {
                query.push_str(&format!("json_extract (data, '$.{}') = ?{} AND ", k, count));
                params.push(v.as_str().unwrap_or_default());
                count += 1;
            }
            query.pop();query.pop();query.pop(); // Remove the last 'AND'
            query.pop(); // Remove the extra space
        }
    
        Ok(query)
    }
}


pub fn main(){
    println!("Hello from database.rs");
    let db = Database::new("test2.db");
    db.show_collections();
    let labrat = db.select_collection("labrat");
    
    let json_string = r#"{"gender": "Male", "first_name": "Erich"}"#;
    let gen_sql = Core::generate_query(json_string).unwrap();
    println!("{}", gen_sql);
    
    
    let sql= "SELECT * FROM labrat WHERE json_extract (data, '$.first_name') = ?1 AND json_extract (data, '$.gender') = ?2";
    println!("{}", sql);
    
    
    
    let json_value: serde_json::Value = serde_json::from_str(json_string).unwrap();

    let mut params = Vec::new();
    if let serde_json::Value::Object(map) = json_value {
        for (_key, value) in map {
            if let serde_json::Value::String(s) = value {
                params.push(s);
            }
        }
    }
    let params = rusqlite::params_from_iter(params);    

    let mut stmt = labrat.connection.prepare(sql).unwrap();


    let mut stmt = labrat.connection.prepare(sql).unwrap();
    let rows = stmt.query_map(params, |row| {
        let json_data: String = row.get("data")?;
        Ok(json_data)
    }).unwrap();

    for row in rows {
        match row {
            Ok((json_data)) => println!("Gender: {}", json_data),
            Err(e) => println!("Failed to get row: {}", e),
        }
    }
}