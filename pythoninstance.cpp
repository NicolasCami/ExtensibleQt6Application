#include "pythoninstance.h"

#include "pythoninclude.h"
#include "pythonmoduleapp.h"
#include "pythonmoduleredirect.h"

#include "appinterface.h"

#include <iostream>

#include <QDebug>
#include <QTextEdit>

namespace py {

Instance::Instance(::app::IApp* app)
  : m_appInterface(app)
{
  try {
    py::redirect::importModule();
    py::app::importModule();

    Py_Initialize();

    py::redirect::start(
      [=](const std::string& str) {
        std::cout << str << std::flush;
        app->textOutputWidget()->insertPlainText(QString::fromStdString(str));
      },
      [=](const std::string& str) {
        std::cout << str << std::flush;
        app->textOutputWidget()->insertPlainText(QString::fromStdString(str));
      },
      [=]() { std::cout << std::flush; });
    py::app::start(app);
  } catch (std::runtime_error& e) {
    qWarning() << "Failed to initialize Python interpreter:" << e.what();
  }
}

Instance::~Instance()
{
  Py_Finalize();
}

} // namespace py
