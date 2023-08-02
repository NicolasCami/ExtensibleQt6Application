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

QTextEdit*
AppImpl::textOutputWidget() const
{
  return m_app->textOutputWidget();
}

} // namespace app
