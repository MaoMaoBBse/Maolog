#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

namespace mao {
// 日志级别
enum class LogLevel { DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, FATAL = 5 };

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

// 重载日志等级的输出流操作
std::ostream& operator<<(std::ostream& os, const LogLevel& loglevel) {
  return os << logLeveltoStr(loglevel);
}

// 各种属性
enum class Attribute {
  FIME_NAME = 1,  // 文件名
  LEVEL,          // 等级
  LINE,           // 行号
  THREAD_ID,      // 线程id
  TIME,           // 时间戳
  CONTENT,        // 内容
  LOGGER_NAME     // 日志器名称
};

/** toStr 可以将任意参数转为为str的模板类 **/
template <typename... Args>
class ToStr;
template <>
class ToStr<> {
public:
  using ptr = std::shared_ptr<ToStr<>>;
  std::string getStr() { return std::string{}; }

protected:
  std::stringstream& getSS() {
    static std::stringstream ss;
    return ss;
  }
};

template <typename Head, typename... Args>
class ToStr<Head, Args...> : public ToStr<Args...> {
public:
  using ptr = std::shared_ptr<ToStr<Head, Args...>>;
  ToStr(Head head, Args... args) : ToStr<Args...>(args...) {
    auto& ss{ToStr<>::getSS()};
    ss << head;
    m_str = ss.str();
    ss.str(std::string{});
  }
  std::string getStr() { return m_str += ToStr<Args...>::getStr(); }

private:
  std::string m_str;
};

// 方便获取ToStr对象的函数
template <typename... Args>
auto make_ToStr_ptr(Args... args) {
  return std::make_shared<ToStr<Args...>>(args...);
}

/** ========================================================== **/

// 日志事件
class LogEvent {
public:
  using ptr = std::shared_ptr<LogEvent>;

  /** ====================== 初始化 ====================== **/
  LogEvent() = default;

  template <typename... Contents>
  LogEvent(const LogLevel& level,
           const std::string& file_name,
           const uint32_t& line,
           const Contents&... contents) {
    init(level, file_name, line, contents...);
  }

  template <typename... Contents>
  inline void init(const LogLevel& level,
                   const std::string& file_name,
                   const uint32_t& line,
                   const std::thread::id& thread_id,
                   const std::time_t& time,
                   const Contents&... contents) {
    m_level = level;
    m_file = file_name;
    m_line = line;
    m_threadid = thread_id;
    m_time = time;
    m_content = toStr(contents...);
  }

  template <typename... Contents>
  inline void init(const LogLevel& level,
                   const std::string& file_name,
                   const uint32_t& line,
                   const Contents&... contents) {
    m_level = level;
    m_file = file_name;
    m_line = line;
    m_threadid = std::this_thread::get_id();
    m_time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    m_content = make_ToStr_ptr(contents...)->getStr();
  }
  /** =================================================== **/

  // 设置行号
  inline void setLine(const uint32_t& line) { m_line = line; }

  // 设置日志等级
  inline void setLogLevel(const LogLevel& level) { m_level = level; }

  // 设置文件名
  inline void setFileName(const std::string& file_name) { m_file = file_name; }

  // 设置日志内容
  template <typename... Args>
  void setContent(const Args&... args) {
    m_content = ToStr(args...);
  }

  // 获取文件名
  inline const std::string& getFileName() const noexcept { return m_file; }

  // 获取日志等级
  inline const LogLevel& getLevel() const noexcept { return m_level; }

  // 获取行号
  inline const uint32_t& getline() const noexcept { return m_line; }

  // 获取时间戳
  inline const std::time_t& getTime() const noexcept { return m_time; }

  // 获取日志内容
  inline const std::string& getContent() const noexcept { return m_content; }

  // 获取线程id
  inline const std::thread::id& getThreadid() const noexcept {
    return m_threadid;
  }

  virtual ~LogEvent() = default;

private:
  std::string m_file{};                                    // 文件名
  LogLevel m_level{LogLevel::DEBUG};                       // 事件等级
  uint32_t m_line{};                                       // 行号
  std::thread::id m_threadid{std::this_thread::get_id()};  // 线程id
  std::string m_content{};                                 // 内容

  std::time_t m_time{std::chrono::system_clock::to_time_t(
      std::chrono::system_clock::now())};  // 时间戳
};

}  // namespace mao
