#include "pythonmoduleredirect.h"

#include "pythoninclude.h"
#include "pythonutils.h"

#include <stdexcept>

namespace {

static bool redirecting = false;

static std::function<void(const std::string&)> stdOut;
static std::function<void(const std::string&)> stdErr;
static std::function<void()> stdFlush;

static const char* MODULE_NAME = "_redirect";

PyObject*
stdredir_stdout(PyObject* self, PyObject* args);
PyObject*
stdredir_stderr(PyObject* self, PyObject* args);
PyObject*
stdredir_stdflush(PyObject* self, PyObject* args);

static PyMethodDef MethodDef[] = {
  { "stdout", stdredir_stdout, METH_VARARGS, "." },
  { "stderr", stdredir_stderr, METH_VARARGS, "." },
  { "stdflush", stdredir_stdflush, METH_NOARGS, "." },
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
stdredir_stdout(PyObject* self, PyObject* args)
{
  char* string;
  if (!PyArg_ParseTuple(args, "s", &string))
    return 0;
  if (stdOut)
    stdOut(string);
  return Py_BuildValue("");
}

PyObject*
stdredir_stderr(PyObject* self, PyObject* args)
{
  char* string;
  if (!PyArg_ParseTuple(args, "s", &string))
    return 0;
  if (stdErr)
    stdErr(string);
  return Py_BuildValue("");
}

PyObject*
stdredir_stdflush(PyObject* self, PyObject* Py_UNUSED(args))
{
  if (stdFlush)
    stdFlush();
  return Py_BuildValue("");
}

static bool imported = false;

static std::string code = R"###(
import _redirect
import sys

class StdOut:
    def write(self, s):
        _redirect.stdout(s)
    def flush(self):
        _redirect.stdflush()

class StdErr:
    def write(self, s):
        _redirect.stderr(s)
    def flush(self):
        _redirect.stdflush()

def redirect():
    sys.stdout = StdOut()
    sys.stderr = StdErr()
    sys.stdin  = None
)###";

} // namespace

namespace py {
namespace redirect {

void
importModule()
{
  if (imported)
    throw std::runtime_error("Python redirect module already imported");

  int ret = PyImport_AppendInittab(MODULE_NAME, PyInit);

  if (ret < 0)
    throw std::runtime_error("Failed to import redirect module");

  imported = true;
}

void
start(std::function<void(const std::string&)> out,
      std::function<void(const std::string&)> err,
      std::function<void()> flush)
{
  if (redirecting)
    throw std::runtime_error("Already redirecting standard streams");

  stdOut = out;
  stdErr = err;
  stdFlush = flush;

  int ret = py::runString(code.c_str());

  if (ret < 0)
    throw std::runtime_error("Failed to redirect standard streams");

  ret = py::runString("redirect()");

  if (ret < 0)
    throw std::runtime_error("Failed to redirect standard streams");

  redirecting = true;
}

void
stop()
{
  if (!redirecting)
    throw std::runtime_error("Not redirecting standard streams");

  redirecting = false;

  stdOut = {};
  stdErr = {};
  stdFlush = {};
}

} // namespace redirect
} // namespace py
