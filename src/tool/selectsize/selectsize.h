// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef SELECTSIZE_H
#define SELECTSIZE_H

#include <QLabel>

// 选中矩形区域的尺寸大小，显示在左上角
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
//    void showEvent(QShowEvent *e) override;
    void paintEvent(QPaintEvent* e) override;
};

#endif // SELECTSIZE_H
