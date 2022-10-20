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
#include <QStyleFactory>
#include "screen/screenshot.h"

// test
//#include <QHotkey>
#include "./screen/tray.h"
#include "tool/pin/pinwidget.h"
#include "tool/testbtstyle.h"
#include "widget/xroundwidget.h"
#include "tool/parameterbar.h"
#include "tool/selectbar.h"
#include <iostream>
#include <QPixmap>

#include "core/xlog.h"
#include "example/exwidget.h"
#include "preference/preference.h"
#include "tool/base/blurwidget.h"
#include <QTranslator>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QGuiApplication>

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
//    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);    // 控制图片缩放质量，svg 的图片不会模糊在 4K 上。   此行无需在 QApplication a(argc,argv);前面
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);  // 上面一行没有生效，但是此行可以生效
#endif

    // 因多处使用 QSettings，故声明组织等信息
    QCoreApplication::setOrganizationName(QStringLiteral("XMuli"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com/XMuli"));
    QCoreApplication::setApplicationName(QStringLiteral(_PROJECT_NAME));
    QCoreApplication::setApplicationVersion(_PROJECT_VERSION);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
//    qApp->setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

    //insSettings->beginGroup(INIT_GENERAL);
    //auto lanuage = insSettings->value("Lanuage", "en_US").toString();
    //insSettings->endGroup();

    //QTranslator* trans = new QTranslator(qApp);
    //QString qmPath(QCoreApplication::applicationDirPath());
    //QString(_COMPILER_ID).compare("MSVC") == 0 ? qmPath += "/../../src/" + lanuage + ".qm" : qmPath += "/../src/" + lanuage + ".qm";
    //qDebug()<<"[*.qm path]:" << qmPath << _COMPILER_ID << "   "<< bool(QString(_COMPILER_ID).compare("MSVC") == 0);
    //trans->load(qmPath); 
    //QCoreApplication::installTranslator(trans);

    // 截图、显示主界面；若点击右上角，则整程序关闭; 如同执行了 close、destroy 一类函数
    Tray::instance();


    //auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    //if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    //QDir d{ path };
    //if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath())) {
    //    qDebug() << "settings in" << QDir::currentPath();
    //    QFile f{ "settings.txt" };
    //    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    //        f.write("Hello, World");
    //}

    //XFrameWidget *m_pw1 = new XFrameWidget();
    ////m_pw1->setPixmap(QPixmap("../../p2.jpg"));
    //m_pw1->setPixmap(QPixmap("D:/projects/PicShot/src/p2.jpg"));
    //m_pw1->setFixedSize(400, 400);
    //m_pw1->show();

    //Preference* pre = new Preference(nullptr);
    //pre->show();

    QStringList listStyle = QStyleFactory::keys();
    foreach(QString val, listStyle)
      qDebug()<<val<<"  ";

#ifdef Q_OS_MAC
    const QString value = "Fusion";  // macintosh Windows Fusion
    QApplication::setStyle(value);
#else
#endif

//    auto t = new TestBTStyle();
//    t->setWindowTitle(QString("MacOS 12.6 + Qt 5.15.2 + Style: %1").arg(value));
//    t->show();

//    WinSetting* set = new WinSetting();
//    set->show();
//    auto t0 = new QWidget();
//    t0->resize(800, 400);
//    t0->show();

//    SelectBar* t1 = new SelectBar(Qt::Horizontal, t0);
//    t1->setWindowFlags(Qt::FramelessWindowHint);
////    t1 ->setBlurBackground(QPixmap("D:/projects/PicShot/src/p1.jpg"), 4);
//    t1->move(200, 100);
//    t1->show();

//    ParameterBar* t2 = new ParameterBar(Qt::Horizontal, t0);
////    t2->setBlurBackground(QPixmap("D:/projects/PicShot/src/p2.jpg"), 5);
//    t2->move(200, 200);
//    t2->show();

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

//    PinWidget* w5 = new PinWidget(QPixmap("C:/Users/xmuli/Desktop/Snipaste_2022-07-16_02-13-39.png"), QRect(100, 100, 400, 400));
//    w5->show();

//    QMouseEvent* evMouse = dynamic_cast<QMouseEvent*>(ev);
//    cout << "Posicion: " << evMouse->x() << ", " << evMouse->y() << endl;

//    QDesktopWidget* desktop = qApp->desktop();  // 获取桌面的窗体对象

//    // 获取屏幕类
//    QScreen *pScreen = QGuiApplication::primaryScreen();
//    // 获取第一个屏幕的图片
//    QPixmap mPixmap = pScreen->grabWindow(0, 0, 0, desktop->size().width(), desktop->size().height());
//    // 设置名称为当前时间


    return QApplication::exec();
}
