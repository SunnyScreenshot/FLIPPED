#include <QApplication>
#include <QPushButton>
#include "SysScreen/sysscreen.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();

    SysScreen obj;
    obj.display();

    return QApplication::exec();
}
