#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Lifetime life_time;
using lifetime_callback = void (*)(void*);

life_time* lifetime_new();
void lifetime_add(life_time* lt, lifetime_callback cb, void* ctx);
void lifetime_destroy(life_time* lt);
void lifetime_free(life_time* lt);

#ifdef __cplusplus
}
#endif
