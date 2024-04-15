//-----------------------------------------------------------------------------
// File: HelloWorld.CPP
//
// Desc: This program demonstrated clipped blting and stretched clipped blting.
//       It is a non-exclusive mode application that displays a rotating donut
//       in a window.  Clipped blting can be demonstrated by moving another
//       window partially or completely in front of the stretch window.  The
//       rotating donut does not overwrite the clipping window.
//
//       The size of the rotating donut can be changed with menu selections.
//       Any other size than 1x1 demonstrates stretch blting.  The window can
//       also be resized by grabbing any one of the corners with the mouse.
//
//       Another menu option can be used to change the rate of rotation of the
//       donut.
//
//       This is not an exclusive mode application and so it is incapable of
//       setting the display mode.  Therefore, it must be executed on an 8 bit
//       per pixel display.  It will not work correctly with other pixel
//       depths.
//
// Copyright (c) 1995-1998 Microsoft Corporation. All rights reserved.
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
#include <windows.h>
#include "HelloWorld.h"

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define APPLICATION_NAME                "EGLWndClass" // "Sample Project"
#define APPLICATION_TITLE               "Hello World"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
// Name of our bitmap resource.
#define APPSTATE_MENU                0
#define APPSTATE_GAME                1
#define APPSTATE_DEMO                2
#define APPSTATE_DEMORECORD          3

unsigned int guiAppState = APPSTATE_GAME;

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
    wc.lpfnWndProc = WindowProc;
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
    ClientRect.right = 640;
    ClientRect.bottom= 480;

    AdjustWindowRect(&ClientRect,uiWndStyle,false);

    
    /*
     * create a window
     */
    *phWnd = /*CreateWindow(
TEXT("EGLWndClass"),
NULL,
WS_OVERLAPPED|WS_SYSMENU|WS_THICKFRAME|WS_DISABLED,
CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
NULL,
NULL,
(HINSTANCE)GetModuleHandle(NULL),
NULL);
    */
    
    CreateWindowEx(
    0,
    APPLICATION_NAME,
    APPLICATION_TITLE,
    // WS_OVERLAPPED|WS_SYSMENU|WS_THICKFRAME|WS_DISABLED, // 
    uiWndStyle,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    ClientRect.right - ClientRect.left,
    ClientRect.bottom - ClientRect.top,
    NULL,
    NULL,
    // (HINSTANCE)GetModuleHandle(NULL), // 
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
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            LoopEngine();
    }
}
// -----------------------------------------------------------------------------