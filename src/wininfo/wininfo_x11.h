#ifndef WININFO_X11_H
#define WININFO_X11_H

#include <QImage>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <list>
#include <string>


class WinInfo_x11
{
public:
    WinInfo_x11();
    ~WinInfo_x11();

    std::list<Window> getWindowsList();
    void test();

    bool bSupportQuery();
    void getTopLevelWindows();
    std::string getWindowAtom(Window win, const char *atom);
    inline std::string getWindowName(Window win);

private:
    Display* m_pDisplay;
    std::list<Window> m_listWin;
};

#endif // WININFO_X11_H
