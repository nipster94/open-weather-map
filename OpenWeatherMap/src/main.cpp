#include "includes/owmgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OWMGUI owmGUI(argc,argv);
    owmGUI.show();
    owmGUI.execute();

    return a.exec();
}
