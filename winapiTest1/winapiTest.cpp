#include <windows.h>
#include <shobjidl.h>

#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Window window;

    if (!window.Create(L"Test Name"))
    {
        return -1;
    }

    ShowWindow(window.GetWindow(), nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
