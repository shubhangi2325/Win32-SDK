#include <Windows.h>
#include "HashQueue.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_EDIT 1
#define ID_EDIT_2 3
#define ID_BUTTON 2
#define ID_EDIT_3 4
#define ID_EDIT_4 5
#define ID_SEARCH_BUTTON 6
#define ID_DELETE_BUTTON 7
#define TEXT_SIZE 100

HWND ghwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void show_contact(hash_queue_t *p_hash_queue);
void file_handling_write(hash_queue_t *p_hash_queue);
void file_handling_read(hash_queue_t *p_hash_queue);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Standard Window");
    static TCHAR szWindowTitle[] = TEXT("CONTACT APP:");

    HWND hwnd = NULL;
    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hInstance = hInstance;
    wnd.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
    wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CONTACT));
    wnd.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CONTACT));
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;

    wnd.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wnd))
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed To Register A Window Class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST);
        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
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
            TEXT("Failed To Create Window"),
            TEXT("CreateWindowEx"),
            MB_ICONERROR | MB_TOPMOST);
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

int cyChar, cxChar;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    static int cxScreen, cyScreen;
    static HBRUSH hBrush;
    static HPEN hPen1;
    static HFONT hFont;
    static hash_queue_t *contact_list = NULL;

    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    HDC hdc = NULL;

    static HWND hwndEdit;
    static HWND hwndEdit_2;
    static HWND hwndEdit_3;
    static HWND hwndEdit_4;
    HWND hwndButton;
    HWND hSearchButton;
    HWND hDeleteButton;

    switch (msg)
    {

    case WM_CREATE:
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        ReleaseDC(hwnd, hdc);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;

        contact_list = create_hash_queue(BUCKET_SIZE);
        file_handling_read(contact_list);

        hwndEdit = CreateWindowW(L"Edit", L"Name",
                                 WS_CHILD | WS_VISIBLE | WS_BORDER,
                                 50, 60, 200, 25, hwnd, (HMENU)ID_EDIT,
                                 NULL, NULL);

        hwndEdit_2 = CreateWindowW(L"Edit", L"Number",
                                   WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   50, 110, 200, 25, hwnd, (HMENU)ID_EDIT_2,
                                   NULL, NULL);

        hwndButton = CreateWindowW(L"button", L"ADD CONTACT",
                                   WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 160, 120, 25,
                                   hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        hwndEdit_3 = CreateWindowW(L"Edit", NULL,
                                   WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   50, 250, 200, 25, hwnd, (HMENU)ID_EDIT_3,
                                   NULL, NULL);

        hSearchButton = CreateWindowW(L"button", L"SEARCH",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 300, 120, 25,
                                      hwnd, (HMENU)ID_SEARCH_BUTTON, NULL, NULL);

        hwndEdit_4 = CreateWindowW(L"Edit", NULL,
                                   WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   50, 370, 200, 25, hwnd, (HMENU)ID_EDIT_4,
                                   NULL, NULL);

        hDeleteButton = CreateWindowW(L"button", L"DELETE",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 420, 120, 25,
                                      hwnd, (HMENU)ID_DELETE_BUTTON, NULL, NULL);

        break;

    case WM_SIZE:

        cxScreen = LOWORD(lParam);
        cyScreen = HIWORD(lParam);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        hBrush = CreateSolidBrush(RGB(159, 213, 227));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 0, cxScreen, cyScreen);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(33, 100, 117));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 0, 0, 300, cyScreen);
        DeleteObject(hBrush);

        hBrush = CreateSolidBrush(RGB(0, 255, 255));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 40, 50, 260, 195);
        Rectangle(hdc, 40, 240, 260, 335);
        Rectangle(hdc, 40, 360, 260, 455);
        DeleteObject(hBrush);

        hFont = CreateFontW(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
        SelectObject(hdc, hFont);
        SetBkColor(hdc, RGB(7, 227, 190));
        TextOut(hdc, cxScreen / 2, 4, TEXT("CONTACT LIST"), strlen(TEXT("CONTACT LIST")));
        DeleteObject(hFont);

        show_contact(contact_list);

        EndPaint(hwnd, &ps);
        break;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case ID_BUTTON:
            int len_1 = GetWindowTextLength(hwndEdit) + 1;
            char name[TEXT_SIZE];
            GetWindowText(hwndEdit, name, len_1);

            int len_2 = GetWindowTextLength(hwndEdit_2) + 1;
            char number[TEXT_SIZE];
            GetWindowText(hwndEdit_2, number, len_2);
            add_key(contact_list, number, name);
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case ID_SEARCH_BUTTON:
            int len_3 = GetWindowTextLength(hwndEdit_3) + 1;
            char search[TEXT_SIZE];
            GetWindowText(hwndEdit_3, search, len_3);
            search_key(contact_list, search);
            break;

        case ID_DELETE_BUTTON:
            int len_4 = GetWindowTextLength(hwndEdit_4) + 1;
            char delete[TEXT_SIZE];
            GetWindowText(hwndEdit_4, delete, len_4);
            remove_key(contact_list, delete);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        break;

    case WM_DESTROY:
        file_handling_write(contact_list);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void file_handling_write(hash_queue_t *p_hash_queue)
{
    index_t index;
    node_t *run = NULL;
    static TCHAR szstring[TEXT_SIZE];
    FILE *fp_contact_write = NULL;

    fp_contact_write = fopen("contacts.txt", "w");
    if (fp_contact_write == NULL)
    {
        printf("Error in opening file: contacts.text\n");
        exit(EXIT_FAILURE);
    }

    node_t *p_List = NULL;
    for (index = 0; index < p_hash_queue->bucket_size; ++index)
    {
        p_List = p_hash_queue->pp_list_head_array[index];
        for (run = p_List->next; run != p_List; run = run->next)
        {
            fprintf(fp_contact_write, "%s: %s\n", run->name, run->number);
        }
    }

    fclose(fp_contact_write);
    fp_contact_write = NULL;
}

TCHAR buffer[TEXT_SIZE];


void file_handling_read(hash_queue_t *p_hash_queue)
{
    char name[32];
    char number[32];
    char *token = NULL;
    char buffer_copy[TEXT_SIZE];
    int i = 0;
    FILE *fp_contact_read = NULL;

    fp_contact_read = fopen("contacts.txt", "r");
    if (fp_contact_read == NULL)
    {
        printf("Error in opening file: contacts.text\n");
        return;
    }

    while (fgets(buffer, TEXT_SIZE, fp_contact_read) != NULL)
    {
        token = strtok(buffer,":");
        strcpy(name,token);

        token = strtok(NULL,": \n");
        strcpy(number,token);
        
        add_key(p_hash_queue, number, name);
        memset(buffer, 0, TEXT_SIZE);
    }
  
    fclose(fp_contact_read);
    fp_contact_read = NULL;
    InvalidateRect(ghwnd, NULL, TRUE);
}

void show_contact(hash_queue_t *p_hash_queue)
{
    index_t index;
    node_t *run = NULL;
    int iCnt = 0;
    int iLength;

    static TCHAR szstring[TEXT_SIZE];
    static TCHAR szContact[] = TEXT("CONTACT LIST");

    HDC hdc;
    hdc = GetDC(ghwnd);
    static HFONT hFont2;
    hFont2 = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, L"decorative");
    SelectObject(hdc, hFont2);
    SetBkColor(hdc, RGB(159, 213, 227));
    SetTextColor(hdc, RGB(117, 33, 78));

    node_t *p_List = NULL;
    for (index = 0; index < p_hash_queue->bucket_size; ++index)
    {
        p_List = p_hash_queue->pp_list_head_array[index];
        for (run = p_List->next; run != p_List; run = run->next)
        {
            ++iCnt;
            iLength = wsprintf(szstring, TEXT("%s:  %s"), run->name, run->number);
            TextOut(
                hdc,
                305,
                2 * cyChar * iCnt,
                szstring,
                iLength);
        }
    }
    DeleteObject(hFont2);
}