/*******************************************************************
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
#include "./screen/tray.h"
#include "widget/xroundwidget.h"
#include "tool/parameterbar.h"
#include "tool/selectbar.h"
#include <iostream>

#include "core/xlog.h"
#include "example/exwidget.h"
#include "preference/preference.h"

// test
#ifdef Q_OS_WIN
    //#include "./platform/wininfo_win.h"
#elif  defined(Q_OS_MAC)
#elif  defined(Q_OS_LINUX)
    #include "./platform/wininfo_x11.h"
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

    // 因多处使用 QSettings，故声明组织等信息
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/XMuli"));
    QCoreApplication::setApplicationName(QStringLiteral(_PROJECT_NAME));
    QCoreApplication::setApplicationVersion(_PROJECT_VERSION);

    QApplication a(argc, argv);
    //a.loadTranslator(QList<QLocale>() << QLocale::system());
    a.setQuitOnLastWindowClosed(false);
//    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

    // 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
    Tray::instance();

    //Preference* pre = new Preference(nullptr);
    //pre->show();


//    WinSetting* set = new WinSetting();
//    set->show();
    //SelectBar* t1 = new SelectBar();
    //t1->move(200, 100);
    //t1->show();

    //ParameterBar* t2 = new ParameterBar();
    //t2->move(200, 200);
    //t2->show();

//    ColorParaBar* t3 = new ColorParaBar(ColorParaBarMode::CPB_ParaBar, Qt::Horizontal);
////    t3->resize(400, 300);
//    t3->move(200, 300);
//    t3->show();
//
//    ColorParaBar* t3_1 = new ColorParaBar(ColorParaBarMode::CPB_HighLight, Qt::Horizontal);
//    //    t3->resize(400, 300);
//    t3_1->move(200, 400);
//    t3_1->show();
//
//    ExWidget* t4 = new ExWidget();
//    //    t3->resize(400, 300);
//    t4->move(100, 200);
//    t4->show();


    return QApplication::exec();
}
