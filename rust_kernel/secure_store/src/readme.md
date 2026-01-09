### 项目说明

使用 rust 实现一个类 QtKeychain 库的功能，主要用于存储登录接口中返回的 `token`；

### 项目结构

```text
secure_store/
├─ Cargo.toml
└─ src/
   ├─ lib.rs          # C ABI 对外接口
   ├─ store.rs        # 安全存储核心逻辑
   └─ error.rs        # 错误定义

```

