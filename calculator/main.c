#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")

#define ID_ZERO 1
#define ID_ONE 2
#define ID_TWO 3
#define ID_THREE 4
#define ID_FOUR 5
#define ID_FIVE 6
#define ID_SIX 7
#define ID_SEVEN 8
#define ID_EIGTH 9
#define ID_NINE 10
#define ID_PLUS 11
#define ID_MIN 12
#define ID_MUL 13
#define ID_DIV 14
#define ID_MODULO 15
#define ID_DOT 16
#define ID_EQUALE 17
#define ID_UNDO 18
#define ID_CLEAN 19
#define ID_EDIT 20

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void get_expression(char Key);
void display(char Pcalstr[]);
void expression_evaluation(char str[]);
HWND ghwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Standard Window");
    static TCHAR szWindowTitle[] = TEXT("Calculator");

    HWND hwnd = NULL;
    WNDCLASSEX wnd;
    MSG msg;

    static int cxScreen, cyScreen;
    static int cxWndSize, cyWndSize;
    int x, y;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
    wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CAL));
    wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CAL));
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW || CS_VREDRAW;

    if (!RegisterClassEx(&wnd))
    {
        MessageBox((HWND)NULL,
                   TEXT("Failed To Register A Window Class"),
                   TEXT("RegisterClassEx"),
                   MB_OK || MB_TOPMOST);

        ExitProcess(EXIT_FAILURE);
    }

    cxScreen = GetSystemMetrics(SM_CXFULLSCREEN);
    cyScreen = GetSystemMetrics(SM_CYFULLSCREEN);
    cxWndSize = 400;
    cyWndSize = 600;

    x = (cxScreen / 2) - (cxWndSize / 2);
    y = (cyScreen / 2) - (cyWndSize / 2);

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU,
        x,
        y,
        cxWndSize,
        cyWndSize,
        (HWND)NULL,
        (HMENU)NULL,
        hInstance,
        (LPVOID)NULL);

    if (hwnd == NULL)
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed To Create A Window"),
            TEXT("CreateWindowEx"),
            MB_OK | MB_TOPMOST);
        ExitProcess(EXIT_FAILURE);
    }

    ghwnd = hwnd;
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}

