#include "logformat.h"

#include <sstream>

namespace mao {

// 参数event：需要格式化的事件
std::string LogFormat::format(LogEvent::ptr event) const {
  std::stringstream ss{};
  ss << std::make_pair(getScheme(schemeid::LEVEL), event->getLevel())
     << std::make_pair(getScheme(schemeid::THREAD_ID), event->get)
}

// 通过id获取对应的格式化方案
virtual const Scheme& getScheme(const schemeid& id) const;
virtual Scheme& getScheme(const schemeid& id);

// 设置对应方案，通过id修改对应格式化方案
virtual void setScheme(const schemeid& id, const Scheme& scheme);

}  // namespace mao
