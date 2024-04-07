#![allow(dead_code)]

use rusqlite;
use rusqlite::Connection;
use serde_json::{self};
use std::time::SystemTime;
use sha2::Digest;
use std::collections::HashMap;

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
}

trait DocumentInput {
    fn apply(self, doc: &mut Document);
}
impl DocumentInput for &str {
    fn apply(self, doc: &mut Document) {
        if self.ends_with(".json") {
            doc.from_json_file(self);
        } else {
            doc.from_json(serde_json::Value::String(self.to_string()));
        }
    }
}
impl DocumentInput for serde_json::Value {
    fn apply(self, doc: &mut Document) {
        doc.from_json(self);
    }
}
impl DocumentInput for HashMap<String, serde_json::Value> {
    fn apply(self, doc: &mut Document) {
        doc.from_hashmap(self);
    }
}
macro_rules! document {
    ($input:expr) => {
        {
            let mut doc = Document::new();
            DocumentInput::apply($input, &mut doc);
            doc
        }
    };
    ({ $( $key:ident : $value:expr ),* }) => {
        {
            let mut doc = Document::new();
            let j = json!({ $(stringify!($key)) * $value, * });
            DocumentInput::apply(j, &mut doc);
            doc
        }
    };
}

pub fn main(){
    println!("Hello from database.rs");
    // let folder_path = "C:\\Users\\adams\\CharlieSierra\\GitHub\\scapes\\";
    let db = Database::new("test.db");
    let test = db.select_collection("test");
    
    
    let json = serde_json::json!({"name": "Adam", "age": 25});
    let json = serde_json::json!({"name$eq$": "Adam", "age$lt$": 25});
    
    
    // let doc = document!({"name": "Adam", "age": 25}); // Replace the colon with a comma
    // let _ = test.insert_one(doc);
    
}