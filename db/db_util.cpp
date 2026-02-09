//
// Created by lingt on 2025/11/29.
//

#include "db_util.h"

namespace talos::db {

DbUtil::DbUtil() {}

DbUtil::~DbUtil() {
    if (_db) {
        _db.reset();
    }
}

bool DbUtil::init(std::string_view conn) {
    std::lock_guard lock(_mutex);
    RustDb* raw = connect_db(conn.data());
    if (!raw) {
        return false;
    }
    _db = RustDbPtr(raw, [](RustDb* db) {
        if (db) {
            destroy_db(db);
        }
    });
    return true;
}

bool DbUtil::isConnected() {
    std::lock_guard lock(_mutex);
    return _db && is_connected(_db.get());
}

// 获取底层指针
RustDb* DbUtil::raw() {
    std::lock_guard locker(_mutex);
    return _db.get();
}

// 主动关闭
void DbUtil::close() {
    std::lock_guard locker(_mutex);
    _db.reset();
}
}  // namespace talos::db