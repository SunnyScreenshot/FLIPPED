// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef XQTLOG_H
#define XQTLOG_H

#include <QObject>

void CustomOutputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);

class XQtLog : public QObject
{
    Q_OBJECT
public:
    explicit XQtLog(QObject *parent = nullptr);
};

#endif // XQTLOG_H
