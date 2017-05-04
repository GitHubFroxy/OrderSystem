#include <QApplication>
#include "widget.h"
#include "myhelper.h"
#include "database.h"
#include "loginwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myHelper::SetStyle("blue");
    myHelper::SetUTF8Code();

    if (!createConnection())
    {
        myHelper::ShowMessageBoxError(QString::fromLocal8Bit("数据库打开失败!"));
        return 0;
    }

    LoginWidget login;
    login.show();

//    Widget w;
//    w.resize(900,650);
//    w.show();
    
    return a.exec();
}
