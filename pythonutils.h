#pragma once

#include <string>

class QString;
class MainWindow;

namespace py {

int runString( const QString &command );
int runString( const std::string &command );

}
