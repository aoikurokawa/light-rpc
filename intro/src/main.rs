use std::{net::TcpListener, time::SystemTime};

fn main() {
    let listner = TcpListener::bind("127.0.0.1:8080").unwrap();
    match listner.accept() {
        Ok((socket, addr)) => {
            let now = SystemTime::now();

            println!("new client: {addr:?}");
            println!("now: {:?}", now);
        }
        Err(e) => println!("couldn't get client: {e:?}"),
    }
}
