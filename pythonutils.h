#pragma once

#include <string>

class QString;

namespace py {

int
runString(const QString& command);
int
runString(const std::string& command);

} // namespace py
