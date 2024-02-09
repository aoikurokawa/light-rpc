use std::{
    io::{Read, Write},
    net::TcpListener,
};
use tracing::{debug, error, info, span, trace, warn, Level};

use chrono::Utc;

#[tokio::main]
async fn main() {
    tracing_subscriber::fmt::init();

    for _file in std::env::args() {
        let span = span!(Level::INFO, "file");
        let _guard = span.enter();
        info!("opening the file");
        // let mut file = std::fs::File::open(file).unwrap();
        info!("reading file contents");
        // let mut bytes = Vec::new();
        // file.read(&mut bytes).unwrap();
        info!("parsing");

        info!("done with that")
    }
}

pub fn tcp() {
    let listner = TcpListener::bind("127.0.0.1:8080").unwrap();
    loop {
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
}
