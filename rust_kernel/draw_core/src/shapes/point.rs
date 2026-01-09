use num_traits::Num;
use serde::{Deserialize, Serialize};
use std::ops::{Add, Div, Mul, Sub};

#[derive(Debug, Clone, Copy, PartialEq, Serialize, Deserialize)]
#[allow(dead_code)]
#[repr(C)]
pub struct Point2D<T>
where
    T: Num + Copy,
{
    pub x: T,
    pub y: T,
}

#[allow(dead_code)]
impl<T> Point2D<T>
where
    T: Num + Copy,
{
    pub fn new(x: T, y: T) -> Self {
        Self { x, y }
    }
}

impl<T> Add for Point2D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl<T> Sub for Point2D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl<T> Mul for Point2D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        Self {
            x: self.x * other.x,
            y: self.y * other.y,
        }
    }
}

impl<T> Div for Point2D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn div(self, other: Self) -> Self {
        if other.x.is_zero() || other.y.is_zero() {
            panic!("除数不能为0");
        }
        Self {
            x: self.x / other.x,
            y: self.y / other.y,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Serialize, Deserialize)]
#[allow(dead_code)]
#[repr(C)]
pub struct Point3D<T>
where
    T: Num + Copy,
{
    pub x: T,
    pub y: T,
    pub z: T,
}

#[allow(dead_code)]
impl<T> Point3D<T>
where
    T: Num + Copy,
{
    pub fn new(x: T, y: T, z: T) -> Self {
        Self { x, y, z }
    }
}

impl<T> Add for Point3D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z,
        }
    }
}

impl<T> Sub for Point3D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z,
        }
    }
}

impl<T> Mul for Point3D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        Self {
            x: self.x * other.x,
            y: self.y * other.y,
            z: self.z * other.z,
        }
    }
}

impl<T> Div for Point3D<T>
where
    T: Num + Copy,
{
    type Output = Self;
    fn div(self, other: Self) -> Self {
        if other.x.is_zero() || other.y.is_zero() || other.z.is_zero() {
            panic!("除数不能为0");
        }
        Self {
            x: self.x / other.x,
            y: self.y / other.y,
            z: self.z / other.z,
        }
    }
}
