//-----------------------------------------------------------------------------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "FPS.h"

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define APPLICATION_NAME                "FPS Project"
#define APPLICATION_TITLE               "FPS Sample"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
// Name of our bitmap resource.
#define APPSTATE_MENU                0
#define APPSTATE_GAME                1
#define APPSTATE_DEMO                2
#define APPSTATE_DEMORECORD            3

unsigned int guiAppState = APPSTATE_GAME;

// Return values for the FS library API
typedef enum FSResult
{
    // Result values that can be obtained with the FS_GetErrorCode function
    FS_RESULT_SUCCESS = 0,
    FS_RESULT_FAILURE,
    FS_RESULT_BUSY,
    FS_RESULT_CANCELED,
    FS_RESULT_CANCELLED = FS_RESULT_CANCELED,
    FS_RESULT_UNSUPPORTED,
    FS_RESULT_ERROR,
    FS_RESULT_INVALID_PARAMETER,
    FS_RESULT_NO_MORE_RESOURCE,
    FS_RESULT_ALREADY_DONE,
    FS_RESULT_PERMISSION_DENIED,
    FS_RESULT_MEDIA_FATAL,
    FS_RESULT_NO_ENTRY,
    FS_RESULT_MEDIA_NOTHING,
    FS_RESULT_MEDIA_UNKNOWN,
    FS_RESULT_BAD_FORMAT,
    FS_RESULT_MAX,
    // Temporary result values to use within procedures
    FS_RESULT_PROC_ASYNC = 256,
    FS_RESULT_PROC_DEFAULT,
    FS_RESULT_PROC_UNKNOWN
}
FSResult;


//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                        rc;
    int                         xRight = -1;
    int                         yBottom = -1;
    MINMAXINFO                 *pMinMax;

    switch (message)
    {
        case WM_DESTROY:
            // Clean up and close the app
            FinishEngine();
            PostQuitMessage(0);
            return 0L;

        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ESCAPE:
                case VK_F12:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                return 0L;
            }

        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow, HWND *phWnd)
{
    WNDCLASS            wc;
    unsigned int uiWndStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU /*| WS_THICKFRAME*/;

    /*
     * set up and register window class
     */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0; // LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL; // MAKEINTRESOURCE(IDR_MENU);
    wc.lpszClassName = APPLICATION_NAME;
    RegisterClass( &wc );


    // Get window rect to get a perfect client rect
    RECT ClientRect;
    ClientRect.left  = 0;
    ClientRect.top   = 0;
    ClientRect.right = 256;
    ClientRect.bottom= 192;

    AdjustWindowRect(&ClientRect,uiWndStyle,false);

    
    /*
     * create a window
     */
    *phWnd = CreateWindowEx(
    0,
    APPLICATION_NAME,
    APPLICATION_TITLE,
    uiWndStyle,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    ClientRect.right - ClientRect.left,
    ClientRect.bottom - ClientRect.top,
    NULL,
    NULL,
    hInstance,
    NULL );

    if( !*phWnd )
    {
    return FALSE;
    }

    ShowWindow( *phWnd, nCmdShow );
    UpdateWindow( *phWnd );

    InitEngine(*phWnd);
    return(1);
}
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG                         msg;
    HWND                        hWnd;

    if (InitApp(hInstance, nCmdShow, &hWnd) != 1)
        return FALSE;

    while (TRUE)
    {	
		FSResult res = (FSResult)11;
		
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            UpdateFrame();
    }
}
// -----------------------------------------------------------------------------