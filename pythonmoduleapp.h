#pragma once

namespace app {
class IApp;
}

namespace py {
namespace app {

void
importModule();
void
start(::app::IApp* app);

} // namespace app
} // namespace py
