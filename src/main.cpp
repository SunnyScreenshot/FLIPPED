#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include "WinScreen/winfullscreen.h"
#include "logger.h"
#include "logmanager.h"
#include "propertyconfigurator.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //配置文件路径
    Log4Qt::PropertyConfigurator::configure(a.applicationDirPath() + "/../log4qt.conf");
    Log4Qt::LogManager::setHandleQtMessages(true);

	WinFullScreen winFullScreen;
    winFullScreen.display();

	winFullScreen.getBasePixmap();
	//winFullScreen.showFullScreen(); //只顶级窗口有效，且不是双屏全屏
    winFullScreen.show();

    return QApplication::exec();
}
