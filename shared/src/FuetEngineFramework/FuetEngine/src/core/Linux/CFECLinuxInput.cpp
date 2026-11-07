// ----------------------------------------------------------------------------
/*! \brief Linux input implementation. Keyboard and mouse state are polled through X11.
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if ((INPUTSYS == USE_NATIVE_SUPPORT) && (TARGETPLATFORM == USE_LINUX))
// ----------------------------------------------------------------------------
#include "CFECGlobals.h"
#include "../CFECore.h"
#include "FECLinuxWindow.h"

// Key symbol values (same as X11/keysym.h, defined here to keep X11 macros out of the engine)
#define FEK_Up      0xff52
#define FEK_Down    0xff54
#define FEK_Left    0xff51
#define FEK_Right   0xff53
#define FEK_Escape  0xff1b
#define FEK_Return  0xff0d
#define FEK_Next    0xff56  // Page Down
#define FEK_Prior   0xff55  // Page Up
#define FEK_F11     0xffc8
#define FEK_space   0x0020
#define FEK_plus    0x002b
// ----------------------------------------------------------------------------
class CInputData
{
	public:
		CInputData(const TFELinuxWindow* _poWindow)
		{
			m_bHasWindow = (_poWindow != NULL);
			if (m_bHasWindow) m_oWindow = *_poWindow;
		}

		/// Window to poll: the one given by the host, or the one created by the first renderer.
		const TFELinuxWindow* poWindow() const
		{
			return( m_bHasWindow?&m_oWindow:FELinux_GetMainWindow() );
		}

		bool			m_bHasWindow;
		TFELinuxWindow	m_oWindow;
};
// ----------------------------------------------------------------------------
FEHandler CFECore::Input::hInit(FEHandler _hParam)
{
    // _hParam is an optional (TFELinuxWindow*). See FECLinuxWindow.h
    return( (FEHandler)new CInputData((const TFELinuxWindow*)_hParam) );
}
// ----------------------------------------------------------------------------
void CFECore::Input::Finish(FEHandler _hInputHandler)
{
    if (_hInputHandler == NULL) return;
    delete ((CInputData*)_hInputHandler);
}
// ----------------------------------------------------------------------------
void CFECore::Input::Update(FEHandler _hInputHandler,TFEInputStruct* _poInputs)
{
	if (_hInputHandler == NULL) return;

	const TFELinuxWindow* poWin = ((CInputData*)_hInputHandler)->poWindow();
	if (poWin == NULL) return;

	// Mouse
	int  iX,iY;
	bool bLeft,bRight,bMiddle;
	FELinux_GetMouse(poWin,&iX,&iY,&bLeft,&bRight,&bMiddle);

	_poInputs->m_oCursor.m_rPressure[IPB_A] = bLeft  ?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_B] = bRight ?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_C] = bMiddle?1.0f:0.0f;

	// coordinates in the viewport ref sys (normalized)
    FEReal rX = (FEReal)iX;
    FEReal rY = (FEReal)iY;

	if ((FECglobals.m_uiViewportWidth != 0) && (FECglobals.m_uiViewportHeight != 0))
	{
		rX -= (FECglobals.m_uiViewportWMargin);
		rY -= (FECglobals.m_uiViewportHMargin);
		rX /= FECglobals.m_uiViewportWidth;
		rY /= FECglobals.m_uiViewportHeight;
	}

    _poInputs->m_oCursor.m_oCoords.x = rX;
    _poInputs->m_oCursor.m_oCoords.y = rY;

	// keyboard keys
	#define KEY(k) FELinux_bIsKeyDown(poWin,(k))

	_poInputs->m_bButtons[IB_UP]    = KEY(FEK_Up)    || KEY('w');
	_poInputs->m_bButtons[IB_DOWN]	= KEY(FEK_Down)  || KEY('s');
	_poInputs->m_bButtons[IB_LEFT]	= KEY(FEK_Left)  || KEY('a');
	_poInputs->m_bButtons[IB_RIGHT] = KEY(FEK_Right) || KEY('d');

	_poInputs->m_bButtons[IB_MIG] = KEY(FEK_Escape);

	_poInputs->m_bButtons[IB_A] = KEY(FEK_space);
	_poInputs->m_bButtons[IB_B] = KEY('m');
	_poInputs->m_bButtons[IB_C] = KEY('n');
	_poInputs->m_bButtons[IB_D] = KEY('b');
	_poInputs->m_bButtons[IB_E] = KEY('q');
	_poInputs->m_bButtons[IB_F] = KEY('e');

	_poInputs->m_bButtons[IB_G] = KEY(FEK_Return);

	_poInputs->m_bButtons[IB_H] = KEY(FEK_plus);
	_poInputs->m_bButtons[IB_I] = KEY(FEK_F11);
	_poInputs->m_bButtons[IB_J] = KEY(FEK_Next);
	_poInputs->m_bButtons[IB_K] = KEY(FEK_Prior);

	#undef KEY
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
