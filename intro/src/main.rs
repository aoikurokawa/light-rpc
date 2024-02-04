use std::{io::Write, net::TcpListener};

use chrono::Utc;

fn main() {
    let listner = TcpListener::bind("127.0.0.1:8080").unwrap();
    match listner.accept() {
        Ok((mut socket, addr)) => {
            let now = Utc::now();

            println!("new client: {addr:?}");
            println!("now: {:?}", now);
            socket
                .write_all(&now.timestamp().to_be_bytes())
                .expect("send write");
        }
        Err(e) => println!("couldn't get client: {e:?}"),
    }
}
