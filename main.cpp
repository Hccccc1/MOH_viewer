//#include "MOH_Viewer/moh_viewer.h"
#include "LoginInterface/logininterface.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MOH_Viewer w;
    LoginInterface w;
    w.show();
    return a.exec();
}
