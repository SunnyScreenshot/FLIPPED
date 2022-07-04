/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date:   2022.07.02
 * Description: 选中矩形区域的尺寸大小，显示在左上角
 ******************************************************************/
#ifndef SELECTSIZE_H
#define SELECTSIZE_H

#include <QLabel>

class SelectSize : public QLabel
{
    Q_OBJECT
public:
    explicit SelectSize(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit SelectSize(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SelectSize() = default;

signals:
    void sigTextChanged(QString text);

public slots:
    void onTextChanged(QString text);

private:
    void initUI();

protected:
    void paintEvent(QPaintEvent* e) override;

};

#endif // SELECTSIZE_H
