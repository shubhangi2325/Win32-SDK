#include <Windows.h>
#include <Windowsx.h>
#include "resource.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

// SYMBOLIC CONSTANT
#define IDM_RECT 1
#define IDM_CIRCLE 2
#define IDM_ROUNDRECT 3
#define IDM_LINE 4
#define IDM_ERRASE 5
#define IDM_UNDO 14

#define ID_RED 6
#define ID_BLUE 7
#define ID_BLACK 8
#define ID_YELLOW 9
#define ID_GREEN 10
#define ID_ORANGE 11
#define ID_PINK 12
#define ID_PURPLE 13

// VECTOR DECLARATION
typedef long long ssize_t;

typedef struct PointData
{
    POINT Point;
    int Status;
    HPEN hPen;
} POINTDATA, *PPOINTDATA;

typedef struct tagVectorOfPoints
{
    PPOINTDATA pPointData;
    ssize_t N;
} POINTVECTOR, *PPOINTVECTOR;

PPOINTVECTOR CreateVector(VOID);
void PushBack(PPOINTVECTOR pPointVec, LONG x, LONG y, int Status, HPEN hpen);
void DestroyVector(PPOINTVECTOR pPointVec);
//---------------------------------------

HWND ghwnd = NULL;
HINSTANCE ghIns;

// MENUBAR FUN DECLARATION
void AddMenu(HWND);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window");
    static TCHAR szWindowTitle[] = TEXT("DRAWING APP");

    HWND hwnd = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
    wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DRAWING));
    wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DRAWING));
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    if (!RegisterClassEx(&wnd))
    {
        MessageBox(
            NULL,
            TEXT("Failed to register a window class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST);
        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (HWND)NULL,
        (HMENU)NULL,
        hInstance,
        (LPVOID)NULL);

    if (hwnd == NULL)
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed to create a window"),
            TEXT("CreateWindowEx"),
            MB_ICONERROR | MB_TOPMOST);
        ExitProcess(EXIT_FAILURE);
    }

    ghwnd = hwnd;
    ghIns = hInstance;

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxChar;
    static int cyChar;
    static int cxScreen;
    static int cyScreen;

    static int iTopX, iTopY;
    static int iBottomX, iBottomY;

    static int iStatus;
    int Index;
    BOOL Flag;

    static PPOINTVECTOR pPointVector = NULL;

    LPDRAWITEMSTRUCT pdis;

    HBRUSH hBrush = NULL;
    HBRUSH hBrush2;
    static HWND hButton1;

    static HPEN hPen;
    HDC hdc = NULL;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    switch (uMsg)
    {
    case WM_CREATE:
        AddMenu(hwnd);
        pPointVector = CreateVector();
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        ReleaseDC(hwnd, hdc);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;

        CreateWindowW(L"Button", L"RED", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 20, 30, 30, hwnd, (HMENU)ID_RED, ghIns, NULL);
        CreateWindowW(L"Button", L"BLUE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 70, 30, 30, hwnd, (HMENU)ID_BLUE, NULL, NULL);
        CreateWindowW(L"Button", L"BLACK", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 120, 30, 30, hwnd, (HMENU)ID_BLACK, NULL, NULL);
        CreateWindowW(L"Button", L"YELLOW", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 170, 30, 30, hwnd, (HMENU)ID_YELLOW, NULL, NULL);
        CreateWindowW(L"Button", L"GREEN", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 220, 30, 30, hwnd, (HMENU)ID_GREEN, ghIns, NULL);
        CreateWindowW(L"Button", L"ORANGE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 270, 30, 30, hwnd, (HMENU)ID_ORANGE, NULL, NULL);
        CreateWindowW(L"Button", L"PINK", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 320, 30, 30, hwnd, (HMENU)ID_PINK, NULL, NULL);
        CreateWindowW(L"Button", L"PURPLE", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 20, 370, 30, 30, hwnd, (HMENU)ID_PURPLE, NULL, NULL);

        break;

    case WM_SIZE:
        cxScreen = LOWORD(lParam);
        cyScreen = HIWORD(lParam);
        break;

    case WM_DRAWITEM:

        switch ((UINT)wParam)
        {
        case ID_RED:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(242, 7, 15));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_BLUE:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(4, 74, 224));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_BLACK:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(13, 13, 13));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_YELLOW:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(225, 250, 5));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_GREEN:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(35, 209, 19));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_ORANGE:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(224, 129, 4));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_PINK:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(245, 10, 190));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;

        case ID_PURPLE:
            pdis = (LPDRAWITEMSTRUCT)lParam;
            hBrush2 = CreateSolidBrush(RGB(116, 22, 122));
            FillRect(pdis->hDC, &pdis->rcItem, hBrush2);
            DeleteObject(hBrush2);
            break;
        }

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_RECT:
            iStatus = IDM_RECT;
            break;

        case IDM_CIRCLE:
            iStatus = IDM_CIRCLE;
            break;

        case IDM_ROUNDRECT:
            iStatus = IDM_ROUNDRECT;
            break;

        case IDM_LINE:
            iStatus = IDM_LINE;
            break;

        case IDM_ERRASE:
            pPointVector->N = 0;
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case IDM_UNDO:
            pPointVector->N -= 2;
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case ID_RED:
            hPen = CreatePen(PS_SOLID, 2, RGB(242, 7, 15));
            break;

        case ID_BLUE:
            hPen = CreatePen(PS_SOLID, 2, RGB(4, 74, 224));
            break;

        case ID_BLACK:
            hPen = CreatePen(PS_SOLID, 2, RGB(13, 13, 13));
            break;

        case ID_YELLOW:
            hPen = CreatePen(PS_SOLID, 2, RGB(225, 250, 5));
            break;

        case ID_GREEN:
            hPen = CreatePen(PS_SOLID, 2, RGB(35, 209, 19));
            break;

        case ID_ORANGE:
            hPen = CreatePen(PS_SOLID, 2, RGB(224, 129, 4));
            break;

        case ID_PINK:
            hPen = CreatePen(PS_SOLID, 2, RGB(245, 10, 190));
            break;

        case ID_PURPLE:
            hPen = CreatePen(PS_SOLID, 2, RGB(116, 22, 122));
            break;
        }
        break;

    case WM_LBUTTONDOWN:
        iTopX = GET_X_LPARAM(lParam);
        iTopY = GET_Y_LPARAM(lParam);
        PushBack(pPointVector, iTopX, iTopY, iStatus, hPen);
        break;

    case WM_LBUTTONUP:
        iBottomX = GET_X_LPARAM(lParam);
        iBottomY = GET_Y_LPARAM(lParam);
        PushBack(pPointVector, iBottomX, iBottomY, iStatus, hPen);
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        hBrush = CreateSolidBrush(RGB(217, 214, 139));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 70, cyChar, cxScreen - cxChar, cyScreen - cyChar);
        DeleteObject(hBrush);

        Index = 0;
        while (Index < pPointVector->N)
        {
            switch (pPointVector->pPointData[Index].Status)
            {
            case IDM_RECT:
                SelectObject(hdc, pPointVector->pPointData[Index].hPen);
                Rectangle(hdc, pPointVector->pPointData[Index].Point.x, pPointVector->pPointData[Index].Point.y, pPointVector->pPointData[Index + 1].Point.x, pPointVector->pPointData[Index + 1].Point.y);

                break;

            case IDM_ROUNDRECT:
                SelectObject(hdc, pPointVector->pPointData[Index].hPen);
                RoundRect(hdc, pPointVector->pPointData[Index].Point.x, pPointVector->pPointData[Index].Point.y, pPointVector->pPointData[Index + 1].Point.x, pPointVector->pPointData[Index + 1].Point.y, (pPointVector->pPointData[Index + 1].Point.x - pPointVector->pPointData[Index].Point.x) / 4, (pPointVector->pPointData[Index + 1].Point.x - pPointVector->pPointData[Index].Point.x) / 4);

                break;

            case IDM_CIRCLE:
                SelectObject(hdc, pPointVector->pPointData[Index].hPen);
                Ellipse(hdc, pPointVector->pPointData[Index].Point.x, pPointVector->pPointData[Index].Point.y, pPointVector->pPointData[Index + 1].Point.x, pPointVector->pPointData[Index + 1].Point.y);

                break;

            case IDM_LINE:
                SelectObject(hdc, pPointVector->pPointData[Index].hPen);
                MoveToEx(hdc, pPointVector->pPointData[Index].Point.x, pPointVector->pPointData[Index].Point.y, NULL);
                LineTo(hdc, pPointVector->pPointData[Index + 1].Point.x, pPointVector->pPointData[Index + 1].Point.y);

                break;
            }

            Index = Index + 2;
        }

        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        DeleteObject(hBrush);
        DeleteObject(hPen);
        DestroyVector(pPointVector);
        pPointVector = NULL;
        PostQuitMessage(EXIT_SUCCESS);
        break;
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

