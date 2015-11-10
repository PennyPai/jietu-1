#include "cscreenpublic.h"

QList<QRect> c_global_windown_rect_list; //用于存储所有的窗口
#ifdef Q_OS_WIN
#include <Windows.h>
#include <vector>
//声明回调函数
bool CALLBACK getWindownRect(HWND hwnd,LPARAM lParam);

bool CALLBACK getWindownRect(HWND hwnd,LPARAM lParam)
{
    if (IsWindow(hwnd) && IsWindowVisible(hwnd) && !IsIconic(hwnd))
    {
        RECT tempRect;
        QRect rect;
        ::GetWindowRect(hwnd,&tempRect);
        rect.setTopLeft(QPoint(tempRect.left,tempRect.top));
        //QRect 差一个px
        rect.setBottomRight(QPoint(tempRect.right - 1,tempRect.bottom - 1));
        if(!c_global_windown_rect_list.contains(rect))
        {
            c_global_windown_rect_list.append(rect);
        }
        //遍历子窗口,TODO
        //  ::EnumChildWindows(hwnd,(WNDENUMPROC)updateWindowRectList,0);
    }
    return true;
}
#endif

QList<QRect> getWindownRectList()
{
    c_global_windown_rect_list.clear();
    //遍历所有顶级窗口,更新窗口大小列表
    ::EnumWindows((WNDENUMPROC)getWindownRect,0);
    return c_global_windown_rect_list;
}
