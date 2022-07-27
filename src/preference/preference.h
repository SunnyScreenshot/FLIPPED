/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.03
 * Description: 偏好设置界面的 Main UI
 ******************************************************************/
#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QHBoxLayout;
QT_END_NAMESPACE

class QHotkey;
class Preference : public QWidget
{
    Q_OBJECT
public:
    explicit Preference(QWidget* parent = nullptr);
    ~Preference() = default;

private:
    void initUI();
    QHBoxLayout* creatResetBtn(QString objectName);
    inline bool checkBoxState2Bool(int state);

    QWidget* tabGeneral();             // 常规
    QWidget* tabInterface();           // 界面
    QWidget* tabOutput();              // 输出
    QWidget* tabPin();                 // 贴图
    QWidget* tabHotkeys();             // 快捷键
    QWidget* tabAbout();               // 关于

protected slots:
    void onReset();

    // tabGeneral
    void onLanuageChange(const QString& language);
    void onSelfStart(int sta);
    void onAsAdmin(int sta);
    void onLogLevelChange(const QString& language);
    void onAutoCheck(int sta);
    void onUpdate();

    // tabInterface
    void onBorderStyle(const QString& style);
    void onBorderColor(const QColor& color);
    void onBorderWidth(int val);
    void onCrosshairColor(const QColor& color);
    void onCrosshairWidth(int val);
    void onSmartWindow(int val);
    void onShowCrosshair(int val);
    void onShowCursor(int val);
    void onAutoCopyToClip(int val);

    // tabOutput
    void onImageQuailty(int val);
    void onFileName(const QString& name);
    void onQuickSavePath(const QString& path);
    void onAutoSavePath(const QString& path);
    void onConfigPath(const QString& path);
    void onChoosePath();

    // tabPin
    void onWindowShadow(int sta);
    void onOpacity(int val);
    void onMaxSize(int val);

private:
    double m_scale;
};

#endif // PREFERENCE_H
