#include "pythonmoduleapp.h"

#include "apputils.h"
#include "pythoninclude.h"
#include "pythonutils.h"

#include "appinterface.h"

#include <stdexcept>

namespace {

static const char* MODULE_NAME = "application_api";

PyObject*
py_dummyFunction(PyObject* self, PyObject* args);

static PyMethodDef MethodDef[] = {
  { "dummyFunction", py_dummyFunction, METH_VARARGS, "." },
  { NULL, NULL, 0, NULL }
};

static PyModuleDef ModuleDef = { PyModuleDef_HEAD_INIT,
                                 MODULE_NAME,
                                 NULL,
                                 -1,
                                 MethodDef,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL };

static PyObject*
PyInit()
{
  PyObject* m;

  m = PyModule_Create(&ModuleDef);
  if (m == NULL)
    return NULL;

  return m;
}

PyObject*
py_dummyFunction(PyObject* self, PyObject* args)
{
  int param;
  if (!PyArg_ParseTuple(args, "i", &param))
    return 0;
  int ret = app::utils::dummyFunction(param);
  return Py_BuildValue("i", ret);
}

static bool imported = false;
static bool started = false;

static std::string code = R"###(
#import sys
#sys.path.append('.../extensible-qt6-app-boilerplate/build/BoilerplateQtApp')

# initialize 'iface' object
main_window = None

def initApplicationInterface(pointer):
    from PyQt6.QtWidgets import QMainWindow
    from PyQt6.sip import wrapinstance
    #import BoilerplateQtApp

    global main_window
    main_window = wrapinstance(pointer, QMainWindow)

)###";

} // namespace

namespace py {
namespace app {

void
importModule()
{
  if (imported)
    throw std::runtime_error("Python app module already imported");

  int ret = PyImport_AppendInittab(MODULE_NAME, PyInit);

  if (ret < 0)
    throw std::runtime_error("Failed to import app module");

  imported = true;
}

void
start(::app::IApp* app)
{
  if (app == nullptr)
    throw std::runtime_error(
      "Python app module requires an application interface");

  if (started)
    throw std::runtime_error("Python app module already started");

  int ret = py::runString(code);

  if (ret < 0)
    throw std::runtime_error(
      "Exception raised during initialization of Python app module");

  ret = py::runString(QStringLiteral("initApplicationInterface(%1)")
                        .arg(reinterpret_cast<quint64>(app->mainWindow())));

  if (ret < 0)
    throw std::runtime_error(
      "Failed to initialize application interface of Python app module");

  started = true;
}

} // namespace app
} // namespace py
