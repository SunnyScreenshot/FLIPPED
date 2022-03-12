#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include <QObject>
#include "screen/screenshot.h"
#include "screen/winsetting.h"

// test
//#include <QHotkey>
#include "./screen/winresetbtn.h"
#include "./screen/tray.h"
#include "widget/xroundwidget.h"

//#include "./wininfo/wininfo_win.h"
#include <iostream>

#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
	
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");

    // getAllTopWinRect();

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


 //   qInfo()<<QObject::tr("Program running program start.");
 
//	setlocale(LC_CTYPE, "");
//	WinInfoWin info;
//	std::vector<WinInfo> vec;
//	info.getAllWinInfo(vec);

    // 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
    Tray* tary = new Tray();

//	std::cout << "-------------------------" << std::endl << std::endl;


    //qInfo()<<QObject::tr("Program running program end.");

	std::cout << "hello word" << std::endl;
	QApplication::setQuitOnLastWindowClosed(false);

    return QApplication::exec();
}
