#ifndef DB_RUST_H
#define DB_RUST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RustDbOpaque RustDb;

RustDb *connect_db(const char *conn);

bool is_connected(RustDb *db);

void destroy_db(RustDb *db);


#ifdef __cplusplus
}
#endif

#endif //!< DB_RUST_H
