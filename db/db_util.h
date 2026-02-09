//
// Created by lingt on 2025/11/29.
//

#ifndef TALOS_DB_UTIL_H
#define TALOS_DB_UTIL_H

#include "rust/db_rust.h"

#include <functional>
#include <memory>
#include <mutex>

using namespace std;

namespace talos::db {

using RustDbPtr = std::unique_ptr<RustDb, std::function<void(RustDb*)>>;

class DbUtil {
public:
    ~DbUtil();

    // C++11 起线程安全
    static DbUtil& instance() {
        static DbUtil db;
        return db;
    }

    bool init(std::string_view conn);
    bool isConnected();
    RustDb* raw();
    void close();

private:
    DbUtil();

    // 禁止复制和移动
    DbUtil(const DbUtil&) = delete;
    DbUtil& operator=(const DbUtil&) = delete;
    DbUtil(DbUtil&&) = delete;
    DbUtil& operator=(DbUtil&&) = delete;

    RustDbPtr _db{nullptr, [](RustDb* db) { /*默认空删除器*/ }};
    std::mutex _mutex;
};
}  // namespace talos::db

#endif  // TALOS_DB_UTIL_H
