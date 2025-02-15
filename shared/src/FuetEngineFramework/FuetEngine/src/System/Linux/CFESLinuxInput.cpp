// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (INPUTSYS == USE_OSINPUT)
// ----------------------------------------------------------------------------

#include "CFESGlobals.h"
#include "../CFESystem.h"
// ----------------------------------------------------------------------------
class CInputData
{
	public:
		CInputData(FEHandler _hHandler) :
			m_hHandler(_hHandler)
		{

		}

		FEHandler m_hHandler;
};
// ----------------------------------------------------------------------------
FEHandler CFESystem::Input::hInit(FEHandler _hParam)
{
    // Create and initialize the handler.
    return( (FEHandler)new CInputData(_hParam) );
}
// ----------------------------------------------------------------------------
void CFESystem::Input::Finish(FEHandler _hInputHandler)
{
    if (_hInputHandler == NULL) return;
}
// ----------------------------------------------------------------------------
void CFESystem::Input::Update(FEHandler _hInputHandler,TFEInputStruct* _poInputs)
{
    if (_hInputHandler == NULL) return;
    if (FESglobals.m_poCurrentRenderer == NULL) return;

	Display* pDisplay = FESglobals.m_poCurrentRenderer->m_poDisplay; // (Display*)((CInputData*)_hInputHandler)->m_hHandler;
	if (pDisplay == NULL) return;

	XEvent event_return;

    // keyboard events
    if(XCheckTypedEvent( pDisplay, KeyPress, &event_return))
    {
        if (event_return.type >= KeyPress)
        {
            XKeyEvent ke = event_return.xkey;
            //printf("EVENT KEYCODE= %d\n",ke.keycode);

            switch(ke.keycode)
            {
                //returncode = KBD_KEY_DOWN;
                case 113:
                // returncode = KBD_KEY_LEFT;
                break;
                case 114:
                // returncode = KBD_KEY_RIGHT;
                break;
                case 111:
                // returncode = KBD_KEY_UP;
                break;
                case 36:
                // returncode = KBD_KEY_ENTER;
                break;
                default:
                // printf("KEYCODE = %d\n",ke.keycode);
                // returncode = 0;
                break;
            }
        }
    }
    else
    {
        // returncode = 0;//printf("NO EVENT\n");
    }

    //  mouse events
    if(XCheckTypedEvent(pDisplay, MotionNotify, &event_return))
    {
        if (event_return.type >= MotionNotify)
        {
            XMotionEvent me = event_return.xmotion;
            XWindowAttributes wa;
            XGetWindowAttributes(pDisplay, me.window, &wa);

            _poInputs->m_oCursor.m_oCoords.x = (float)me.x / (float)wa.width;
            _poInputs->m_oCursor.m_oCoords.y = (float)me.y / (float)wa.height;
        }
    }

    //  mouse events
    if(XCheckTypedEvent(pDisplay, ButtonPress, &event_return))
    {
        if (event_return.type >= ButtonPress)
        {
            XButtonEvent be = event_return.xbutton;

            switch (be.button)
            {
                case Button1: _poInputs->m_oCursor.m_rPressure[IPB_A] = 1.0f; break;
                case Button2: _poInputs->m_oCursor.m_rPressure[IPB_B] = 1.0f; break;
                case Button3: _poInputs->m_oCursor.m_rPressure[IPB_C] = 1.0f; break;
            }
        }
    }

    if(XCheckTypedEvent(pDisplay, ButtonRelease, &event_return))
    {
        if (event_return.type >= ButtonRelease)
        {
            XButtonEvent be = event_return.xbutton;

            switch (be.button)
            {
                case Button1: _poInputs->m_oCursor.m_rPressure[IPB_A] = 0.0f; break;
                case Button2: _poInputs->m_oCursor.m_rPressure[IPB_B] = 0.0f; break;
                case Button3: _poInputs->m_oCursor.m_rPressure[IPB_C] = 0.0f; break;
            }
        }
    }
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
