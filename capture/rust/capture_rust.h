//
// Created by lingt on 2025/12/31.
//

#ifndef TALOS_CAPTURE_RUST_H
#define TALOS_CAPTURE_RUST_H

namespace talos::screen_capture {
#ifdef __cplusplus
    extern "C" {
#endif
        bool init_capture(size_t width, size_t height);
        bool start_capture();
        bool stop_capture();
        unsigned char* get_frame_buffer();
        bool is_frame_ready();

#ifdef __cplusplus
    }
#endif
}


#endif //TALOS_CAPTURE_RUST_H
