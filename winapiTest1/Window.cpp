#include "Window.h"

void Window::CalculateLayout()
{
    if (pRenderTarget == NULL)
        return;
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    const float x = size.width / 2;
    const float y = size.height / 2;
    const float radius = min(x, y);
    ellipse = D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius);

}

HRESULT Window::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget
        );
        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(0.12f, 0.78f, 0.78f);
            hr = pRenderTarget->CreateSolidColorBrush(color,&pBrush);
            if(SUCCEEDED(hr))
                CalculateLayout();
        }
    }
    return hr;
}

void Window::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void Window::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear();
        pRenderTarget->FillEllipse(ellipse, pBrush);

        hr = pRenderTarget->EndDraw();

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            DiscardGraphicsResources();
        EndPaint(m_hwnd, &ps);
    }
}

void Window::Resize()
{
    if (pRenderTarget == NULL)
        return;
    RECT rt;
    GetClientRect(m_hwnd, &rt);
    D2D1_SIZE_U size = D2D1::SizeU(rt.right, rt.bottom);

    pRenderTarget->Resize(size);
    CalculateLayout();
    InvalidateRect(m_hwnd, NULL, FALSE);
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;



    case WM_SIZE:
        Resize();
        return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}