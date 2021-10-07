#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include "WinScreen/winfullscreen.h"
#include "WinScreen/winmain.h"
#include "logger.h"
#include "logmanager.h"
#include "propertyconfigurator.h"

// test
#include <QHotkey>
#include "WinScreen/winresetbtn.h"
#include "WinScreen/tray.h"


int main(int argc, char *argv[]) {

	// TODO 2021-10-08 高分屏适配的许多尝试: QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
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

 //   qInfo()<<QObject::tr("Program running program start.");
 
	// 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
	Tray* tary = new Tray();

    //qInfo()<<QObject::tr("Program running program end.");

    return QApplication::exec();
}
