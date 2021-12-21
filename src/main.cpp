#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include "winscreen/screenshot.h"
#include "winscreen/winmain.h"
#include "logger.h"
#include "logmanager.h"
#include "propertyconfigurator.h"

// test
#include <QHotkey>
#include "./winscreen/winresetbtn.h"
#include "./winscreen/tray.h"
#include "./syswininfo/syswininfo_win.h"
#include "widget/xroundwidget.h"

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
//    getAllTopWinRect();

    // TODO 2021-10-08 高分屏适配的许多尝试: QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    //a.loadTranslator(QList<QLocale>() << QLocale::system());
    //a.setOrganizationName("XMuli");
    //a.setApplicationName("PicShot");
    //a.setApplicationVersion("0.1");
    //a.setQuitOnLastWindowClosed(false);
    //a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    /* 枚举窗口的所有进程 */
    //获取屏幕上所有的顶层窗口,每发现一个窗口就调用回调函数一次

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
