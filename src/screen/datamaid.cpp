// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped
#include "datamaid.h"
#include <QFile>
#include <QDir>
#include <QPainter>
#include <QDateTime>
#include <QByteArray>
#include <QDomElement>
#include <QSvgRenderer>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsEffect>
#include "screenshot.h"
#include "../preference/appellation.h"

DataIni::DataIni(QObject *parent)
    : QObject{parent}
{
    resetAllData();
    writeToAllIni(true);
}

void DataIni::readFromAllIni()
{
    dataDotIni->beginGroup(INIT_GENERAL);
    const auto lanuage = dataDotIni->value(XC::tgLanuage, resetLanuage()).toString();
    const auto font = dataDotIni->value(XC::tgFont, resetFont()).toString();
    const auto autoRun = dataDotIni->value(XC::tgAutoRun, resetAutoRun()).toBool();
    const auto asAdmin = dataDotIni->value(XC::tgAsAdmin, resetAsAdmin()).toBool();
    const auto logLevel = dataDotIni->value(XC::tgLogLevel, resetLogLevel()).toString();
    const auto autoUpdate = dataDotIni->value(XC::tgAutoCheckUpdate, resetAutoUpdate()).toBool();
    dataDotIni->endGroup();
    setProperty("lanuage", lanuage);
    setProperty("font", font);
    setProperty("autoRun", autoRun);
    setProperty("asAdmin", asAdmin);
    setProperty("logLevel", logLevel);
    setProperty("autoUpdate", autoUpdate);

    dataDotIni->beginGroup(INIT_INTERFACE);
    const auto borderStyle = dataDotIni->value(XC::tiBorderStyle, resetBorderStyle()).toString();
    const auto borderColor = dataDotIni->value(XC::tiBorderColor, resetBorderColor()).toString();
    const auto borderWidth = dataDotIni->value(XC::tiBorderWidth, resetBorderWidth()).toInt();
    const auto crosshairColor = dataDotIni->value(XC::tiCrosshairColor, resetCrosshairColor()).toString();
    const auto crosshairWidth = dataDotIni->value(XC::tiCrosshairWidth, resetCrosshairWidth()).toInt();
    const auto smartWindow = dataDotIni->value(XC::tiSmartWindow, resetSmartWindow()).toBool();
    const auto crosshair = dataDotIni->value(XC::tiCrosshair, resetCrosshair()).toBool();
    const auto showCursor = dataDotIni->value(XC::tiShowCursor, resetShowCursor()).toBool();
    const auto autoCopy2Clipboard = dataDotIni->value(XC::tiAutoCopy2Clipboard, resetAutoCopy2Clipboard()).toBool();
    dataDotIni->endGroup();
    setProperty("borderStyle", borderStyle);
    setProperty("borderColor", borderColor);
    setProperty("borderWidth", borderWidth);
    setProperty("crosshairColor", crosshairColor);
    setProperty("crosshairWidth", crosshairWidth);
    setProperty("smartWindow", smartWindow);
    setProperty("crosshair", crosshair);
    setProperty("showCursor", showCursor);
    setProperty("autoCopy2Clipboard", autoCopy2Clipboard);

    dataDotIni->beginGroup(INIT_OUTPUT);
    const auto imageQuailty = dataDotIni->value(XC::toImageQuailty, resetImageQuailty()).toInt();
    dataDotIni->endGroup();
    setProperty("imageQuailty", imageQuailty);

    dataDotIni->beginGroup(INIT_PIN);
    const auto windowShadow = dataDotIni->value(XC::tpWindowShadow, resetWindowShadow()).toBool();
    const auto opacity = dataDotIni->value(XC::tpOpacity, resetOpacity()).toInt();
    const auto maxSize = dataDotIni->value(XC::tpMaxSize, resetMaxSize()).toInt();
    dataDotIni->endGroup();
    setProperty("windowShadow", windowShadow);
    setProperty("opacity", opacity);
    setProperty("maxSize", maxSize);

    dataDotIni->beginGroup(INIT_HOTKEYS);
    const auto scrnCapture = dataDotIni->value(XC::thScrnCapture, resetScrnCapture()).toString();
    const auto delayCapture = dataDotIni->value(XC::thDelayCapture, resetDelayCapture()).toString();
    const auto fullScrnCapture = dataDotIni->value(XC::thFullScreen, resetFullScrnCapture()).toString();
    dataDotIni->endGroup();
    setProperty("scrnCapture", scrnCapture);
    setProperty("delayCapture", delayCapture);
    setProperty("fullScrnCapture", fullScrnCapture);
}

void DataIni::writeToAllIni(const bool bReset)
{
    writeToGeneralIni(bReset);
    writeToInterfaceIni(bReset);
    writeToOutputIni(bReset);
    writeToPinIni(bReset);
    writeToHotkeysIni(bReset);
}

