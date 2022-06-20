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

// 1. HAIF_INTERVAL 为一半间隔，边框宽度一半高; 2 * HAIF_INTERVAL 为边框的宽度，为 getOuterRect - getRect  == getRect - getInnerRect == HAIF_INTERVAL
// 2. HAIF_INTERVAL 为边框一般的灵敏度，光标移动到上面便会变化形态
#define HAIF_INTERVAL 4

#define HAIF_R_BORDER_MARK 4  // 边框上标记点的半径

// custom widget parameter
#define CW_RADIRS 6  // 圆角矩形的圆角大小 eg: 绘画工具栏
#define CW_MARGIN 8  // 圆角矩形的边框留白
#define CW_ITEM_SPACE 10  // item 之间间隔


// 默认图片宽度、高度 px （再乘以缩放比）
#define ICON_WIDTH  24 * 2
#define ICON_HEIGHT 24 * 2

#define ICON_WIDTH_IN_MARGIN  5 * 2
#define ICON_HEIGHT_IN_MARGIN  5 * 2

#define COLOR_LABEL_WIDTH 12 * 2       // 圆形取色盘宽度
#define COLOR_PARA_MARGIN 6 * 2        // 取色盘, 圆形按钮的边框距离
#define COLOR_PARA_HOR_SPACING  7 * 2  // 取色盘, 圆形按钮之间的水平间距
#define COLOR_PARA_VER_SPACING  4 * 2  // 取色盘, 圆形按钮之间的竖直间距




#endif // XGLOBAL_H
