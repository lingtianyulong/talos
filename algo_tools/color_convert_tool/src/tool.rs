use base_tool::ToolTrait;

pub struct ColorConvertTool {
    name: String,
}

impl ColorConvertTool {
    pub fn new(name: String) -> Self {
        Self { name }
    }
}

impl ToolTrait for ColorConvertTool {
    fn name(&self) -> &str {
        &self.name
    }

    fn init(&self) -> bool {
        println!("init color convert tool");
        true
    }

    fn run(&self) -> i32 {
        println!("run color convert tool");
        0
    }

    fn destroy(&self) {
        // 什么都不做
        println!("destroy color convert tool");
    }
}
