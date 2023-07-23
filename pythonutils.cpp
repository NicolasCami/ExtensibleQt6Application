#include "pythonutils.h"

#include "pythoninclude.h"

#include <QString>

namespace py {

int runString(const QString& command)
{
    return runString(command.toStdString());
}

int runString(const std::string& command)
{
    // See https://docs.python.org/3/c-api/init.html#non-python-created-threads
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    int ret = PyRun_SimpleString(command.c_str());

    PyGILState_Release(gstate);

    return ret;
}

}
