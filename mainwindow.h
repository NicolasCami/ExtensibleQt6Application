#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  QTextEdit* textOutputWidget() const;

private Q_SLOTS:
  void on_pushButton_clicked();

private:
  Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
