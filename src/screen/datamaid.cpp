// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped
#include "datamaid.h"

DataMaid::DataMaid(QObject *parent)
    : QObject{parent}
{
    init();
}

void DataMaid::init()
{
    resetGeneral();
    resetInterface();
    resetOutput();
    resetPin();
    resetHotkeys();
}

void DataMaid::resetGeneral()
{
    resetLanuage();
    resetFont();
    resetAutoRun();
    resetAsAdmin();
    resetLogLevel();
    resetAutoUpdate();
}

void DataMaid::resetInterface()
{
    resetBorderStyle();
    resetBorderColor();
    resetBorderWidth();
    resetCrosshairColor();
    resetCrosshairWidth();
    resetSmartWindow();
    resetCrosshair();
    resetShowCursor();
    resetAutoCopy2Clipboard();
}

void DataMaid::resetOutput()
{
    resetImageQuailty();
}

void DataMaid::resetPin()
{
    resetWindowShadow();
    resetOpacity();
    resetMaxSize();
}

void DataMaid::resetHotkeys()
{
    resetScrnCapture();
    resetDelayCapture();
    resetFullScrnCapture();
}
