use std::net::{SocketAddrV4, TcpListener};

fn main() {
    let listner = TcpListener::bind("127.0.0.1:8080").unwrap();
    match listner.accept() {
        Ok((_socket, addr)) => println!("new client: {addr:?}"),
        Err(e) => println!("couldn't get client: {e:?}"),
    }
}