// MenuBar CODE
void AddMenu(HWND hwnd)
{
    HMENU hMenuBar;
    hMenuBar = CreateMenu();

    AppendMenuW(hMenuBar, MF_STRING, IDM_RECT, L"&RECTANGLE");
    AppendMenuW(hMenuBar, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuBar, MF_STRING, IDM_CIRCLE, L"&CIRCLE");
    AppendMenuW(hMenuBar, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuBar, MF_STRING, IDM_ROUNDRECT, L"&ROUNDRECT");
    AppendMenuW(hMenuBar, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuBar, MF_STRING, IDM_LINE, L"&LINE");
    AppendMenuW(hMenuBar, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuBar, MF_STRING, IDM_ERRASE, L"&ERRASE");
    AppendMenuW(hMenuBar, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenuBar, MF_STRING, IDM_UNDO, L"&UNDO");

    SetMenu(hwnd, hMenuBar);
}

// Vector CODE
PPOINTVECTOR CreateVector(VOID)
{
    PPOINTVECTOR pPointVector = NULL;

    pPointVector = (PPOINTVECTOR)malloc(sizeof(POINTVECTOR));
    if (pPointVector == NULL)
        DestroyWindow(ghwnd);

    pPointVector->pPointData = NULL;
    pPointVector->N = 0;

    return (pPointVector);
}

VOID PushBack(PPOINTVECTOR pPointVec, LONG x, LONG y, int Status, HPEN hPen)
{
    pPointVec->pPointData = (PPOINTDATA)realloc(pPointVec->pPointData,
                                                (pPointVec->N + 1) * sizeof(POINTDATA));
    if (pPointVec->pPointData == NULL)
        DestroyWindow(ghwnd);

    pPointVec->N += 1;
    pPointVec->pPointData[pPointVec->N - 1].Point.x = x;
    pPointVec->pPointData[pPointVec->N - 1].Point.y = y;
    pPointVec->pPointData[pPointVec->N - 1].Status = Status;
    pPointVec->pPointData[pPointVec->N - 1].hPen = hPen;
}

VOID DestroyVector(PPOINTVECTOR pPointVec)
{
    free(pPointVec->pPointData);
    free(pPointVec);
}