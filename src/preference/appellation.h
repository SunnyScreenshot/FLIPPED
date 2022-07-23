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



#endif // APPELLATION_H
