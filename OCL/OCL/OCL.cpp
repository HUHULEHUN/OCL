#include <windows.h>
#include <cstdio>

// 全局变量
HWND g_StatusLabel = NULL;
int g_CurrentPanelId = 1; // 1:下载 2:开始 3:账户

// 按钮 ID
#define BTN_DOWNLOAD 100
#define BTN_START    101
#define BTN_ACCOUNT  102
#define BTN_DO_DOWNLOAD 103
#define BTN_BEGIN    104
#define BTN_LOGIN    105

// 自定义绘制用
#define WS_VORDER 0x00000002 // 简易占位，实际用默认风格

void ShowPanel(HWND hwnd, int id) {
    g_CurrentPanelId = id;
    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // 左侧侧边栏按钮（模拟 PCL 深色侧边栏）
        CreateWindowA("BUTTON", "下载游戏", WS_VISIBLE | WS_CHILD, 20, 70, 140, 45, hwnd, (HMENU)BTN_DOWNLOAD, NULL, NULL);
        CreateWindowA("BUTTON", "开始游戏", WS_VISIBLE | WS_CHILD, 20, 125, 140, 45, hwnd, (HMENU)BTN_START, NULL, NULL);
        CreateWindowA("BUTTON", "账户", WS_VISIBLE | WS_CHILD, 20, 180, 140, 45, hwnd, (HMENU)BTN_ACCOUNT, NULL, NULL);

        // 默认显示下载面板控件
        CreateWindowA("STATIC", "选择版本：", WS_VISIBLE | WS_CHILD, 200, 80, 100, 25, hwnd, NULL, NULL, NULL);
        CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 280, 80, 150, 100, hwnd, NULL, NULL, NULL);
        CreateWindowA("BUTTON", "开始下载", WS_VISIBLE | WS_CHILD, 260, 130, 120, 40, hwnd, (HMENU)BTN_DO_DOWNLOAD, NULL, NULL);
        g_StatusLabel = CreateWindowA("STATIC", "准备下载...", WS_VISIBLE | WS_CHILD, 200, 190, 300, 25, hwnd, NULL, NULL, NULL);
        break;

    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
        int id = dis->CtlID;
        const char* text = "";
        if (id == BTN_DOWNLOAD) text = "下载游戏";
        if (id == BTN_START) text = "开始游戏";
        if (id == BTN_ACCOUNT) text = "账户";

        bool selected = (g_CurrentPanelId == 1 && id == BTN_DOWNLOAD) ||
            (g_CurrentPanelId == 2 && id == BTN_START) ||
            (g_CurrentPanelId == 3 && id == BTN_ACCOUNT);

        HBRUSH brush = CreateSolidBrush(selected ? RGB(60, 130, 200) : RGB(45, 48, 56));
        FillRect(dis->hDC, &dis->rcItem, brush);
        DeleteObject(brush);

        SetBkMode(dis->hDC, TRANSPARENT);
        SetTextColor(dis->hDC, RGB(255, 255, 255));
        DrawTextA(dis->hDC, text, -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return TRUE;
    }

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        switch (id)
        {
        case BTN_DOWNLOAD:
            ShowPanel(hwnd, 1);
            break;
        case BTN_START:
            ShowPanel(hwnd, 2);
            MessageBoxA(hwnd, "进入开始游戏面板", "提示", MB_OK);
            break;
        case BTN_ACCOUNT:
            ShowPanel(hwnd, 3);
            MessageBoxA(hwnd, "进入账户面板", "提示", MB_OK);
            break;
        case BTN_DO_DOWNLOAD:
        {
            for (int i = 0; i <= 100; i += 10)
            {
                char buf[64];
                sprintf_s(buf, "正在下载... %d%%", i);
                SetWindowTextA(g_StatusLabel, buf);
                Sleep(80);
            }
            SetWindowTextA(g_StatusLabel, "下载完成！（");
            MessageBoxA(hwnd, "游戏下载完成！", "提示", MB_OK | MB_ICONINFORMATION);
            break;
        }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = CreateSolidBrush(RGB(245, 245, 245));
    wc.lpszClassName = "McLauncherPCL";
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowA("McLauncherPCL", "OCL（beta）",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        200, 100, 610, 500, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}