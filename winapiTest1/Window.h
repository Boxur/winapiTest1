#pragma once
#include "BaseWindow.h"
#include <d2d1.h>
//add a library search to the compiler
#pragma comment(lib,"d2d1.lib")
 
//a template function for safely releasing items
template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

//decleration of the class
class Window : public BaseWindow<Window>
{
    //ID2D1 resource class
    ID2D1Factory            *pFactory;
    //HWND handle of the render target (which for some reason is "ID2D1HwndRenderTarget" type and not just HWND bc why use whats already there)
    ID2D1HwndRenderTarget   *pRenderTarget;
    //a simple color brush for drawing
    ID2D1SolidColorBrush    *pBrush;
    //an eclipse that will be used as a circle
    D2D1_ELLIPSE            ellipse;

    //declaration of the methods (i will explain what they do in Window.cpp file above them)
    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();


public:
    //simple constructor to set the pointers to NULL
    Window() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {}

    //a simple methot to return the name of the Window
    PCWSTR ClassName() const { return L"Name I guess"; }
    //a default message handling function decration
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
