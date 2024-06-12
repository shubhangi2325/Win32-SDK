#include<Windows.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define MAX_CHAR 30
#define ROW_NUM 63
#define COL_NUM 10

LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdline,int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window");
    static TCHAR szWindowTitle[] = TEXT("MSTC 04 :: Painting Demo ");

    HWND hwnd = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd,sizeof(WNDCLASSEX));
    ZeroMemory(&msg,sizeof(MSG));

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor((HINSTANCE)NULL,IDC_ARROW);
    wnd.hIcon = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    wnd.hIconSm = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    wnd.lpfnWndProc = WndProc;
    wnd.hInstance = hInstance;
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = szClassName;
    wnd.style = CS_HREDRAW | CS_VREDRAW;

    if(!RegisterClassEx(&wnd))
    {
        MessageBox( 
            (HWND)NULL,
            TEXT("failed to register a window class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST
            );

        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION |WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (HWND)NULL,
        (HMENU)NULL,
        hInstance,
        (LPVOID)NULL
    );

    if(hwnd == NULL)
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed to create window"),
            TEXT("CreateWindowEx"),
            MB_ICONERROR | MB_TOPMOST
        );

        ExitProcess(EXIT_FAILURE);    
    }

    ShowWindow(hwnd,nShowCmd);
    UpdateWindow(hwnd);

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    static int cxScreen;
    static int cyScreen;

    static int cxChar;
    static int cyChar;

    HDC hdc = NULL;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    static TCHAR szStringName[MAX_CHAR];

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);
            GetTextMetrics(hdc,&tm);
            ReleaseDC(hwnd,hdc);
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            break;

        case WM_SIZE:
            cxScreen = LOWORD(lParam);
            cyScreen = HIWORD(lParam);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps);
            for(int iRow_Count = 0; iRow_Count < ROW_NUM; ++iRow_Count)
            {
                for(int iCol_Count = 0 ;  iCol_Count < COL_NUM; ++iCol_Count)
                {
                    int iLenght = wsprintf(szStringName,TEXT("Row = %d Column = %d"),iRow_Count+1,iCol_Count+1);
                    TextOut(
                        hdc,
                        cxChar * (  MAX_CHAR * iCol_Count),
                        cyChar * iRow_Count,
                        szStringName,
                        iLenght
                    );
                }
            }
            EndPaint(hwnd,&ps);     
            break;  

        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            break;    
    }

    return(DefWindowProc(hwnd,uMsg,wParam,lParam));

}
