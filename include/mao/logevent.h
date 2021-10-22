#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

namespace mao {
// 日志级别
enum class LogLevel { DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, FATAL = 5 };

// 将日志级别转换成字符串
std::string logLeveltoStr(const LogLevel& level);

// 每条属性的id
enum class Attribute {
  FIME_NAME = 1,  // 文件名
  LEVEL,          // 等级
  LINE,           // 行号
  THREAD_ID,      // 线程id
  TIME,           // 时间戳
  CONTENT         // 内容
};

// 日志事件
class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;
  LogEvent() = default;

  // 设置行号
  void setLine(const uint32_t& line);

  // 设置日志等级
  void setLogLevel(const LogLevel& level);

  // 设置文件名
  void setFileName(const std::string& file_name);

  // 设置日志内容
  template <typename... T>
  void setContent(const std::initializer_list<T...>& content_list) {
    std::stringstream ss{};
    for (const auto& v : content_list) {
      ss << v;
    }
    m_content = ss.str();
  }

  template <typename T>
  T get(const Attribute& att) const {
    switch (att) {
      case Attribute::CONTENT:
        return m_content;
        break;

      default:
        break;
    }
  }

private:
  std::string m_file{};                // 文件名
  LogLevel m_level = LogLevel::DEBUG;  // 事件等级
  uint32_t m_line{};                   // 行号
  uint32_t m_threadid{};               // 线程id
  std::time_t m_time{};                // 时间戳
  std::string m_content{};             // 内容
};

template <typename T>
class get {
public:
  T operator()(const LogEvent& event, const Attribute& att) {}

  T operator()(LogEvent& event, const Attribute& att) {}
};

}  // namespace mao