char calstr[100];
char answer_str[50];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBrush;
    static HFONT hFont2;
    HDC hdc;
    static int cxScreen, cyScreen;
    char PressKey;
    PAINTSTRUCT ps;
    static char string[100];

    switch (msg)
    {
    case WM_CREATE:
        CreateWindowW(L"Button", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                      4, 2, 370, 228, hwnd, (HMENU)0,
                      NULL, NULL);

        // Buttons Code
        CreateWindowW(L"button", L"C", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      20, 250, 80, 50, hwnd, (HMENU)ID_CLEAN, NULL, NULL);

        CreateWindowW(L"button", L"<-", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      110, 250, 80, 50, hwnd, (HMENU)ID_UNDO, NULL, NULL);

        CreateWindowW(L"button", L"%", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      200, 250, 80, 50, hwnd, (HMENU)ID_MODULO, NULL, NULL);

        CreateWindowW(L"button", L"\\", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      290, 250, 80, 50, hwnd, (HMENU)ID_DIV, NULL, NULL);

        CreateWindowW(L"button", L"7", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      20, 310, 80, 50, hwnd, (HMENU)ID_SEVEN, NULL, NULL);

        CreateWindowW(L"button", L"8", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      110, 310, 80, 50, hwnd, (HMENU)ID_EIGTH, NULL, NULL);

        CreateWindowW(L"button", L"9", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      200, 310, 80, 50, hwnd, (HMENU)ID_NINE, NULL, NULL);

        CreateWindowW(L"button", L"x", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      290, 310, 80, 50, hwnd, (HMENU)ID_MUL, NULL, NULL);

        CreateWindowW(L"button", L"4", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      20, 370, 80, 50, hwnd, (HMENU)ID_FOUR, NULL, NULL);

        CreateWindowW(L"button", L"5", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      110, 370, 80, 50, hwnd, (HMENU)ID_FIVE, NULL, NULL);

        CreateWindowW(L"button", L"6", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      200, 370, 80, 50, hwnd, (HMENU)ID_SIX, NULL, NULL);

        CreateWindowW(L"button", L"-", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      290, 370, 80, 50, hwnd, (HMENU)ID_MIN, NULL, NULL);

        CreateWindowW(L"button", L"1", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      20, 430, 80, 50, hwnd, (HMENU)ID_ONE, NULL, NULL);

        CreateWindowW(L"button", L"2", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      110, 430, 80, 50, hwnd, (HMENU)ID_TWO, NULL, NULL);

        CreateWindowW(L"button", L"3", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      200, 430, 80, 50, hwnd, (HMENU)ID_THREE, NULL, NULL);

        CreateWindowW(L"button", L"+", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      290, 430, 80, 50, hwnd, (HMENU)ID_PLUS, NULL, NULL);

        CreateWindowW(L"button", L"0", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      110, 490, 80, 50, hwnd, (HMENU)ID_ZERO, NULL, NULL);

        CreateWindowW(L"button", L".", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      200, 490, 80, 50, hwnd, (HMENU)ID_DOT, NULL, NULL);

        CreateWindowW(L"button", L"=", WS_CHILD | WS_VISIBLE | WS_BORDER,
                      290, 490, 80, 50, hwnd, (HMENU)ID_EQUALE, NULL, NULL);

        break;

    case WM_SIZE:
        cxScreen = LOWORD(lParam);
        cyScreen = HIWORD(lParam);
        break;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case ID_ZERO:
            PressKey = '0';
            get_expression(PressKey);
            break;

        case ID_ONE:
            PressKey = '1';
            get_expression(PressKey);
            break;

        case ID_TWO:
            PressKey = '2';
            get_expression(PressKey);
            break;

        case ID_THREE:
            PressKey = '3';
            get_expression(PressKey);
            break;

        case ID_FOUR:
            PressKey = '4';
            get_expression(PressKey);
            break;
            ;

        case ID_FIVE:
            PressKey = '5';
            get_expression(PressKey);
            break;

        case ID_SIX:
            PressKey = '6';
            get_expression(PressKey);
            break;

        case ID_SEVEN:
            PressKey = '7';
            get_expression(PressKey);
            break;

        case ID_EIGTH:
            PressKey = '8';
            get_expression(PressKey);
            break;

        case ID_NINE:
            PressKey = '9';
            get_expression(PressKey);
            break;

        case ID_PLUS:
            PressKey = '+';
            get_expression(PressKey);
            break;

        case ID_MIN:
            PressKey = '-';
            get_expression(PressKey);
            break;

        case ID_MUL:
            PressKey = '*';
            get_expression(PressKey);
            break;

        case ID_DIV:
            PressKey = '/';
            get_expression(PressKey);
            break;

        case ID_MODULO:
            PressKey = '%';
            get_expression(PressKey);
            break;

        case ID_DOT:
            PressKey = '.';
            get_expression(PressKey);
            break;

        case ID_EQUALE:
            PressKey = '=';
            get_expression(PressKey);
            break;

        case ID_UNDO:
            PressKey = '<';
            get_expression(PressKey);
            break;

        case ID_CLEAN:
            PressKey = 'c';
            get_expression(PressKey);
            break;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        hBrush = CreateSolidBrush(RGB(46, 9, 94));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 0, cxScreen, cyScreen);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(17, 17, 18));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 4, 2, 370, 228);
        DeleteObject(hBrush);

        display(calstr);
        display(answer_str);

        for (int j = strlen(answer_str); j > 0; j--)
        {
            answer_str[j - 1] = answer_str[j];
        }

        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void get_expression(char Key)
{

    static int i = 0;
    static int float_point = 0;

    if (Key >= '0' && Key <= '9')
    {
        calstr[i] = Key;
        i++;
    }
    else if (Key == '<')
    {

        if (calstr[i - 1] == '.')
            float_point = 0;

        calstr[i - 1] = '\0';
        i--;

        InvalidateRect(ghwnd, NULL, TRUE);
    }
    else if (Key == '.' && float_point == 0)
    {
        float_point = 1;
        calstr[i] = Key;
        i++;
    }
    else if (Key == '+' || Key == '-' || Key == '*' || Key == '/' || Key == '%')
    {
        float_point = 0;

        switch (calstr[i - 1])
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            calstr[i - 1] = Key;
            break;

        case '.':
            calstr[i++] = '0';
            calstr[i++] = Key;
            break;

        default:
            calstr[i] = Key;
            i++;
            break;
        }
    }
    else if (Key == 'c')
    {
        for (int k = i; k > 0; k--)
        {
            calstr[k - 1] = calstr[k];
        }

        i = 0;
        float_point = 0;
        InvalidateRect(ghwnd, NULL, TRUE);
    }

    if (Key == '=')
    {
        if (calstr[i - 1] == '+' || calstr[i - 1] == '-' || calstr[i - 1] == '*' || calstr[i - 1] == '/' || calstr[i - 1] == '%')
        {
            calstr[i - 1] = '\0';
            i--;
            InvalidateRect(ghwnd, NULL, TRUE);
            MessageBox(ghwnd, TEXT("Please Enter Valid Expression"),
                       TEXT("ERROR"),
                       MB_TOPMOST | MB_OK);
        }
        expression_evaluation(calstr);
        SendMessage(ghwnd, WM_COMMAND, ID_CLEAN, 0);
    }

    display(calstr);
}
void display(char *Pcalstr)
{
    HDC hdc;
    static HFONT hFont2;

    hdc = GetDC(ghwnd);
    hFont2 = CreateFontW(23, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, L"decorative");
    SelectObject(hdc, hFont2);
    SetBkColor(hdc, RGB(17, 17, 18));
    SetTextColor(hdc, RGB(241, 240, 245));
    SetTextAlign(hdc, TA_BASELINE | TA_RIGHT);
    TextOut(hdc, 370, 220, Pcalstr, strlen(Pcalstr));
    DeleteDC(hdc);
}

void expression_evaluation(char str[])
{
    float num;
    char op[] = "+-*/%";
    char nums[] = "0123456789.";
    char *tok = NULL;
    char operator_list[10];
    float num_list[10];
    int i = 0;

    char str_copy[100];
    strcpy(str_copy, str);

    tok = strtok(str, op);
    while (tok != NULL)
    {
        num = atof(tok);
        num_list[i] = num;
        tok = strtok(NULL, op);
        i++;
    }

    i = 0;
    tok = strtok(str_copy, nums);
    while (tok != NULL)
    {
        operator_list[i] = *tok;
        tok = strtok(NULL, nums);
        i++;
    }

    float number;
    float answer = num_list[0];

    for (int k = 0; k < strlen(operator_list); ++k)
    {
        number = num_list[k + 1];

        switch (operator_list[k])
        {
        case '+':
            answer += number;
            break;

        case '-':
            answer -= number;
            break;

        case '/':
            answer /= number;
            break;

        case '*':
            answer *= number;
            break;

        case '%':
            (int)answer %= (int)number;
            break;
        }
    }

    sprintf(answer_str, TEXT("%.4f"), answer);
}
