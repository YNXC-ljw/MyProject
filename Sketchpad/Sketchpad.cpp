#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK MainWndProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize the window. 
        return 0;
    case WM_PAINT:
        // Paint the window's client area. 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 所有绘制操作都在这里进行

        EndPaint(hwnd, &ps);
        return 0;

    case WM_SIZE:
        // Set the size and position of the window. 
        return 0;

    case WM_DESTROY:
        // Clean up window-specific data objects.
        PostQuitMessage(0);
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

        // 
        // Process other messages. 
        // 

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nShowCmd)
{
    // 1.注册窗口类
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_SAVEBITS | CS_DROPSHADOW;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"wei";

    RegisterClassEx(&wc);
    // 2.创建窗口
    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"画板",
        WS_OVERLAPPEDWINDOW,
        300,200,800,600,
        NULL,NULL,
        hInstance,
        NULL
    );
    if (hwnd == NULL)
    {
        DWORD err = GetLastError();

        wchar_t buf[256];
        wsprintf(buf, L"创建失败，错误码:%d", err);

        MessageBox(NULL, buf, L"Error", MB_OK);

        return 0;
    }
    // 3.显示窗口
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);
    // 4.消息处理
    MSG msg;
    bool Rst;
    while ((Rst = GetMessage(&msg, NULL, 0, 0)) != 0)// 鼠标点击--产生消息--程序处理
    {
        if (Rst == -1)
        {
            GetLastError();
            return 0;
        }
        else
        {
            // 等待消息
            TranslateMessage(&msg);
            // 处理消息        
            DispatchMessage(&msg);
        }
    }
	return 0;
}









//HMENU hMenu = CreateMenu();
//HMENU hSubMenu = CreatePopupMenu();
//HMENU hhSubMenu = CreatePopupMenu();
//HMENU hhhSubMenu = CreatePopupMenu();
//
//AppendMenu(hhSubMenu, MF_STRING, 10, L"保存");
//AppendMenu(hhSubMenu, MF_STRING, 11, L"打开");
//AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hhSubMenu, L"文件");