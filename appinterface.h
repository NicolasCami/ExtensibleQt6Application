#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

namespace app {

class IApp : public QObject
{
  Q_OBJECT

public:
  IApp() = default;

  virtual QWidget* mainWindow() const = 0;
  virtual QTextEdit* textOutputWidget() const = 0;
};

} // namespace app
