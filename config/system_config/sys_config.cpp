//
// Created by xiaoqiang on 2025/4/5.
//
#include "sys_config.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>


#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>


#include "logger/logger.h"

namespace talos::sys::config {
RustSysConfig::RustSysConfig() {
  _winWidth = 1280;
  _winHeight = 720;
  _config = nullptr;
}

RustSysConfig::~RustSysConfig() {
  if (_config != nullptr) {
    free_sys_config(_config);
  }
}

bool RustSysConfig::loadConfig(std::string_view config_path) {
  if (!std::filesystem::exists(config_path)) {
    Logger::Error(std::format(
        "当前加载的系统的配置文件路径不存在, 配置文件路径为:{}", config_path));
    return false;
  }

  // 打开文件
  std::ifstream ifs(config_path.data());
  if (!ifs.is_open()) {
    Logger::Error(std::format("{} does not exist", config_path));
    return false;
  }

  std::stringstream stream;
  stream << ifs.rdbuf();
  std::string json = stream.str();

  _config = create_sys_config_from_json(json.c_str());
  if (_config == nullptr) {
    return false;
  }

  _winHeight = get_width(_config);
  _winWidth = get_height(_config);
  return true;
}

void RustSysConfig::saveConfig(std::string_view savePath) {
  // 创建 json 数据
  QJsonObject obj;
  obj["version"] = "1.0.0.0";
  obj["WinWidth"] = _winWidth;
  obj["WinHeight"] = _winHeight;

  // 打开文件
  QFile file(savePath.data());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Logger::Error(std::format("{} does not exist", savePath));
    return;
  }

  QJsonDocument doc(obj);
  QString json = doc.toJson(QJsonDocument::Indented);

  QTextStream stream(&file);
  stream.setEncoding(QStringConverter::Utf8);
  stream << json;
  stream.flush();

  file.close();
}

void RustSysConfig::setWinWidth(int winWidth) { _winWidth = winWidth; }

void RustSysConfig::setWinHeight(int winHeight) { _winHeight = winHeight; }

std::optional<int> RustSysConfig::getWinWidth() const { return _winWidth; }

std::optional<int> RustSysConfig::getWinHeight() const { return _winHeight; }
} // namespace talos::sys::config
