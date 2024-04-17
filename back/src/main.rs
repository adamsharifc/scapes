mod database;
mod metadata_extract;
mod integrations;

fn main() {

	integrations::SendToTimeline(10.0, 21.0, 29.0, 44100, "07054102", "C:\\Users\\adams\\Music\\bbcsounds\\07054102.wav" , "protools-win") ;		
}