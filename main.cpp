#include <QApplication>
#include <QPushButton>
#include "WinScreen/winfullscreen.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

	WinFullScreen winFullScreen;
	winFullScreen.display();
	winFullScreen.getBasePixmap();
	//winFullScreen.showFullScreen(); //只顶级窗口有效，且不是双屏全屏
	winFullScreen.show();

    return QApplication::exec();
}
