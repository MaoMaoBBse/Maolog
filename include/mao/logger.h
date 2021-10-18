#pragma once

#include "logevent.h"

#include <sstream>

namespace mao {

// 日志输出基类
class LogOut {
public:
  using ptr = std::shared_ptr<LogOut>;

  // 记录成功，返回true
  virtual bool log(const std::string& logger_name, const LogEvent& event) = 0;
  virtual ~LogOut() = default;

private:
};

// 标准输出
class SystamLogOut : public LogOut {
public:
  using ptr = std::shared_ptr<LogOut>;
  bool log(const std::string& format) override;

private:
};

// 文件输出
class FileLogOut : public LogOut {
public:
  using ptr = std::shared_ptr<FileLogOut>;
  FileLogOut(const std ::string& file_path = "log/log.txt");

  // 重新打开文件
  bool reopen();
  bool log(const std::string& format) override;

private:
  std::string m_file_path;
};

// 日志器
class Logger {
public:
  using ptr = std::shared_ptr<Logger>;
  Logger() = default;

  /**
   * 记录事件
   * 如果事件等级不足日志器类的等级，则无动作。
   */
  bool log(const LogEvent::ptr) {}

  void set_logout(LogOut::ptr nwe_logout);

private:
  std::string m_logger_name{"root"};
  LogLevel m_level{LogLevel::DEBUG};  // 记录事件的最低等级要求
  LogOut::ptr m_logout_ptr{std::make_shared<SystamLogOut>()};  // 日志输出目标
};

}  // namespace mao