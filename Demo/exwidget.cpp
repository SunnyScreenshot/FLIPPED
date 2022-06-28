#include "exwidget.h"
#include <windows.h>


ExWidget::ExWidget(QWidget *parent)
    : QWidget(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QColor color(255, 50, 50, 80);


    HWND hMoudle = (HWND)(winId());
    HMODULE hDLL = LoadLibrary(L"Acrylic");

    using fun = void (*)(HWND hWnd);

    fun pSetBlur = (fun)GetProcAddress(hDLL, "setBlur");
    pSetBlur((HWND)(winId()));

    FreeLibrary(hDLL);

}

ExWidget::~ExWidget()
{
}

