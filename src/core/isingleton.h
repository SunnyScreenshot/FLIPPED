/*******************************************************************
   * Copyright (c) 2021-2022 偕臧  All rights reserved.
   *
   * Author: XMuli <xmulitech@gmail.com>
   * GitHub: https://github.com/XMuli
   * Blog:   https://xmuli.tech
   *
   * Date:   2022.01.29
   * Description: 实现一个模板的静态单例
   ******************************************************************/
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

    ISingleton(T&&) = delete;
    ISingleton(const T&) = delete;
    void operator= (const T&) = delete;

protected:
    ISingleton() = default;
    virtual ~ISingleton() = default;
};

#define FRIEND_CREAT_SINGLETION(_name)  friend class ISingleton<_name>;

#endif // TSINGLETON_H
