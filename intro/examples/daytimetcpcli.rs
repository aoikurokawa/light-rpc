use std::{env, io::Read, net::TcpStream};

fn main() {
    let args: Vec<String> = env::args().collect();

    let addr = args.get(1).expect("get addr");
    let mut stream = TcpStream::connect(addr).expect("connect");

    let mut buf = [0; 8];
    stream.read_exact(&mut buf).expect("read buf");

    println!("Timestamp: {}", i64::from_be_bytes(buf));
}
