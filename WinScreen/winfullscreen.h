//
// Created by xmuli on 2021/9/29.
//

#ifndef PICSHOT_WINFULLSCREEN_H
#define PICSHOT_WINFULLSCREEN_H

#include <QWidget>
#include <QList>
#include <QColor>

QT_BEGIN_NAMESPACE
class QScreen;
class QPixmap;
QT_END_NAMESPACE

class WinFullScreen : public  QWidget
{
    Q_OBJECT
public:
    explicit WinFullScreen(QWidget *parent = nullptr);
    ~WinFullScreen() override;

	void getVirtualScreen();
	void display();
	double getScale();
	double getScale(QScreen *screen);
	QPixmap* getblurPixmap(QColor color = QColor(255, 255, 255, 0.2 * 255));
	QPixmap* getBasePixmap();

protected:
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
	QList<QScreen *> m_screens;  // 所有屏幕
	QScreen* m_primaryScreen;    // 主屏幕

	QPixmap* m_currPixmap;       // 当前屏幕截图
	QPixmap* m_blurPixmap;       // 遮罩
	QPixmap* m_basePixmap;       // 当前屏幕截图 + 遮罩
};


#endif //PICSHOT_WINFULLSCREEN_H
