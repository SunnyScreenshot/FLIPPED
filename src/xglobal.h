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

#define HEIGHT_SCALE = 1.5  // 4K 采用 1.5；  1080 则 1 倍缩放

// custom widget parameter



// 默认图片宽度、高度 px （再乘以缩放比）
#define ICON_WIDTH  24 * 2
#define ICON_HEIGHT 24 * 2

#define ICON_WIDTH_IN_MARGIN  5 * 2
#define ICON_HEIGHT_IN_MARGIN  5 * 2


// 上面那个 bar 的间距和一些参数，暂时没使用下面的
#define CW_RADIRS 6  // 圆角矩形的圆角大小 eg: 绘画工具栏
//#define CW_MARGIN 8 * 2  // 圆角矩形的边框留白
#define CW_ITEM_SPACE 10 * 2  // item 之间间隔

// 上面、下面两个 Bar 的一些公共参数
#define SPACER_LINE_WIDTH  1 * 2       // 分割线的宽度
#define SPACER_LINE_HEIGHT 20 * 2      // 分割线的高度

#define BAR_MARGIN_HOR 10 * 2      // 两个 bar； 选取的最小边框. 左右间距
#define BAR_MARGIN_VER 8 * 2       // 两个 bar； 选取的最小边框. 上下间距  --> 取色盘 不显示时
#define BAR_MARGIN_VER_HAS_COLOR_PARA 6 * 2  // 特殊时、仅底部 bar； 选取的最小边框. 上下间距  -->  取色盘 显示时，由于非等距离， 故要小一点

// 底部参数 Button Bar Para
#define BB_PARA_SPACING 10 * 2         // 圆形取色盘宽度


// 线宽度 3个
#define WIDTH_LABEL_WIDTH 24 * 2       // 色盘宽度
#define WIDTH_PARA_MARGIN 0 * 2        // 等大的小圆形按钮的边框距离
#define WIDTH_PARA_HOR_SPACING  0 * 2  // 等大的小圆形按钮之间的水平间距
#define WIDTH_PARA_VER_SPACING  0 * 2  // 等大的小圆形按钮之间的竖直间距

// 八个
#define COLOR_LABEL_WIDTH 12 * 2       // 圆形取色盘宽度
#define COLOR_PARA_MARGIN_VER 6 * 2    // 取色盘, 圆形按钮的边框 上下 距离
#define COLOR_PARA_MARGIN_HOR 10 * 2   // 取色盘, 圆形按钮的边框 左右 距离
#define COLOR_PARA_HOR_SPACING  7 * 2  // 取色盘, 圆形按钮之间的水平间距
#define COLOR_PARA_VER_SPACING  4 * 2  // 取色盘, 圆形按钮之间的竖直间距

#endif // XGLOBAL_H
