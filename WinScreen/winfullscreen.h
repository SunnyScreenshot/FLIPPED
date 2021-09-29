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
	QList<QScreen *> m_screens;  // кЫспфад╩
	QScreen* m_primaryScreen;    // жВфад╩

	QPixmap* m_currPixmap;       // ╣╠г╟фад╩╫ьм╪
	QPixmap* m_blurPixmap;       // узуж
	QPixmap* m_basePixmap;       // ╣╠г╟фад╩╫ьм╪ + узуж
};


#endif //PICSHOT_WINFULLSCREEN_H
