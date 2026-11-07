// ----------------------------------------------------------------------------
/*! \brief Linux (X11/GLX) window and OpenGL context support.
 *
 *  Host applications (samples, wxWidgets, ...) can hand the engine a native X11
 *  window through CFECore::Graphics::hInit() and CFECore::Input::hInit() by
 *  passing a pointer to a TFELinuxWindow. If NULL is passed, the engine creates
 *  its own window (see FELinux_CreateContext).
 *
 *  This header deliberately does not include any X11 / GLX header, so it can be
 *  included from engine code without polluting the namespace with X11 macros.
 *
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FECLinuxWindowH
#define FECLinuxWindowH
// ----------------------------------------------------------------------------

/// Native window description provided by the host application.
/// m_pDisplay is a (Display*), m_ulWindow is a (Window) XID.
/// For wxWidgets/GTK: display = GDK_WINDOW_XDISPLAY(w), window = GDK_WINDOW_XID(w).
typedef struct TFELinuxWindow
{
    void*           m_pDisplay;
    unsigned long   m_ulWindow;

}TFELinuxWindow;

// Opaque rendering context handle
struct TFELinuxGLContext;

// ----------------------------------------------------------------------------
/// Creates a GLX rendering context on the given window (or on a newly created window if _poWindow is NULL).
/// Contexts share display lists / textures with the first context created. Makes the context current. On success returns the context and fills the window size.
TFELinuxGLContext* FELinux_CreateContext(const TFELinuxWindow* _poWindow,unsigned int* _puiWidth,unsigned int* _puiHeight);

/// Creates a top-level window ready for OpenGL (same visual the engine would pick) on the connection shared
/// by every window of the engine. The result can be passed as host window to Graphics::hInit / Input::hInit.
/// The application owns it: close it with FELinux_DestroyWindow and poll FELinux_bCloseRequested().
bool FELinux_bCreateWindow(const char* _szTitle,int _iX,int _iY,unsigned int _uiWidth,unsigned int _uiHeight,TFELinuxWindow* _poWindow);

/// Destroys a window created with FELinux_bCreateWindow.
void FELinux_DestroyWindow(const TFELinuxWindow* _poWindow);

/// Processes the pending X events of the shared connection. Returns true once the user asked to close any of
/// the windows created with FELinux_bCreateWindow (or by the engine).
bool FELinux_bCloseRequested();

/// Destroys the context (and the window if the engine created it).
void FELinux_DestroyContext(TFELinuxGLContext* _poCtx);

void FELinux_MakeCurrent(TFELinuxGLContext* _poCtx);
void FELinux_SwapBuffers(TFELinuxGLContext* _poCtx);
void* FELinux_GetProcAddress(const char* _szName);

/// Retrieves the current window size.
void FELinux_GetWindowSize(TFELinuxGLContext* _poCtx,unsigned int* _puiWidth,unsigned int* _puiHeight);

/// Processes pending X events for windows created by the engine (close, resize). Does nothing for host windows.
void FELinux_PumpEvents(TFELinuxGLContext* _poCtx);

/// Window used by the first renderer (for the input module). Valid after the first Graphics::hInit.
const TFELinuxWindow* FELinux_GetMainWindow();

// Input helpers (X11 polling, no event queue needed)
/// Returns true if the key with the given X11 KeySym is currently pressed.
bool FELinux_bIsKeyDown(const TFELinuxWindow* _poWindow,unsigned long _ulKeySym);

/// Retrieves the mouse position relative to the window and the state of the three buttons.
void FELinux_GetMouse(const TFELinuxWindow* _poWindow,int* _piX,int* _piY,bool* _pbLeft,bool* _pbRight,bool* _pbMiddle);

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
