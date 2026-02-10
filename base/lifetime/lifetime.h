#include "rust/rust_lifetime.h"

#ifdef _WIN32
#ifdef BASE_DLL_EXPORT
#define BASE_API __declspec(dllexport)
#else
#define BASE_API __declspec(dllimport)
#endif
#else
#define BASE_API
#endif

#include <functional>
#include <utility>

namespace talos::base {
namespace lifetime {
class BASE_API Lifetime {
public:
    Lifetime();
    ~Lifetime();

    Lifetime(const Lifetime&) = delete;
    Lifetime& operator=(const Lifetime&) = delete;

    Lifetime(Lifetime&& that) noexcept;
    Lifetime& operator=(Lifetime&& that) noexcept;

    // add 函数的作用:将一个复杂的 C++ 可调用对象包装成 Rust 能理解的简单 C 指针
    // 这是典型的"所有权转移"模式：
    // C++ 侧: 在 add 调用时，将可调用对象 fun 的生命周期从栈转移到了堆(heap_fun)
    // FFI 边界: 只传递一个不透明的 void* 指针给 Rust
    // Rust 侧: 在 api.rs 中，Rust 只是存储这个指针.它并不关心指针指向什么，也不负责释放它
    // 触发销毁:当Lifetime对象析构或手动调用 lifetime_destroy 时, Rust 会依次调用存储的 C 函数指针
    // 闭环: C 指针指向的“垫片”函数被执行, 它将 void* 转回 DecayedFun*, 执行逻辑后通过
    // delete 彻底回收内存
    template <typename Fun> void add(Fun&& fun) {
        // 类型退化
        // std::decay_t 把一个"奇怪形状的类型", 退化成一个"可以安全存储、new、delete的普通值类型"
        using DecayedFun = std::decay_t<Fun>;
        // 堆分配
        // 因为 Lambda 可能捕获了局部变量，它是有“体积”和“状态”的
        // 我们必须把它移动到堆上，才能保证 Rust 以后回调它时，这些数据依然有效
        auto heap_fun = new DecayedFun(std::forward<Fun>(fun));
        lifetime_add(
            _lt,
            // Shim（垫片）函数
            // 这是一个无捕获的 Lambda，可以自动转换为 C 风格函数指针
            [](void* ctx) {
                auto fn = static_cast<DecayedFun*>(ctx);
                // std::invoke 是 C++17 引入的标准方法，它能够统一处理普通函数、Lambda
                // 表达式、函数对象（Functors）以及成员函数指针
                std::invoke(*fn);
                // 自清理
                // 这一点至关重要：回调执行完后，由 C++ 侧负责释放 B 步骤分配的内存。
                delete fn;
            },
            heap_fun);
    }

    // 只负责销毁资源, 不销毁 lifetime 对象
    void destroy();

private:
    life_time* _lt = nullptr;
};

}  // namespace lifetime
}  // namespace talos::base