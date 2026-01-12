use crate::shapes::point::Point2D;
use crate::shapes::shape::Shape;
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
#[allow(dead_code)]
pub struct Rect {
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,

    pub top_left: Point2D<i32>,
    pub top_right: Point2D<i32>,
    pub bottom_left: Point2D<i32>,
    pub bottom_right: Point2D<i32>,
}

#[allow(dead_code)]
impl Rect {
    pub fn new(x: i32, y: i32, width: i32, height: i32) -> Self {
        Self {
            x,
            y,
            width,
            height,
            top_left: Point2D::new(x, y),
            top_right: Point2D::new(x + width, y),
            bottom_left: Point2D::new(x, y + height),
            bottom_right: Point2D::new(x + width, y + height),
        }
    }
}

impl Shape for Rect {
    fn draw(&self) {
        println!("Rect: {:#?}", self);
    }

    fn drag(&mut self, offset_x: i32, offset_y: i32) {
        // 注意：这里需要可变引用才能修改，当前实现无法修改
        self.x += offset_x;
        self.y += offset_y;

        // self.top_left.x += offset_x;
        // self.top_left.y += offset_y;
        // self.top_right.x += offset_x;
        // self.top_right.y += offset_y;
        // self.bottom_left.x += offset_x;
        // self.bottom_left.y += offset_y;
        // self.bottom_right.x += offset_x;
        // self.bottom_right.y += offset_y;

        // self.width = self.top_right.x - self.top_left.x;
        // self.height = self.bottom_right.y - self.top_left.y;
    }

    fn modify(&mut self, x: i32, y: i32) {
        // 注意：这里需要可变引用才能修改，当前实现无法修改
        self.x = x;
        self.y = y;

        // self.top_left.x = x;
        // self.top_left.y = y;
        // self.top_right.x = x + self.width;
        // self.top_right.y = y;
        // self.bottom_left.x = x;
        // self.bottom_left.y = y + self.height;
        // self.bottom_right.x = x + self.width;
        // self.bottom_right.y = y + self.height;
    }

    fn move_to(&mut self, x: i32, y: i32) {
        // 注意：这里需要可变引用才能修改，当前实现无法修改
        self.x = x;
        self.y = y;

        // self.top_left.x = x;
        // self.top_left.y = y;
        // self.top_right.x = x + self.width;
        // self.top_right.y = y;
        // self.bottom_left.x = x;
        // self.bottom_left.y = y + self.height;
        // self.bottom_right.x = x + self.width;
        // self.bottom_right.y = y + self.height;
    }

    fn to_string(&self) -> String {
        format!(
            "x: {}, y: {}, width: {}, height: {}",
            self.x, self.y, self.width, self.height
        )
    }

    fn to_json(&self) -> String {
        serde_json::to_string(self).unwrap()
    }

    fn from_json(&self, json: String) -> Self {
        serde_json::from_str(&json).unwrap()
    }
}
