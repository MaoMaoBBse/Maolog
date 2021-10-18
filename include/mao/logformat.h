#pragma once

#include "logevent.h"

#include <map>
#include <ostream>
#include <unordered_map>
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
  enum class Show { NO = 0, YES };

  // 格式化方案
  struct Scheme {
  public:
    Show is_show{Show::YES};  // 是否展示
    std::string left_str{};   // 左包裹字符
    std::string right_str{};  // 右包裹字符
  };

  // 参数event：需要格式化的事件
  virtual std::string format(LogEvent::ptr event) const;

  // 通过属性获取对应的格式化方案
  virtual const Scheme& getScheme(const Attribute& att) const;
  virtual Scheme& getScheme(const Attribute& att);

  // 设置对应方案，通过属性修改对应格式化方案
  virtual void setScheme(const Attribute& att, const Scheme& scheme);

  virtual ~LogFormat() = default;

protected:
  using AllSchemes = std::unordered_map<Attribute, Scheme>;

  template <typename T>
  friend std::ostream& operator<<(std::ostream& os,
                                  const std::pair<Scheme, T>& pair) {
    os << pair.first.left_str << pair.second << pair.first.right_str;
    return os;
  }

  // 默认初始化
  AllSchemes m_all_schemes{
      {Attribute::CONTENT, Scheme{}},                   // 内容
      {Attribute::FIME_NAME, Scheme{}},                 // 文件名
      {Attribute::LEVEL, Scheme{Show::YES, "[", "]"}},  // 事件等级
      {Attribute::LINE, Scheme{Show::YES, ": "}},       // 行号
      {Attribute::THREAD_ID, Scheme{}},                 // 线程id
      {Attribute::TIME, Scheme{}}                       // 时间戳
  };
};

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
  virtual std::string cformat(LogEvent::ptr event) const;

  // 通过属性获取对应的格式化方案
  virtual const CScheme& getCScheme(const Attribute& att) const;
  virtual CScheme& getCScheme(const Attribute& att);

  // 设置对应方案，通过属性修改对应配色格式化方案
  virtual void setCScheme(const Attribute& att, const CScheme& scheme);

  virtual ~CLogFormat() = default;

protected:
  using AllCSchemes = std::unordered_map<Attribute, CScheme>;

  // 默认初始化
  AllCSchemes m_all_cschemes{
      {Attribute::CONTENT, CScheme{}},    // 内容
      {Attribute::FIME_NAME, CScheme{}},  // 文件名
      {Attribute::LEVEL, CScheme{}},      // 事件等级
      {Attribute::LINE, CScheme{}},       // 行号
      {Attribute::THREAD_ID, CScheme{}},  // 线程id
      {Attribute::TIME, CScheme{}}        // 时间戳
  };
};

}  // namespace mao
