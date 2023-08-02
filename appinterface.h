#pragma once

#include <QObject>

namespace app {

class IApp : public QObject
{
  Q_OBJECT

public:
  IApp() = default;

  virtual QWidget* mainWindow() const = 0;
};

} // namespace app
