﻿/*******************************************************************
   * Copyright (c) 2021-2022 偕臧  All rights reserved.
   *
   * Author: XMuli <xmulitech@gmail.com>
   * GitHub: https://github.com/XMuli
   * Blog:   https://xmuli.tech
   *
   * Date:   2021.09.15
   * Description: 
   ******************************************************************/
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

#include <iostream>

#include "core/xlog/xlog.h"

// test
#ifdef Q_OS_WIN
    //#include "./wininfo/wininfo_win.h"
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
    #include "./wininfo/wininfo_x11.h"
#endif

int main(int argc, char *argv[])
{

    // TODO 2022-06-18 高分屏适配的许多尝试:
    // 【方案一】 Qt 5.14+, 解决 1.5会缩放到2倍，不过显示会有问题，比如按钮之间时不时会有虚线。
    //    qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
    //    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy);

    // 【方案二】 Qt5.12 替代方案
    //    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1.5");

//    // 【方案三】 Qt5.12 替代方案
//#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
//    qputenv("QT_SCALE_FACTOR", "1.0"); // 全局缩放因子 https://blog.csdn.net/u014410266/article/details/107488789
//    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif

    // 【方案四】 使用 windows 自带的
    // exe 统计目录下使用 qt.conf 文件


    // 三种方案 https://blog.csdn.net/hanjiang08/article/details/124653265
    // https://blog.csdn.net/qq_18260845/article/details/103861201
    // https://blog.csdn.net/feiyangqingyun/article/details/124860909


#if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
//    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  // 2K、4K 2@ 倍；获取的分辨率 4K 下实际为 /2 后。 此行需在 QApplication a(argc,argv);前面
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);    // 控制图片缩放质量，svg 的图片不会模糊在 4K 上。                此行无需在 QApplication a(argc,argv);前面
#endif

    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setApplicationName(QStringLiteral("PicShot"));
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));

    QApplication a(argc, argv);
    //a.loadTranslator(QList<QLocale>() << QLocale::system());
    a.setQuitOnLastWindowClosed(false);
//    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

    // 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
    Tray::instance();
    return QApplication::exec();
}
