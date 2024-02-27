#include <windows.h>
#include <shobjidl.h>

#include "Window.h"

/*  
      !!! IMPORTANT !!! sry if i misspell sth i mainly write theese comments for myself to remember this better
*/

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //create an instance of the Window class defined in Window.h and Window.cpp files
    Window window;

    //create the window and test if it created
    if (!window.Create(L"Test Name"))
    {
        return -1;
    }

    //show the window
    ShowWindow(window.GetWindow(), nCmdShow);

    //create an empty message
    MSG msg = { };
    //message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
