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
    QHBoxLayout* creatResetBtn();

    QWidget* tabGeneral();             // 常规
    QWidget* tabInterface();           // 界面
    QWidget* tabOutput();              // 输出
    QWidget* tabPin();                 // 贴图
    QWidget* tabHotkeys();             // 快捷键
    QWidget* tabAbout();               // 关于

private:
    double m_scale;
};

#endif // PREFERENCE_H
