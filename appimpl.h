#pragma once

#include "appinterface.h"

class MainWindow;

namespace app {

class AppImpl : public IApp
{
  Q_OBJECT

public:
  AppImpl(MainWindow* app);

  QWidget* mainWindow() const override;
  QTextEdit* textOutputWidget() const override;

private:
  MainWindow* m_app{ nullptr };
};

} // namespace app
