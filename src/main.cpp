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

#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char *argv[])
{


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

 
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    logger->flush_on(spdlog::level::err);
    //spdlog::fl flush_every(std::chrono::seconds(5));

    logger->info("Welcome to spdlog!");
    logger->error("Some error message with arg: {}", 1);
    logger->warn("Easy padding in numbers like {:08d}", 12);
    logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger->info("Support for floats {:03.2f}", 1.23456);
    logger->info("Positional args are {1} {0}..", "too", "supported");
    logger->info("{:<30}", "left aligned");
    logger->set_level(spdlog::level::debug); // Set global log level to debug
    logger->debug("This message should be displayed..");

    // change log pattern
    logger->set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");


    logger->info("Welcome to spdlog!");
    logger->error("Some error message with arg: {}", 1);
    logger->warn("Easy padding in numbers like {:08d}", 12);
    logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger->info("Support for floats {:03.2f}", 1.23456);
    logger->info("Positional args are {1} {0}..", "too", "supported");
    logger->info("{:<30}", "left aligned");
    logger->set_level(spdlog::level::debug); // Set global log level to debug
    logger->debug("This message should be displayed..");

    // 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
    Tray* tary = new Tray();

//	std::cout << "-------------------------" << std::endl << std::endl;


    //qInfo()<<QObject::tr("Program running program end.");

	std::cout << "hello word" << std::endl;
	QApplication::setQuitOnLastWindowClosed(false);

    return QApplication::exec();
}