void DataIni::writeToGeneralIni(const bool bReset)
{
    if (bReset)  resetGeneral();
    dataDotIni->beginGroup(INIT_GENERAL);
    dataDotIni->setValue(XC::tgLanuage, m_lanuage);
    dataDotIni->setValue(XC::tgFont, m_font);
    dataDotIni->setValue(XC::tgAutoRun, m_autoRun);
    dataDotIni->setValue(XC::tgAsAdmin, m_asAdmin);
    dataDotIni->setValue(XC::tgLogLevel, m_logLevel);
    dataDotIni->setValue(XC::tgAutoCheckUpdate, m_autoUpdate);
    dataDotIni->endGroup();
}

void DataIni::writeToInterfaceIni(const bool bReset)
{
    if (bReset)  resetInterface();
    dataDotIni->beginGroup(INIT_INTERFACE);
    dataDotIni->setValue(XC::tiBorderStyle, m_borderStyle);
    dataDotIni->setValue(XC::tiBorderColor, m_borderColor);
    dataDotIni->setValue(XC::tiBorderWidth, m_borderWidth);
    dataDotIni->setValue(XC::tiCrosshairColor, m_crosshairColor);
    dataDotIni->setValue(XC::tiCrosshairWidth, m_crosshairWidth);
    dataDotIni->setValue(XC::tiSmartWindow, m_smartWindow);
    dataDotIni->setValue(XC::tiCrosshair, m_crosshair);
    dataDotIni->setValue(XC::tiShowCursor, m_showCursor);
    dataDotIni->setValue(XC::tiAutoCopy2Clipboard, m_autoCopy2Clipboard);
    dataDotIni->endGroup();
}

void DataIni::writeToOutputIni(const bool bReset)
{
    if (bReset)  resetOutput();
    dataDotIni->beginGroup(INIT_OUTPUT);
    dataDotIni->setValue(XC::toImageQuailty, m_imageQuailty);
    dataDotIni->endGroup();
}

void DataIni::writeToPinIni(const bool bReset)
{
    if (bReset)  resetPin();
    dataDotIni->beginGroup(INIT_PIN);
    dataDotIni->setValue(XC::tpWindowShadow, m_windowShadow);
    dataDotIni->setValue(XC::tpOpacity, m_opacity);
    dataDotIni->setValue(XC::tpMaxSize, m_maxSize);
    dataDotIni->endGroup();
}

void DataIni::writeToHotkeysIni(const bool bReset)
{
    if (bReset)  resetHotkeys();
    dataDotIni->beginGroup(INIT_HOTKEYS);
    dataDotIni->setValue(XC::thScrnCapture, m_scrnCapture);
    dataDotIni->setValue(XC::thDelayCapture, m_delayCapture);
    dataDotIni->setValue(XC::thFullScreen, m_fullScrnCapture);
    dataDotIni->endGroup();
}

void DataIni::resetAllData()
{
    resetGeneral();
    resetInterface();
    resetOutput();
    resetPin();
    resetHotkeys();
}

void DataIni::resetGeneral()
{
    resetLanuage();
    resetFont();
    resetAutoRun();
    resetAsAdmin();
    resetLogLevel();
    resetAutoUpdate();
}

void DataIni::resetInterface()
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

void DataIni::resetOutput()
{
    resetImageQuailty();
}

void DataIni::resetPin()
{
    resetWindowShadow();
    resetOpacity();
    resetMaxSize();
}

void DataIni::resetHotkeys()
{
    resetScrnCapture();
    resetDelayCapture();
    resetFullScrnCapture();
}

DataMaid::DataMaid(QObject *parent)
{
    m_dataIni.resetAllData();
    init();
}

void DataMaid::init()
{
    connect(&m_dataIni, &DataIni::sigWindowShadow, this, &DataMaid::sigWindowShadow);
    connect(&m_dataIni, &DataIni::sigOpacity, this, &DataMaid::sigOpacity);
    connect(&m_dataIni, &DataIni::sigMaxSize, this, &DataMaid::sigMaxSize);
}

QVariant DataMaid::paraValue(const char *key)
{
    return m_dataIni.property(key);
}

void DataMaid::setParaValue(const char *key, const QVariant &val)
{
    m_dataIni.setProperty(key, val);
}

void DataMaid::readFromAllIni()
{
    m_dataIni.readFromAllIni();
}

void DataMaid::writeToAllIni(const bool bReset)
{
    m_dataIni.writeToAllIni(bReset);
}

void DataMaid::writeToGeneralIni(const bool bReset)
{
    m_dataIni.writeToGeneralIni(bReset);
}

void DataMaid::writeToInterfaceIni(const bool bReset)
{
    m_dataIni.writeToInterfaceIni(bReset);
}

void DataMaid::writeToOutputIni(const bool bReset)
{
    m_dataIni.writeToOutputIni(bReset);
}

void DataMaid::writeToPinIni(const bool bReset)
{
    m_dataIni.writeToPinIni(bReset);
}

void DataMaid::writeToHotkeysIni(const bool bReset)
{
    m_dataIni.writeToHotkeysIni(bReset);
}

