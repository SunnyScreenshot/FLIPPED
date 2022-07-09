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

// 截图-选中矩形
#define SELECT_RECT_PEN_WIDTH  1 * 1           // 选中矩形的边框, 其画笔的默认宽度
#define SELECT_ASSIST_RECT_PEN_WIDTH  4 * 1    // 四角落辅助的 ∟，其画笔的默认宽度
#define SELECT_ASSIST_RECT_WIDTH  30 * 1       // 四角周辅助的 ∟ 的宽度
#define SELECT_ASSIST_RECT_HEIGHT 30 * 1       // 四角周辅助的 ∟ 的高度

// 默认图片宽度、高度 px （再乘以缩放比）
#define ICON_WIDTH  24 * 1
#define ICON_HEIGHT 24 * 1

#define ICON_WIDTH_IN_MARGIN  5 * 1
#define ICON_HEIGHT_IN_MARGIN  5 * 1


// 上面、下面两个 SelectBar、 ParameterBar 的一些公共参数
//#define B_SPACER_LINE_WIDTH  1 * 1       // 分割线的宽度
#define B_SPACER_LINE_HEIGHT 20 * 1      // 分割线的高度
#define B_RADIRS 6  // 圆角矩形的圆角大小 eg: 绘画工具栏

// 顶部 SelectBar 中的项目
#define SB_MARGIN_HOR 10 * 1      // 两个 bar； 选取的最小边框. 左右间距
#define SB_MARGIN_VER 8 * 1       // 两个 bar； 选取的最小边框. 上下间距  --> 取色盘 不显示时

#define SB_ITEM_SPACE 10 * 1  // item 之间间隔

// 底部 ParameterBar 中的项目
#define PB_ITEM_SPACE 0 * 1
#define PB_MARGIN_HOR 0 * 1
#define PB_MARGIN_VER 0 * 1

// 底部 ParameterBar 中的子项, 如 ManageBar 的间距和一些参数
#define MB_ITEM_SPACE 0 * 1       // ManageBar 中 item 的间距
#define MB_MARGIN_HOR 10 * 1      // ManageBar 选取的最小边框. 左右间距
#define MB_MARGIN_VER 8 * 1       // ManageBar 选取的最小边框. 上下间距 

// 取色盘 ColorParaBar 的八个圆 
  // Mode: CPB_ParaBar --> QGridLayout
#define CPB_PB_LABEL_WIDTH 12 * 1              // 圆形取色盘宽度
#define CPB_PB_MARGIN_HOR 10 * 1               // 取色盘, 圆形按钮的边框 左右 距离
#define CPB_PB_MARGIN_VER 6 * 1                // 取色盘, 圆形按钮的边框 上下 距离
#define CPB_PB_SPACING_HOR  7 * 1              // 取色盘, 圆形按钮之间的水平间距
#define CPB_PB_SPACING_VER  4 * 1              // 取色盘, 圆形按钮之间的竖直间距

  // Mode: CPB_HighLight  -> QHBoxLayout
#define CPB_HL_LABEL_WIDTH 9 * 1            // 圆形取色盘宽度
#define CPB_HL_MARGIN_HOR 10 * 1               // 取色盘, 圆形按钮的边框 左右 距离
#define CPB_HL_MARGIN_VER 6 * 1                // 取色盘, 圆形按钮的边框 上下 距离
#define CPB_HL_SPACING_HOR  5 * 1              // 取色盘, 圆形按钮之间的水平间距
#define CPB_HL_SPACING_VER  0 * 1              // 取色盘, 圆形按钮之间的竖直间距

  // 其中的外圆
#define CPB_MARGIN_SELECTED  1 * 1          // 取色盘, 选中颜色的圆距离外框圆的距离
#define CPB_WIDTH_SELECTED  1 * 1           // 取色盘, 圆形按钮之间的竖直间距

// 几个 combobox 的详细（序号、大小）
#define COMBOBOX_WIDTH 48 * 1               // combobox 宽度
#define COMBOBOX_HEIGHT 20 * 1              // combobox 宽度

// 选择线宽度 WidthParaBar 的 3 个圆，已经替换，后面可以删
#define WIDTH_LABEL_WIDTH 24 * 1            // 色盘宽度
#define WIDTH_PARA_MARGIN 0 * 1             // 等大的小圆形按钮的边框距离
#define WIDTH_PARA_HOR_SPACING  0 * 1       // 等大的小圆形按钮之间的水平间距
#define WIDTH_PARA_VER_SPACING  0 * 1       // 等大的小圆形按钮之间的竖直间距

// 左上角用来显示，选中矩形区域 SelectSize 的大小
#define SS_MARGIN_LEFT 3 * 1                // 边框左部的间距
#define SS_MARGIN_RIGHT 5 * 1               // 边框右部的间距
#define SS_MARGIN_TOP 2 * 1                 // 边框顶部的间距
#define SS_MARGIN_BOTTOM 3 * 1              // 边框底部的间距
#define SS_SPACE_TO_SELECTRECT 10 * 1       // 此部件 底部距离选中框的顶部的距离
#define SS_RADIRS 2                         // 此部件 圆角矩形的圆角
//#define SS_MARGIN_BOTTOM 3 * 1            // 边框底部的间距
//#define SS_HOR_SPACING  0 * 1             // 水平间距
//#define SS_VER_SPACING  0 * 1             // 竖直间距
                                            




// 偏好主 UI 界面 Preference 缩写 PRE_
#define PRE_MARGIN_HOR 10 * 1               // 布局到水平间隔
#define PRE_MARGIN_VER 10 * 1               // 布局到垂直间隔
#define PRE_MARGIN_VER_TOP 10 * 1           // 布局到顶上面间隔
#define PRE_MARGIN_VER_BOTTOM 10 * 1        // 布局到最底部间隔

// Tab General GridLayout: 缩写 TIG_     
#define TGV_SPACING_VER  14 * 1             // QVBoxLayout 的 spacing
#define TGG_SPACING_VER  9 * 1              // GridLayout 的 spacing
#define TGG_MARGIN_HOR  14 * 1              // 内层的边框距离
#define TGG_MARGIN_VER  14 * 1              // 内层的边框距离

// Tab Interface GridLayout: 缩写 TIG_     
#define TIV_SPACING_VER  14 * 1             // QVBoxLayout 的 spacing
#define TIG_SPACING_VER  9 * 1              // GridLayout 的 spacing
#define TIG_MARGIN_HOR  14 * 1              // 内层的边框距离
#define TIG_MARGIN_VER  14 * 1              // 内层的边框距离

// Tab Hotkeys GridLayout: 缩写 THG_
#define THV_SPACING_VER  14 * 1             // QVBoxLayout 的 spacing
#define THG_SPACING_VER  7 * 1              // GridLayout 的 spacing
#define THG_MARGIN_HOR  14 * 1              // 内层的边框距离
#define TIG_MARGIN_VER  14 * 1              // 内层的边框距离

// Tab About GridLayout: 缩写 TAG_     
#define TAV_SPACING_VER  14 * 1             // QVBoxLayout 的 spacing
#define TAG_SPACING_VER  7 * 1              // GridLayout 的 spacing
#define TAG_MARGIN_HOR  14 * 1              // 内层的边框距离
#define TAG_MARGIN_VER  25 * 1              // 内层的边框距离

#endif // XGLOBAL_H
