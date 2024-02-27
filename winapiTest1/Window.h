#pragma once
#include "BaseWindow.h"
#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")
 

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class Window : public BaseWindow<Window>
{
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;
    D2D1_ELLIPSE            ellipse;


    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();


public:
    Window() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {}

    PCWSTR ClassName() const { return L"Name I guess"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
