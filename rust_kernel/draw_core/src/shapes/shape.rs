pub trait Shape {
    fn draw(&self);
    fn drag(&mut self, offset_x: i32, offset_y: i32);
    fn modify(&mut self, x: i32, y: i32);
    fn move_to(&mut self, x: i32, y: i32);

    fn to_string(&self) -> String;
    fn to_json(&self) -> String;
    fn from_json(&self, json: String) -> Self;
}
