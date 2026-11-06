//-----------------------------------------------------------------------------
// Common Linux entry point for the FuetEngine samples.
//
// Windows samples create their own HWND in main.cpp and hand it to the engine.
// On Linux the engine creates its own X11/GLX window when InitEngine receives
// NULL (a host such as wxWidgets can pass a TFELinuxWindow* instead, see
// core/Linux/FECLinuxWindow.h).
//
// Every sample exposes InitEngine / FinishEngine and a per-frame function whose
// name is given by FE_SAMPLE_LOOP (LoopEngine by default). FE_SAMPLE_HEADER is the
// sample header that declares them.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "core/Linux/FECLinuxWindow.h"

// Header of the sample (declares InitEngine / FinishEngine / loop function with its own linkage).
#ifndef FE_SAMPLE_HEADER
#error FE_SAMPLE_HEADER must be defined (e.g. "helloworld.h")
#endif
#include FE_SAMPLE_HEADER

#ifndef FE_SAMPLE_LOOP
#define FE_SAMPLE_LOOP LoopEngine
#endif

// Key symbols (X11/keysym.h)
#define FEK_Escape  0xff1b
#define FEK_F12     0xffc9
//-----------------------------------------------------------------------------
int main(int _iArgc,char** _ppcArgv)
{
    (void)_iArgc;
    (void)_ppcArgv;

    // Optional: FE_SAMPLE_FRAMES=N makes the sample exit after N frames (useful for automated tests).
    const char* szFrames = getenv("FE_SAMPLE_FRAMES");
    long lMaxFrames = (szFrames != NULL)?atol(szFrames):0;

    InitEngine(NULL);

    if (FELinux_GetMainWindow() == NULL)
    {
        fprintf(stderr,"The engine could not create its window.\n");
        return(1);
    }

    for (long lFrame = 0; (lMaxFrames == 0) || (lFrame < lMaxFrames); lFrame++)
    {
        FE_SAMPLE_LOOP();

        // Same exit keys as the Windows samples.
        if (FELinux_bIsKeyDown(FELinux_GetMainWindow(),FEK_Escape) || FELinux_bIsKeyDown(FELinux_GetMainWindow(),FEK_F12))
            break;
    }

    FinishEngine();
    return(0);
}
//-----------------------------------------------------------------------------
