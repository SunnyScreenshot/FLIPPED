/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.23
 * Description: 统一存放 class Preference 中新建的对象的 objectName
 ******************************************************************/
#ifndef APPELLATION_H
#define APPELLATION_H

#include <QString>

 // perference UI config
//const QString INIT_GENERAL("General");                   // 初始化 常规
//const QString INIT_INTERFACE("Interface");               // 初始化 界面
//const QString INIT_OUTPUT("Output");                     // 初始化 输出
//const QString INIT_PIN("Pin");                           // 初始化 贴图
//const QString INIT_HOTKEYS("Hotkeys");                   // 初始化 快捷键

namespace XC {

//tabGeneral      常规
const QString tgLanuage("Lanuage");
const QString tgSelfStarting("Run onSystem startup");
const QString tgAsAdmin("As administrator");
const QString tgLogLevel("Log level");
const QString tgAutoCheckUpdate("Auto Check");
const QString tgUpdate("Update");
const QString tgReset("tgReset"); // 单独添加 tg, 下同

//tabInterface()  界面
const QString tiBorderStyle("Border Style");
const QString tiBorderColor("Border Color");
const QString tiBorderWidth("Border Width");
const QString tiCrosshairColor("Crosshair Color");
const QString tiCrosshairWidth("Crosshair Width");
const QString tiSmartWindow("Smart window");
const QString tiCrosshair("Ccrosshair");
const QString tiShowCursor("Show cursor");
const QString tiAutoCopyToClipboard("Automatically copy to clipboard");
const QString tiReset("tiReset");

//tabOutput()     输出
const QString toImageQuailty("Image quailty");
const QString toFileName("File Name");
const QString toQuickSavePath("Quick save path");
const QString toAutoSavePath("Auto save path");
const QString toConfigPath("Config path");
const QString toReset("toReset");

//tabPin()        贴图
const QString tpWindowShadow("Window shadow");
const QString tpOpacity("Opacity");
const QString tpMaxSize("Maximum size");
const QString tpReset("tpReset");

//tabHotkeys()    快捷键
//const QString thActiveWindow("Active Window");
//const QString thScrollingWindow("Scrolling Window");
//const QString thDelayCapture("Delay Capture");
//const QString thFullScreen("Full Screen");
//const QString thFixdSize_Region("Fixd_Size Region");
//const QString thPaste("Paste");
//const QString thHide_ShowAllImages("Hide_Show all images");
//const QString thSwitchCurGroup("Switch current group");
const QString thReset("thReset");

//tabAbout()      关于
}

// 对象名、类型、命名的对象名称
#define NEW_OBJECT(_object, _type, _objectName) \
    auto _object = new _type(this); \
    if (_object) \
        _object->setObjectName(_objectName);

// 对象名、类型、命名的对象名称、可能需要显示的文本
#define NEW_OBJECT_AND_TEXT(_object, _type, _objectName, _text) \
    auto _object = new _type(_text, this); \
    if (_object) \
        _object->setObjectName(_objectName);

// 写入配置文件 .ini 的内容
#define WRITE_CONFIG_INI(group, key, value) \
    insSettings->beginGroup(group); \
    insSettings->setValue(key, value); \
    insSettings->endGroup();

// 读取配置文件 .ini 的内容
#define READ_CONFIG_INI(group, key, _value) \
    insSettings->value("/" + group + "/" + key, _value)

// 所有页面的 Reset Btn 都连接到同一个槽函数，内部统一处理
#define CONNECT_RESET_BTN(objectName) \
    auto btn = findChild<QPushButton *>(objectName);\
    if (btn) \
        connect(btn, &QPushButton::released, this, &Preference::onReset);




#endif // APPELLATION_H
