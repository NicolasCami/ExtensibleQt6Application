#include "appimpl.h"

#include "mainwindow.h"

namespace app {

AppImpl::AppImpl(MainWindow* app)
  : m_app(app)
{
}

QWidget*
AppImpl::mainWindow() const
{
  return m_app;
}

} // namespace app
