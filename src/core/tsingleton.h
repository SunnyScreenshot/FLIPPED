/*******************************************************************
 * Copyright (C)  2022  偕臧  All rights reserved.
 *
 * Author: 偕臧 <xmulitech@gmail.com>
 * GitHub: https://github.com/xmuli
 *
 * Create: 2022.01.29
 * Modify: 2022.01.29
 * Description: 实现一个模板的静态单例
 ******************************************************************/
#ifndef TSINGLETON_H
#define TSINGLETON_H

template<typename T>
class TSingleton
{
public:
    static T& instance() {
        static T instance;
        return instance;
    }

    TSingleton(T&&) = delete;
    TSingleton(const T&) = delete;
    void operator= (const T&) = delete;

protected:
    TSingleton() = default;
    virtual ~TSingleton() = default;
};

#endif // TSINGLETON_H
