#include "schemeeditormainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SchemeEditorMainWindow w;
    w.show();

    return a.exec();
}
