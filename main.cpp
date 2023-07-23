#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "pythoninstance.h"
#include "appimpl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "extensible-qt6-app-boilerplate_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    app::AppImpl appInterface(&w);
    py::Instance pythonInstance(&appInterface);

    w.show();
    int exitCode = a.exec();

    return exitCode;
}
