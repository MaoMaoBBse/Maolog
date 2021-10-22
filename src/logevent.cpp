#include "logevent.h"

namespace mao {

// 将日志级别转换成字符串
std::string logLeveltoStr(const LogLevel& level) {
  // 日志级别与字符串组成的散列表
  static std::unordered_map<LogLevel, std::string> m_log_level_str_list = {
      {LogLevel::DEBUG, std::string{"debug"}},
      {LogLevel::INFO, std::string{"info"}},
      {LogLevel::WARN, std::string{"warn"}},
      {LogLevel::ERROR, std::string{"error"}},
      {LogLevel::FATAL, std::string{"fatal"}}};

  return m_log_level_str_list.find(level)->second;
}

// 设置行号
void LogEvent::setLine(const uint32_t& line) {
  m_line = line;
}
// 设置日志等级
void LogEvent::setLogLevel(const LogLevel& level) {
  m_level = level;
}

// 设置文件名
void LogEvent::setFileName(const std::string& file_name) {
  m_file = file_name;
}

// 获取文件名
const std::string& LogEvent::getFileName() const noexcept {
  return m_file;
}

// 获取日志等级
const LogLevel& LogEvent::getLevel() const noexcept {
  return m_level;
}

// 获取行号
const uint32_t& LogEvent::getline() const noexcept {
  return m_line;
}

// 获取时间戳
const std::time_t& LogEvent::getTime() const noexcept {
  return m_time;
}

// 获取日志内容
const std::string& LogEvent::getContent() const noexcept {
  return m_content;
}

}  // namespace mao