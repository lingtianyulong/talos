#![deny(clippy::unwrap_used)]

fn main() {
    let x: Option<i32> = Some(5);
    let y = x.unwrap_or_default();

    println!("Value: {}", y);
    println!("Hello, world!");
}
