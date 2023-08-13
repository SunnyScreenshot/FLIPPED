// SPDX-License-Identifier:
// SPDX-FileCopyrightText: 2021-2023 XMuli <xmulitech@gmail.com>
//
// Source: https://github.com/XMuli/FlippedCode

#ifndef TSINGLETON_H
#define TSINGLETON_H

template<typename T>
class ISingleton
{
public:
    static T& instance() {
        static T instance;
        return instance;
    }

    ISingleton(const T&) = delete;
    T& operator=(const T&) = delete;
    ISingleton(T&&) = delete;
    T& operator=(T&&) = delete;

protected:
    ISingleton() = default;
    virtual ~ISingleton() = default;
};

#define FRIEND_CREAT_SINGLETION(_name)  friend class ISingleton<_name>;

#endif // TSINGLETON_H
