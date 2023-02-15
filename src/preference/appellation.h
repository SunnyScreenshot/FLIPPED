﻿// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#ifndef APPELLATION_H
#define APPELLATION_H

#include <QString>

// Description: 统一存放 class Preference 中新建的对象的 objectName
 // perference UI config
//const QString INIT_GENERAL("General");                   // 初始化 常规
//const QString INIT_INTERFACE("Interface");               // 初始化 界面
//const QString INIT_OUTPUT("Output");                     // 初始化 输出
//const QString INIT_PIN("Pin");                           // 初始化 贴图
//const QString INIT_HOTKEYS("Hotkeys");                   // 初始化 快捷键

namespace XC {

//tabGeneral      常规
const QString tgLanuage("lanuage");
const QString tgFont("font");
const QString tgSelfStarting("run_on_ystem_startup");
const QString tgAsAdmin("as_administrator");
const QString tgLogLevel("log_level");
const QString tgAutoCheckUpdate("auto_check");
const QString tgUpdate("update");
const QString tgReset("tg_reset"); // 单独添加 tg, 下同

//tabInterface()  界面
const QString tiBorderStyle("border_style");
const QString tiBorderColor("border_color");
const QString tiBorderWidth("border_width");
const QString tiCrosshairColor("crosshair_color");
const QString tiCrosshairWidth("crosshair_width");
const QString tiSmartWindow("smart_window");
const QString tiCrosshair("crosshair");
const QString tiShowCursor("show_cursor");
const QString tiAutoCopyToClipboard("auto_copy_to_clipboard");
const QString tiReset("ti_reset");

//tabOutput()     输出
const QString toImageQuailty("image_quailty");
const QString toFileName("file_name");
const QString toQuickSavePath("quick_save_path");
const QString toAutoSavePath("auto_save_path");
const QString toConfigPath("config_path");
const QString toReset("to_reset");

//tabPin()        贴图
const QString tpWindowShadow("window_shadow");
const QString tpOpacity("opacity");
const QString tpMaxSize("maximum_size");
const QString tpReset("tp_reset");

//tabHotkeys()    快捷键
//const QString thActiveWindow("active_window");
//const QString thScrollingWindow("scrolling_window");
//const QString thDelayCapture("delay_capture");
//const QString thFullScreen("full_screen");
//const QString thFixdSize_Region("fixd_size_region");
//const QString thPaste("paste");
//const QString thHide_ShowAllImages("hide_show_all_images");
//const QString thSwitchCurGroup("switch_current_group");
const QString thReset("th_reset");

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
    settingIni->beginGroup(group); \
    settingIni->setValue(key, value); \
    settingIni->endGroup();

// 读取配置文件 .ini 的内容
#define READ_CONFIG_INI(group, key, defValue) \
    settingIni->value("/" + group + "/" + key, defValue)

#endif // APPELLATION_H