double DataMaid::scale(QScreen* screen)
{
    double scale = 0;
#if defined(Q_OS_WIN) ||  defined(Q_OS_LINUX)          // or defined(Q_WS_WIN) || defined(Q_WS_X11)
    scale = screen->logicalDotsPerInch() / 96.0;
    if (scale < 1.25)
        return 1;
    else if (1.25 <= scale && scale < 1.5)
        return 1.25;
    else if (1.5 <= scale && scale < 1.75)
        return 1.5;
    else if (1.75 <= scale && scale < 2)
        return 1.75;
    else if (2 <= scale && scale < 2.25)
        return 2;
    else if (2.25 <= scale && scale < 2.5)
        return 2.25;
    else if (2.5 <= scale && scale < 3)
        return 2.5;
    else if (3 <= scale && scale < 3.5)
        return 3;
    else if (3.5 <= scale && scale < 4)
        return 3.5;
    else
        return scale;
#elif  defined(Q_OS_MAC)
    scale = screen->logicalDotsPerInch() / 72.0;
    return scale;
#endif
}

/*!
 * \brief changeSVGColor 修改 .svg 的填充色
 * \param path   .svg 文件路径
 * \param shape  欲修改的图形， 通常是 path、偶尔是 rect 等
 * \param color  填充的颜色
 * \param size   svg 伸缩后的尺寸
 * \return 一个 QIcon 对象
 */
QIcon DataMaid::ChangeSVGColor(QString path, QString shape, QColor color, QSize size)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);
    QDomElement elem = doc.documentElement(); // const 和 值传递
    SetAttrRecur(elem, shape, "fill", color.name());

    QSvgRenderer svgRenderer(doc.toByteArray());
    // create pixmap target (could be a QImage)
    QPixmap pix(svgRenderer.defaultSize().scaled(size.width(), size.height(), Qt::KeepAspectRatio));  // fix: 修改 svg 颜色后略有模糊
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    svgRenderer.render(&pixPainter);
    QIcon myicon(pix);
    return myicon;
}

void DataMaid::SetAttrRecur(QDomElement& elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
        elem.setAttribute(strattr, strattrval);

    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++) {
        if (!elem.childNodes().at(i).isElement())
            continue;

        QDomElement t = elem.childNodes().at(i).toElement();
        SetAttrRecur(t, strtagname, strattr, strattrval);
    }
}

const QPixmap* DataMaid::smoothMosaic(QPixmap* pixmap, int radius)
{
    if (!pixmap)
        return nullptr;

    radius = qMax<int>(10, radius);
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(*pixmap);
    item->setGraphicsEffect(blur);

    QGraphicsScene scene;
    scene.addItem(item);

    QPainter painter(pixmap);
    const double DPI = ScreenShot::GetDevicePixelRatio();
    const QRect tRt(pixmap->rect().topLeft() / DPI, pixmap->rect().size() / DPI);
    scene.render(&painter, tRt, QRectF());

    blur->setBlurRadius(radius + 2);
    // multiple repeat for make blur effect stronger
    scene.render(&painter, tRt, QRectF());

    return pixmap;
}

const QImage DataMaid::pixlelatedMosaic(QPixmap* pixmap, int px)
{
    if (!pixmap)
        return QImage();

    const QImage& image = pixmap->toImage();
    QImage* pImage = const_cast<QImage*>(&image);
    const int width = image.width();
    const int height = image.height();

    for (int i = 0; i < width; i += px) {
        for (int j = 0; j < height; j += px) {
            QSize size(px, px);
            if (width - i < px)
                size.setWidth(width - i);
            if (height - j < px)
                size.setHeight(height - j);

            const QPoint topLeft(i, j);
            const QRect rt(topLeft, size);
            //qDebug()<<"--------->>i:"<< i << "  j:" << j << "  rt:" << rt;
            QColor color = pImage->pixelColor(rt.topLeft());
            for (int x = rt.x(); x <= rt.right(); ++x) {
                for (int y = rt.y(); y <= rt.bottom(); ++y)
                    pImage->setPixelColor(x, y, color);
            }
        }
    }

    return image;
}

void DataMaid::setAutoRun()
{
#if defined(Q_OS_WIN)
    dataDotIni->beginGroup(INIT_GENERAL);
    const bool bAutoRun = dataDotIni->value(tgAutoRun, true).toBool();
    dataDotIni->endGroup();

    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (bAutoRun) {
        QString strAppPath = QDir::toNativeSeparators(qApp->applicationFilePath());
        strAppPath.replace(QChar('/'), QChar('\\'), Qt::CaseInsensitive);
        reg.setValue("Flipped", strAppPath);
    } else {
        reg.setValue("Flipped", "");
    }
#else
    // TODO 2023.03.31:
#endif
}

const QString DataMaid::formatToFileName(const QString name)
{
    auto fileName = name.trimmed();
    auto first = fileName.indexOf("$");
    auto second = fileName.lastIndexOf("$");
    auto nameTime = fileName.mid(first + 1, second - first - 1);
    auto finalyName = fileName.replace(first, second - first + 1, QDateTime::currentDateTime().toString(nameTime));

    return finalyName;
}
