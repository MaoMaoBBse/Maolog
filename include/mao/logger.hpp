#pragma once

#include <iostream>

#include "logformat.hpp"

namespace mao {

// 标准日志器
class Logger {
public:
  using ptr = std::shared_ptr<Logger>;

  /**
   * Logger构造函数
   * @param name 本日志器的名称
   * @param log_min_level 本日志器记录的事件的最低等级要求
   * @param format_ptr 格式化器，用于格式化事件
   */
  Logger(const std::string& name = "root",
         const LogLevel& log_min_level = LogLevel::DEBUG,
         const LogFormat::ptr format_ptr = std::make_shared<LogFormat>())
      : m_name(name), m_level(log_min_level), m_format_ptr(format_ptr) {}

  /**
   * 记录事件
   * 如果事件等级不足日志器类的等级，则无动作。
   */
  virtual bool log(const LogEvent::ptr event_ptr) {
    if (event_ptr->getLevel() < m_level)
      return false;
    const auto& logger_name_scheme{
        m_format_ptr->getScheme(Attribute::LOGGER_NAME)};
    auto& os{std::cout};
    if (logger_name_scheme.is_show) {
      os << logger_name_scheme.left_str << m_name
         << logger_name_scheme.right_str;
    }
    os << m_format_ptr->format(event_ptr);
    return true;
  }

  virtual bool log(const LogEvent& event) {
    return log(std::make_shared<LogEvent>(event));
  }

  virtual ~Logger() = default;

private:
  std::string m_name;           // 日志器名称
  LogLevel m_level;             // 记录事件的最低等级要求
  LogFormat::ptr m_format_ptr;  // 格式化事件方法
};

}  // namespace mao