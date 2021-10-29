#pragma once

#include "logevent.hpp"

#include <iomanip>
#include <list>
#include <map>
#include <vector>

namespace mao {

/**
 * 日志格式器
 * 用于将日志事件格式化成字符串
 */
class LogFormat {
public:
  using ptr = std::shared_ptr<LogFormat>;

  // 是否展示
  class Show {
  public:
    static const bool NO = false;
    static const bool YES = true;

  private:
    Show() = delete;
  };

  /** @deprecate 废弃方案 **/
  enum class _Show { NO = false, YES = true };

  // 格式化方案
  struct Scheme {
  public:
    bool is_show{Show::YES};  // 是否展示
    std::string left_str{};   // 左包裹字符
    std::string right_str{};  // 右包裹字符
  };

protected:
  using Schemes = std::unordered_map<Attribute, Scheme>;

public:
  /**
   * format
   * @param event 需要进行格式化的事件
   * @return 返回格式化后端字符串
   */
  virtual std::string format(const LogEvent::ptr event_ptr) const {
    std::string str;
    for (const auto& att : m_seq_list) {
      const auto& iter{m_schemes.find(att)};
      if (iter == m_schemes.end() || !iter->second.is_show) {
        break;
      }

      str += iter->second.left_str;
      switch (iter->first) {
        case Attribute::LEVEL:
          str += make_ToStr_ptr((event_ptr->getLevel()))->getStr();
          break;

        case Attribute::THREAD_ID:
          str += make_ToStr_ptr(event_ptr->getThreadid())->getStr();
          break;

        case Attribute::TIME:
          str += make_ToStr_ptr(
                     std::put_time(std::localtime(&event_ptr->getTime()),
                                   "%x, %X"))
                     ->getStr();
          break;

        case Attribute::FIME_NAME:
          str += event_ptr->getFileName();
          break;

        case Attribute::LINE:
          str += make_ToStr_ptr(event_ptr->getline())->getStr();
          break;

        case Attribute::CONTENT:
          str += event_ptr->getContent();
          break;
      }
      str += iter->second.right_str;
    }
    return str;
  }
  virtual std::string format(const LogEvent& event) const {
    return format(std::make_shared<LogEvent>(event));
  }
  /** 通过属性获取对应的样式 **/
  virtual const Scheme getScheme(const Attribute& att) const {
    auto iter{m_schemes.find(att)};
    if (iter != m_schemes.end())
      return iter->second;
    return Scheme{};
  }

  /** 设置新样式或者修改旧样式 **/
  virtual void setScheme(const Attribute& att, const Scheme& new_scheme) {
    m_schemes[att] = new_scheme;
  }

  virtual ~LogFormat() = default;

protected:
  // 由事件属性与其对应的样式组成
  Schemes m_schemes{
      {Attribute::CONTENT, Scheme{Show::YES, "\"", "\"\n"}},  // 内容
      {Attribute::FIME_NAME, Scheme{Show::YES, "", ","}},     // 文件名
      {Attribute::LEVEL, Scheme{Show::YES, "[", "]"}},        // 事件等级
      {Attribute::LINE, Scheme{Show::YES, "", ", "}},         // 行号

      {Attribute::THREAD_ID, Scheme{Show::YES, "[tharedid: ", "]"}},  // 线程id

      {Attribute::TIME, Scheme{Show::YES, "[", "]: "}},       // 时间戳
      {Attribute::LOGGER_NAME, Scheme{Show::YES, "[", "] "}}  // 日志器名称
  };

  // 格式化顺序表，用于控制格式化时的顺序
  std::list<Attribute> m_seq_list{Attribute::LEVEL, Attribute::THREAD_ID,
                                  Attribute::TIME,  Attribute::FIME_NAME,
                                  Attribute::LINE,  Attribute::CONTENT};
};

/* =================================================================
// 有色格式化器: 格式化时添加配色
class CLogFormat : public LogFormat {
public:
  // 配色
  enum class Style {
    RESET = 0,  // 重置
    BOLD,       // 加粗/高亮
    DIM,        // 暗淡
    UNDERLINE,  // 下划线
    TWINKLE,    // 闪烁
    REVERSAL,   // 反转

    // 前景色
    F_BLACK = 30,  // 黑色
    F_RED,         // 红色
    F_GREEN,       // 绿色
    F_YELLOW,      // 黄色
    F_BLUE,        // 蓝色
    F_FUCHSION,    // 品红
    F_CYAN,        // 青色
    F_WHITE        // 白色
  };
  // 颜色方案
  class CScheme {
  public:
    std::vector<Style> content_colour{Style::RESET};  // 内容的配色方案
    std::vector<Style> left_str_colour{Style::RESET};  // 左包裹字符的配色方案
    std::vector<Style> right_str_colour{Style::RESET};  // 右包裹字符的配色方案
  };

  // 与format相比添加了配色信息
  virtual std::string format(const LogEvent::ptr event) const;

  virtual ~CLogFormat() = default;

protected:
  using AllCSchemes = std::unordered_map<Attribute, CScheme>;

  // 默认初始化
  AllCSchemes m_cschemes{
      {Attribute::CONTENT, CScheme{}},    // 内容
      {Attribute::FIME_NAME, CScheme{}},  // 文件名
      {Attribute::LEVEL, CScheme{}},      // 事件等级
      {Attribute::LINE, CScheme{}},       // 行号
      {Attribute::THREAD_ID, CScheme{}},  // 线程id
      {Attribute::TIME, CScheme{}}        // 时间戳
  };
};
=============================================================== */

}  // namespace mao
