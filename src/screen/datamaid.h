// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/Flipped

#ifndef DATAMAID_H
#define DATAMAID_H

#include <QObject>
#include <QGlobalStatic>

// .ini config data helper
class DataMaid : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lanuage MEMBER m_lanuage RESET resetLanuage NOTIFY sigLanuage)
    Q_PROPERTY(QString font MEMBER m_font RESET resetFont NOTIFY sigFont)
    Q_PROPERTY(bool autoRun MEMBER m_autoRun RESET resetAutoRun NOTIFY sigAutoRun)
    Q_PROPERTY(bool asAdmin MEMBER m_asAdmin RESET resetAsAdmin NOTIFY sigAsAdmin)
    Q_PROPERTY(QString logLevel MEMBER m_logLevel RESET resetLogLevel NOTIFY sigLogLevel)
    Q_PROPERTY(bool autoUpdate MEMBER m_autoUpdate RESET resetAutoUpdate NOTIFY sigAutoUpdate)

    Q_PROPERTY(QString borderStyle MEMBER m_borderStyle RESET resetBorderStyle NOTIFY sigBorderStyle)
    Q_PROPERTY(QString borderColor MEMBER m_borderColor RESET resetBorderColor NOTIFY sigBorderColor)
    Q_PROPERTY(int borderWidth MEMBER m_borderWidth RESET resetBorderWidth NOTIFY sigBorderWidth)
    Q_PROPERTY(QString crosshairColor MEMBER m_crosshairColor RESET resetCrosshairColor NOTIFY sigCrosshairColor)
    Q_PROPERTY(int crosshairWidth MEMBER m_crosshairWidth RESET resetCrosshairWidth NOTIFY sigCrosshairWidth)
    Q_PROPERTY(bool smartWindow MEMBER m_smartWindow RESET resetSmartWindow NOTIFY sigSmartWindow)
    Q_PROPERTY(bool crosshair MEMBER m_crosshair RESET resetCrosshair NOTIFY sigCrosshair)
    Q_PROPERTY(bool showCursor MEMBER m_showCursor RESET resetShowCursor NOTIFY sigShowCursor)
    Q_PROPERTY(bool autoCopy2Clipboard MEMBER m_autoCopy2Clipboard RESET resetAutoCopy2Clipboard NOTIFY sigAutoCopy2Clipboard)

    Q_PROPERTY(int imageQuailty MEMBER m_imageQuailty RESET resetImageQuailty NOTIFY sigImageQuailty)

    Q_PROPERTY(bool m_windowShadow MEMBER m_windowShadow RESET resetWindowShadow NOTIFY sigWindowShadow)
    Q_PROPERTY(int opacity MEMBER m_opacity RESET resetOpacity NOTIFY sigOpacity)
    Q_PROPERTY(int maxSize MEMBER m_maxSize RESET resetMaxSize NOTIFY sigMaxSize)

    Q_PROPERTY(QString scrnCapture MEMBER m_scrnCapture RESET resetScrnCapture NOTIFY sigScrnCapture)
    Q_PROPERTY(QString delayCapture MEMBER m_delayCapture RESET resetDelayCapture NOTIFY sigDelayCapture)
    Q_PROPERTY(QString fullScrnCapture MEMBER m_fullScrnCapture RESET resetFullScrnCapture NOTIFY sigFullScrnCapture)

public:
    explicit DataMaid(QObject *parent = nullptr);

    void init();
    void resetGeneral();
    void resetInterface();
    void resetOutput();
    void resetPin();
    void resetHotkeys();

public:
    void resetLanuage() { m_lanuage = "en_US";}
    void resetFont() { m_font = "SimSun,9";}
    void resetAutoRun() { m_autoRun = true;}
    void resetAsAdmin() { m_asAdmin = false;}
    void resetLogLevel() { m_logLevel = "debug";}
    void resetAutoUpdate() { m_autoUpdate = false;}

    void resetBorderStyle() { m_borderStyle = "flipped";}
    void resetBorderColor() { m_borderColor = "#0e70ff";}
    void resetBorderWidth() { m_borderWidth = 2;}
    void resetCrosshairColor() { m_crosshairColor = "#db000f";}
    void resetCrosshairWidth() { m_crosshairWidth = 1;}
    void resetSmartWindow() { m_smartWindow = true;}
    void resetCrosshair() { m_crosshair = false;}
    void resetShowCursor() { m_showCursor = false;}
    void resetAutoCopy2Clipboard() { m_autoCopy2Clipboard = true;}

    void resetImageQuailty() { m_imageQuailty = -1;}

    void resetWindowShadow() { m_windowShadow = false;}
    void resetOpacity() { m_opacity = 100;}
    void resetMaxSize() { m_maxSize = 100000;}

    void resetScrnCapture() { m_scrnCapture = "Ctrl+Shift+L";}       // TODO 2023.04.02: 对应的 Mac 上面需要修改
    void resetDelayCapture() { m_delayCapture = "Ctrl+Shift+S";}
    void resetFullScrnCapture() { m_fullScrnCapture = "Ctrl+Shift+F";}

signals:
    void sigLanuage(QString language);
    void sigFont(QString font);
    void sigAutoRun(bool enable);
    void sigAsAdmin(bool enable);
    void sigLogLevel(QString level);
    void sigAutoUpdate(bool enable);

    void sigBorderStyle(QString style);
    void sigBorderColor(QString color);
    void sigBorderWidth(int width);
    void sigCrosshairColor(QString color);
    void sigCrosshairWidth(int width);
    void sigSmartWindow(bool enable);
    void sigCrosshair(bool enable);
    void sigShowCursor(bool enable);
    void sigAutoCopy2Clipboard(bool enable);

    void sigImageQuailty(int val);

    void sigWindowShadow(bool shadow);
    void sigOpacity(int val);
    void sigMaxSize(int val);

    void sigScrnCapture(QString hotkey);
    void sigDelayCapture(QString hotkey);
    void sigFullScrnCapture(QString hotkey);

private:
    QString m_lanuage;             // General
    QString m_font;
    bool    m_autoRun;
    bool    m_asAdmin;
    QString m_logLevel;
    bool    m_autoUpdate;

    QString m_borderStyle;         // Interface
    QString m_borderColor;
    int     m_borderWidth;
    QString m_crosshairColor;
    int     m_crosshairWidth;
    bool    m_smartWindow;
    bool    m_crosshair;
    bool    m_showCursor;
    bool    m_autoCopy2Clipboard;

    int     m_imageQuailty;        // Output

    bool    m_windowShadow;        // Pin
    int     m_opacity;
    int     m_maxSize;

    QString m_scrnCapture;         // Hotkeys
    QString m_delayCapture;
    QString m_fullScrnCapture;
};
Q_GLOBAL_STATIC(DataMaid, dataMaid);


#endif // DATAMAID_H
