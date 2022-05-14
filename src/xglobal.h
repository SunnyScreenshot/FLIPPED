/*******************************************************************
 * Copyright (c) 2021-2022 偕臧  All rights reserved.
 *
 * Author: XMuli <xmulitech@gmail.com>
 * GitHub: https://github.com/XMuli
 * Blog:   https://xmuli.tech
 *
 * Date: 2021.11.09
 * Description: 基本定义的宏
 ******************************************************************/

#ifndef XGLOBAL_H
#define XGLOBAL_H

// 默认图片宽度、高度 px（再乘以缩放比）
#define ICON_WIDTH  20
#define ICON_HEIGHT 20


// 1. HAIF_INTERVAL 为一半间隔，边框宽度一半高; 2 * HAIF_INTERVAL 为边框的宽度，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL
// 2. HAIF_INTERVAL 为边框一般的灵敏度，光标移动到上面便会变化形态
#define HAIF_INTERVAL 4

#define HAIF_R_BORDER_MARK 4  // 边框上标记点的半径

#define ROUNDED_RECT 4  // 圆角矩形的圆角大小   1920 * 1080 --> 4px 合适；

#endif // XGLOBAL_H
