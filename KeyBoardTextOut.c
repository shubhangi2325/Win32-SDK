#include<Windows.h>
#include<stdlib.h>
#include<string.h>
#include<tchar.h>


#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")

HWND ghwnd = NULL;

typedef long long ssize_t;

typedef struct tagVectorOfString{
    char** ppString;
    ssize_t N;
}STRINGVECTOR,*PSTRINGVECTOR;

PSTRINGVECTOR CreateVector(VOID);
void PushBack(PSTRINGVECTOR pStringVec, char* pString);
void DestroyVector(PSTRINGVECTOR pStringVec);


LRESULT CALLBACK WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance , HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window");
    static TCHAR szWindowTitle[] = TEXT("DEMO: Taking Char From Keyboard Printing Textout With Scrollbar");

    HWND hwnd = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor((HINSTANCE)NULL,IDC_ARROW);
    wnd.hIcon = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    wnd.hIconSm = LoadIcon((HINSTANCE)NULL,IDI_APPLICATION);
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_VREDRAW | CS_HREDRAW;

    if(!RegisterClassEx(&wnd))
    {
        MessageBox(
            (HWND)NULL,
            TEXT("Failed TO Register a Window Class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST

        );
        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION 
        | WS_MINIMIZEBOX |WS_MAXIMIZEBOX | WS_SYSMENU 
        |WS_HSCROLL |WS_VSCROLL,
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
            TEXT("Failed Create An Application Window"),
            TEXT("CreateWindowEx"),
            MB_ICONERROR | MB_TOPMOST
        );
        ExitProcess(EXIT_FAILURE);
    }

    ghwnd = hwnd;

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

    #define MAX_CHAR 32

    static int cxChar,cyChar;
    static int cyScreen,cxScreen;
      
    static PSTRINGVECTOR pStringVec = NULL;

    static TCHAR szString[] = TEXT("Entered CHAR Is = %c");
    static TCHAR szStringTextOut[MAX_CHAR];
    static int iLength; 
    int iIndex;
    static int iHScrollPosition,iVScrollPosition;
    int iPaintBeginVertical, iPaintEndVertical;

    HDC hdc = NULL;
    TEXTMETRIC tm;
    PAINTSTRUCT ps;
    SCROLLINFO si; 

    switch(uMsg)
    {
        case WM_CREATE: 
            pStringVec = CreateVector();
            hdc = GetDC(hwnd); 
            GetTextMetrics(hdc, &tm); 
            ReleaseDC(hwnd, hdc); 
            cxChar = tm.tmAveCharWidth; 
            cyChar = tm.tmHeight + tm.tmExternalLeading; 
            break; 

            case WM_SIZE:
            cxScreen = LOWORD(lParam);
            cyScreen = HIWORD(lParam);

            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin =  0;
            si.nMax = 100 + pStringVec->N ;
            si.nPage = cyScreen / cyChar;
            SetScrollInfo(hwnd,SB_VERT,&si,TRUE);

            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = MAX_CHAR * 10;
            si.nPage = cxScreen / cxChar;
            SetScrollInfo(hwnd,SB_HORZ,&si,TRUE);

            break;    

        case WM_PAINT: 
            hdc = BeginPaint(hwnd, &ps); 

            
            si.cbSize = sizeof(si); 
            si.fMask = SIF_POS; 
            GetScrollInfo(hwnd, SB_VERT, &si);
            iVScrollPosition = si.nPos; 
            GetScrollInfo(hwnd, SB_HORZ, &si); 
            iHScrollPosition = si.nPos;
            
            iPaintBeginVertical = max(
                                        0, 
                                        iVScrollPosition + ps.rcPaint.top / cyChar
                                    ); 
            iPaintEndVertical = min(
                                        pStringVec->N, 
                                        iVScrollPosition + ps.rcPaint.bottom / cyChar
                                );
           
                for(iIndex =iPaintBeginVertical ;iIndex < iPaintEndVertical; iIndex += 1)
                    {
                         TextOut(hdc, 
                         (1 -  iHScrollPosition)*cxChar, 
                         (iIndex - iVScrollPosition) * cyChar, 
                         pStringVec->ppString[iIndex],
                         strlen(pStringVec->ppString[iIndex])
                         );
                    }
            
            EndPaint(hwnd, &ps); 
            break;

        case WM_CHAR:
            ZeroMemory(szStringTextOut,MAX_CHAR);
            iLength = wsprintf(szStringTextOut,szString,wParam);
            PushBack(pStringVec,szStringTextOut);

            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin =  0;
            si.nMax = pStringVec->N ;
            si.nPage = cyScreen / cyChar;
            SetScrollInfo(hwnd,SB_VERT,&si,TRUE);

            InvalidateRect(hwnd, NULL, TRUE);     
            break;

        case WM_KEYDOWN:

            switch(wParam)
            {
                case VK_PRIOR:
                    SendMessage(hwnd,WM_VSCROLL,SB_PAGEUP,0);
                    break;

                case VK_NEXT:
                    SendMessage(hwnd,WM_VSCROLL,SB_PAGEDOWN,0);
                    break;

                case VK_HOME:
                    SendMessage(hwnd,WM_VSCROLL,SB_TOP,0);
                    break;

                case VK_END:
                    SendMessage(hwnd,WM_VSCROLL,SB_BOTTOM,0);
                    break;

                case VK_UP:
                    SendMessage(hwnd,WM_VSCROLL,SB_LINEUP,0);
                    break;

                case VK_DOWN:
                    SendMessage(hwnd,WM_VSCROLL,SB_LINEDOWN,0);
                    break;

                case VK_LEFT:
                    SendMessage(hwnd,WM_HSCROLL,SB_LINELEFT,0);
                    break;

                case VK_RIGHT:
                    SendMessage(hwnd,WM_HSCROLL,SB_LINERIGHT,0);
                    break;          
            }   
            break; 

        case WM_VSCROLL:

            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd,SB_VERT,&si);
            iHScrollPosition = si.nPos;

            switch(LOWORD(wParam))
            {
                case SB_LINEUP:
                    si.nPos -= 1;
                    break;

                case SB_LINEDOWN:
                    si.nPos += 1;
                    break;

                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;

                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;

                case SB_TOP:
                    si.nPos = si.nMin;
                    break;

                case SB_BOTTOM:
                    si.nPos  = si.nMax;
                    break;

                case SB_THUMBPOSITION:
                    si.nPos = si.nTrackPos;
                    break;

            }

            si.fMask = SIF_POS;
            SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
            GetScrollInfo(hwnd,SB_VERT,&si);
          

            if(iVScrollPosition != si.nPos )
            {
                ScrollWindow(hwnd,0,(iVScrollPosition - si.nPos)*cyChar,NULL,NULL);
                UpdateWindow(hwnd);
            }
            break;

        case WM_HSCROLL:

            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd,SB_HORZ,&si);
            iHScrollPosition = si.nPos;

            switch(LOWORD(wParam))
            {
                case SB_LINELEFT:
                    si.nPos -= 1;
                    break;

                case SB_LINERIGHT:
                    si.nPos += 1;
                    break;

                case SB_PAGELEFT:
                    si.nPos -= si.nPage;
                    break;

                case SB_PAGERIGHT:
                    si.nPos += si.nPage;
                    break;

                case SB_THUMBPOSITION:
                    si.nPos = si.nTrackPos;
                    break;                

            }    

            si.fMask = SIF_POS;
            SetScrollInfo(hwnd,SB_HORZ,&si,TRUE);
            GetScrollInfo(hwnd,SB_HORZ,&si);

            if(iHScrollPosition != si.nPos)
            {
                ScrollWindow(hwnd,(iHScrollPosition - si.nPos)*cxChar ,0 ,NULL,NULL);
                UpdateWindow(hwnd);
            }
            break;


            
        case WM_DESTROY:
            DestroyVector(pStringVec);
            pStringVec = NULL;
            PostQuitMessage(0);
            break;

    }
    return(DefWindowProc(hwnd,uMsg,wParam,lParam));

}
PSTRINGVECTOR CreateVector(VOID)
{
    PSTRINGVECTOR pStringVec = NULL;

    pStringVec = (PSTRINGVECTOR)malloc(sizeof(STRINGVECTOR));
    if(pStringVec == NULL)
        {
             MessageBox(NULL, TEXT("Memory allocation failed"), TEXT("Error"), MB_ICONERROR);
             exit(EXIT_FAILURE);
        }
        
    pStringVec->ppString = NULL;
    pStringVec->N = 0;
   
    return(pStringVec);   
}
void PushBack(PSTRINGVECTOR pStringVec, char* pString)
{
    pStringVec->ppString = (char**)realloc(pStringVec->ppString,(pStringVec->N+1) *sizeof(char*));

    if(pStringVec->ppString == NULL)
       {
             MessageBox(NULL, TEXT("Memory reallocation failed"), TEXT("Error"), MB_ICONERROR);
             exit(EXIT_FAILURE);    
       }

     pStringVec->N += 1;

    pStringVec->ppString[pStringVec->N-1] = (char*)malloc(strlen(pString) + 1);
    if(pStringVec->ppString[pStringVec->N-1] == NULL)
     {
         MessageBox(NULL, TEXT("Memory allocation failed"), TEXT("Error"), MB_ICONERROR);
         exit(EXIT_FAILURE);

     }

    strcpy(pStringVec->ppString[pStringVec->N-1],pString);     


}
void DestroyVector(PSTRINGVECTOR pStringVec)
{
for (int i = 0; i < pStringVec->N; i++) {
        free(pStringVec->ppString[i]);
    }
    free(pStringVec->ppString);
    free(pStringVec);
}
