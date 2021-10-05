#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include "WinScreen/winfullscreen.h"
#include "logger.h"
#include "logmanager.h"
#include "propertyconfigurator.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#if  0
    //配置文件路径（日志调试）
//    Log4Qt::PropertyConfigurator::configure(a.applicationDirPath() + "/../log4qt.conf");
//    Log4Qt::LogManager::setHandleQtMessages(true);
#else
    // 改变缺省消息处理程序的输出（终端调试）
//    qSetMessagePattern("%{appname} %{type} %{time [yyyy-MM-dd hh:mm:ss]} %{pid} %{threadid} %{qthreadptr} (%{file} %{line}) %{function} %{message}");
    qSetMessagePattern("%{type} %{time [yyyy-MM-dd hh:mm:ss]} (%{file} %{line}) %{function} %{message}");
#endif

    qInfo()<<QObject::tr("Program running program start.");

	WinFullScreen winFullScreen;
    winFullScreen.display();

	winFullScreen.getBasePixmap();
	//winFullScreen.showFullScreen(); //只顶级窗口有效，且不是双屏全屏
    winFullScreen.show();

    qInfo()<<QObject::tr("Program running program end.");

    return QApplication::exec();
}
