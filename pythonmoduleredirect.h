#pragma once

#include <functional>
#include <string>

namespace py {
namespace redirect {

void
importModule();
void
start(std::function<void(const std::string&)> out,
      std::function<void(const std::string&)> err,
      std::function<void()> flush);

} // namespace redirect
} // namespace py
