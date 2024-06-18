#include<Windows.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Standard Window");
    static TCHAR szWindowTitle[] = TEXT("SCROLL BAR::");

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
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.hInstance = hInstance;
    wnd.style = CS_VREDRAW | CS_HREDRAW;

    if(!RegisterClassEx(&wnd))
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed To Register A Window Class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST
        );
        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
              WS_EX_APPWINDOW,
              szClassName,
              szWindowTitle,
              WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX |WS_MAXIMIZEBOX |
              WS_VSCROLL | WS_HSCROLL,
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
            TEXT("Failed To Create Window"),
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
    #define MAX_CHAR 38
    #define ROW_NUM 63
    #define COL_NUM 10

    static int cxScreen;
    static int cyScreen;

    static int cxChar;
    static int cyChar;

    static int iHScrollPosition,iVScrollPosition;

    static TCHAR szFormatString[]= TEXT("Row Num = %d,Column Num = %d");
    static TCHAR szTextOut[MAX_CHAR];

    int iRowCount,iColCount,iLength;

    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    HDC hdc;

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

            SetScrollRange(hwnd,SB_VERT,0,(ROW_NUM- cyScreen/cyChar),FALSE);
            SetScrollPos(hwnd,SB_VERT,iVScrollPosition,TRUE);
            iVScrollPosition = max(0,min(iVScrollPosition,(ROW_NUM - cyScreen/cyChar)));
            if(iVScrollPosition != GetScrollPos(hwnd,SB_VERT))
            {
                SetScrollPos(hwnd,SB_VERT,iVScrollPosition,FALSE);
                InvalidateRect(hwnd,NULL,TRUE);
            }
              
            SetScrollRange(hwnd,SB_HORZ,0,(COL_NUM*MAX_CHAR - cxScreen/cxChar),FALSE);
            SetScrollPos(hwnd,SB_HORZ,iHScrollPosition,TRUE);
            iHScrollPosition = max(0,min(iHScrollPosition,(COL_NUM*MAX_CHAR - cxScreen/cxChar)));
            if(iHScrollPosition != GetScrollPos(hwnd,SB_HORZ))
            {
                SetScrollPos(hwnd,SB_HORZ,iHScrollPosition,FALSE);
                InvalidateRect(hwnd,NULL,TRUE);
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps) ;

            for(iRowCount = 0;iRowCount < ROW_NUM;++iRowCount)
            {
                for(iColCount = 0; iColCount < COL_NUM; ++iColCount)
                {
                    iLength = wsprintf(szTextOut,szFormatString,iRowCount,iColCount);
                    TextOut(
                        hdc,
                        (MAX_CHAR * iColCount - iHScrollPosition)*cxChar,
                        (iRowCount - iVScrollPosition)*cyChar,
                        szTextOut,
                        iLength
                    );
                }
            }

            EndPaint(hwnd,&ps);
            break;

         case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
                case SB_LINEUP:
                    iVScrollPosition -= 1;
                    break;

                case SB_LINEDOWN:
                    iVScrollPosition += 1;
                    break;

                case SB_PAGEUP:
                    iVScrollPosition -= cyScreen/cyChar;
                    break;

                case SB_PAGEDOWN:
                    iVScrollPosition += cyScreen/cyChar;
                    break;

                case SB_THUMBPOSITION:
                    iVScrollPosition = HIWORD(wParam);
                    break;                
            }   

            iVScrollPosition = max(0,min(iVScrollPosition,(ROW_NUM - cyScreen/cyChar)));
            if(iVScrollPosition != GetScrollPos(hwnd,SB_VERT))
            {
                SetScrollPos(hwnd,SB_VERT,iVScrollPosition,FALSE);
                InvalidateRect(hwnd,NULL,TRUE);
            }
            break;

         case WM_HSCROLL:
            switch(LOWORD(wParam))
            {
                case SB_LINELEFT:
                    iHScrollPosition -= 1;
                    break;

                case SB_LINERIGHT:
                    iHScrollPosition += 1;
                    break;

                case SB_PAGELEFT:
                    iHScrollPosition -= cxScreen/cxChar;
                    break;

                case SB_PAGERIGHT:
                    iHScrollPosition += cxScreen/cxChar;
                    break;

                case SB_THUMBPOSITION:
                    iHScrollPosition = HIWORD(wParam);
                    break;                
            }   

            iHScrollPosition = max(0,min(iHScrollPosition,(MAX_CHAR * COL_NUM - cxScreen/cxChar)));
            if(iHScrollPosition != GetScrollPos(hwnd,SB_HORZ))
            {
                SetScrollPos(hwnd,SB_HORZ,iHScrollPosition,FALSE);
                InvalidateRect(hwnd,NULL,TRUE);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;    

    }

    return(DefWindowProc(hwnd,uMsg,wParam,lParam));

}