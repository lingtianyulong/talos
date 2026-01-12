//
// Created by lingt on 2025/12/15.
//

#ifndef TALOS_DUMP_HELPER_H
#define TALOS_DUMP_HELPER_H

namespace utils {
#ifdef __cplusplus
    extern "C" {
#endif
        bool init_dump(const char* dump_dir);
#ifdef __cplusplus
    }
#endif
}

#endif //TALOS_DUMP_HELPER_H
