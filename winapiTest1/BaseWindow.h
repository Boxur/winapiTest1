#pragma once
#include <windows.h>
template <class TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{ 
		TYPE* pThis = NULL;

		//creating the window
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
			pThis = (TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//fail check
		if (pThis)
			return pThis->HandleMessage(uMsg, wParam, lParam);
		else
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	BaseWindow() : m_hwnd(NULL) {}

	BOOL Create(
		PCWSTR lpWindowName,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int width = CW_USEDEFAULT,
		int height = CW_USEDEFAULT,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		DWORD dwExStyle = 0,
		HWND parent = 0,
		HMENU hMenu = 0
		)
	{
		WNDCLASS wc = {0};

		wc.lpfnWndProc = TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();
		
		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, width, height, parent, hMenu, GetModuleHandle(NULL), this);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND GetWindow() const { return m_hwnd; }

protected:
	HWND m_hwnd;

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

