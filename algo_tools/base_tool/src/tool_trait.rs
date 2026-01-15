pub trait ToolTrait {
    fn name(&self) -> &str;
    fn init(&self) -> bool;
    fn run(&self) -> i32;
    fn destroy(&self);
}
