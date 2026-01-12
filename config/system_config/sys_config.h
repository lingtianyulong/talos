//
// Created by xiaoqiang on 2025/4/5.
//

#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include <optional>
#include <string_view>

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct SystemConfig SystemConfig;

    SystemConfig* create_sys_config_from_json(const char* json);
    char* sys_config_to_json(SystemConfig* config);

    void free_sys_config(SystemConfig* config);
    void free_sys_config_from_json(const char* json);

    int get_width(const SystemConfig* config);
    int get_height(const SystemConfig* config);

#ifdef __cplusplus
}
#endif


namespace talos::sys::config {
    class RustSysConfig final {
    public:
        ~RustSysConfig();

        static RustSysConfig &getInstance() {
            static RustSysConfig instance;
            return instance;
        }

    private:
        RustSysConfig();

        RustSysConfig(const RustSysConfig &) = delete;

        SystemConfig &operator=(const RustSysConfig &) = delete;

        RustSysConfig(RustSysConfig &&) = delete;

        RustSysConfig &operator=(RustSysConfig &&) = delete;

    public:
        bool loadConfig(std::string_view config_path);

        void saveConfig(std::string_view savePath);

        void setWinWidth(int winWidth);

        void setWinHeight(int winHeight);

        std::optional<int> getWinWidth() const;

        std::optional<int> getWinHeight() const;

    private:
        int _winWidth; //!< 窗口宽度
        int _winHeight; //!< 窗口高度

        SystemConfig* _config;
    };
} //!< end namespace config::sys

#endif //SYS_CONFIG_H
