mod shapes;
pub use shapes::shape::Shape;

use shapes::point::Point2D;
use shapes::rect::Rect;

use crate::shapes::point::Point3D;

pub type Point2d = Point2D<i32>;
pub type Point3d = Point3D<i32>;
pub type Point2df = Point2D<f64>;
pub type Point3df = Point2D<f64>;

#[unsafe(no_mangle)]
pub extern "C" fn create_rect(x: i32, y: i32, width: i32, height: i32) -> *mut Rect {
    Box::into_raw(Box::new(Rect::new(x, y, width, height)))
}

#[unsafe(no_mangle)]
pub extern "C" fn destroy_rect(rect: *mut Rect) {
    if rect.is_null() {
        return;
    }
    unsafe {
        drop(Box::from_raw(rect));
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn create_point(x: i32, y: i32) -> *mut Point2d {
    Box::into_raw(Box::new(Point2d::new(x, y)))
}

#[unsafe(no_mangle)]
pub extern "C" fn destroy_point(pt: *mut Point2d) {
    if pt.is_null() {
        return;
    }
    unsafe {
        drop(Box::from_raw(pt));
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn create_point3d(x: i32, y: i32, z: i32) -> *mut Point3d {
    Box::into_raw(Box::new(Point3d::new(x, y, z)))
}

#[unsafe(no_mangle)]
pub extern "C" fn destroy_point3d(pt: *mut Point3d) {
    if pt.is_null() {
        return;
    }

    unsafe {
        drop(Box::from_raw(pt));
    }
}
