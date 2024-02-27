#include "Window.h"

//a method for calculating the graphics layout, which in this scenario is just resizing the ellipse
void Window::CalculateLayout()
{
    //return if no window to draw in
    if (pRenderTarget == NULL)
        return;
    //getting the size of the render Window
    D2D1_SIZE_F size = pRenderTarget->GetSize();
    //simple calculations of the center of the window
    const float x = size.width / 2;
    const float y = size.height / 2;
    //getting the radius to be the smaller of the x;y values to always be as big as possible while staying fuly in the circle
    const float radius = min(x, y);
    //actualy edditing the ellipse variable to get all the params i just calculated
    ellipse = D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius);

}

//a method to set all the resources we need when drawing
HRESULT Window::CreateGraphicsResources()
{
    //a simple HRESULT variable to keep track if the steps are finished correctly
    HRESULT hr = S_OK;
    //only do this if needed ofc
    if (pRenderTarget == NULL)
    {
        RECT rc;
        //retrieving the rect size of the render Window
        GetClientRect(m_hwnd, &rc);

        //getting only the actual size bc the position is not needed
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        //actualy create all the resources we need
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget
        );
        //only if finished correctly
        if (SUCCEEDED(hr))
        {
            // set the color in this case i tried to recreate #1FC8C8 for a nice teal color
            const D2D1_COLOR_F color = D2D1::ColorF(0.12f, 0.78f, 0.78f);
            //actualy make the solid brush for drawing
            hr = pRenderTarget->CreateSolidColorBrush(color,&pBrush);
            if(SUCCEEDED(hr))
                //finish by creating the layout of the graphics
                CalculateLayout();
        }
    }
    return hr;
}

//method used on the closing to not create momory leaks
void Window::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

//method called when repainting the window
void Window::OnPaint()
{
    //create the resources for graphics
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        //paintstruct to keep handle of all the information
        PAINTSTRUCT ps;
        //prepare the window to be painted and fill ps with the default info for painting
        BeginPaint(m_hwnd, &ps);

        //initiate the drawing process
        pRenderTarget->BeginDraw();

        //clear the graphisc with default black to not have old frames vissible(for some reason it works without this but it will stay here LMAO)
        pRenderTarget->Clear();
        //actualy draw the ellipse(in this context its a circle)
        pRenderTarget->FillEllipse(ellipse, pBrush);

        //finish the drawing process
        hr = pRenderTarget->EndDraw();

        //check if the process failed and handle appropriately
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            DiscardGraphicsResources();
        //this one has to be called after each call to BeginPaint() but the code will work without it (Again xDD)
        EndPaint(m_hwnd, &ps);
    }
}

//method to handle the resizing of the window
void Window::Resize()
{
    //if nothing to actualy resize return
    if (pRenderTarget == NULL)
        return;
    RECT rt;
    //get the new window size and store it to rt variable
    GetClientRect(m_hwnd, &rt);
    //get just the size bc there is no need for the position now
    D2D1_SIZE_U size = D2D1::SizeU(rt.right, rt.bottom);

    //resize the render target
    pRenderTarget->Resize(size);
    //recalculate the graphisc layout
    CalculateLayout();

    //update the window
    InvalidateRect(m_hwnd, NULL, FALSE);
}

//method for actualy handling events
LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        //fill the resources with some values
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        //kinda self explainatory
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        //also self explainatory
        OnPaint();
        return 0;



    case WM_SIZE:
        //same here 
        Resize();
        return 0;
    }
    //"Calls the default window procedure to provide default processing for any window messages that an application does not process"
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}