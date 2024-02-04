use std::{env, io::Write, net::TcpStream};

fn main() {
    let args: Vec<String> = env::args().collect();

    let addr = args.get(1).expect("get addr");
    let mut stream = TcpStream::connect(addr).expect("connect");

    stream.write(&[0]).expect("write");
}
