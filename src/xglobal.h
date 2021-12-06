//
// Created by XMuli <xmulitech@gmail.com> on 2021/11/09.
//
#ifndef XGLOBAL_H
#define XGLOBAL_H

// 默认图片宽度、高度 px（再乘以缩放比）
#define ICON_WIDTH  20
#define ICON_HEIGHT 20

// 1. HAIF_INTERVAL 为一半间隔，边框宽度一半高; 2 * HAIF_INTERVAL 为边框的宽度，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL
// 2. HAIF_INTERVAL 为边框一般的灵敏度，光标移动到上面便会变化形态
#define HAIF_INTERVAL 4

#define HAIF_R_BORDER_MARK 4  // 边框上标记点的半径

#define ROUNDED_RECT 8  // 圆角矩形的圆角大小

#endif // XGLOBAL_H
