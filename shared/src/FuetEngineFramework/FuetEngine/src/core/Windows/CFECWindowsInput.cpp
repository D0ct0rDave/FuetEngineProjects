// ----------------------------------------------------------------------------
/*! \class CFECoundMgr
 *  \brief Sound Manager Class Definition
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (INPUTSYS == USE_NATIVE_SUPPORT)
// ----------------------------------------------------------------------------

#include "CFECGlobals.h"
#include "../CFECore.h"
#include <windows.h>
// ----------------------------------------------------------------------------
class CInputData
{
	public:CInputData(FEHandler _hHandler) : 
			m_hHandler(_hHandler)
		{
			
		}

		FEHandler m_hHandler;
};
// ----------------------------------------------------------------------------
FEHandler CFECore::Input::hInit(FEHandler _hParam)
{
    // Create and initialize the handler.    
    return( (FEHandler)new CInputData(_hParam) );
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

	// Discard inputs if our window doesn't own the focus
	#if (!defined(DEVELOPMENT)) && (!defined(PLAYTEST))
	if (::GetForegroundWindow() != (HWND)((CInputData*)_hInputHandler)->m_hHandler) return;
	#endif
	
	// Mouse buttons
	_poInputs->m_oCursor.m_rPressure[IPB_A] = (GetAsyncKeyState(VK_LBUTTON) != 0)?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_B] = (GetAsyncKeyState(VK_RBUTTON) != 0)?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_C] = (GetAsyncKeyState(VK_MBUTTON) != 0)?1.0f:0.0f;

	POINT oCursorCoords;		
	::GetCursorPos(&oCursorCoords);
	::ScreenToClient( (HWND)((CInputData*)_hInputHandler)->m_hHandler,&oCursorCoords );

	RECT  oWindowRect;
	::GetClientRect((HWND)((CInputData*)_hInputHandler)->m_hHandler, &oWindowRect);

	// FEReal rWindowWidth = (FEReal)(oWindowRect.right - oWindowRect.left);
	// FEReal rWindowHeight= (FEReal)(oWindowRect.bottom - oWindowRect.top);

	// coordinates in the window ref sys 
    FEReal rX           = (FEReal)(oCursorCoords.x - oWindowRect.left);
    FEReal rY           = (FEReal)(oCursorCoords.y - oWindowRect.top);
	
	// lets calc the coords in the viewport ref sys (normalized)
	rX -= (FECglobals.m_uiViewportWMargin);
	rY -= (FECglobals.m_uiViewportHMargin);
	rX /= FECglobals.m_uiViewportWidth;
	rY /= FECglobals.m_uiViewportHeight;

    _poInputs->m_oCursor.m_oCoords.x = rX; // CFEMath::rClamp(_0r,_1r,rX);
    _poInputs->m_oCursor.m_oCoords.y = rY; // CFEMath::rClamp(_0r,_1r,rY);

	// keyboard keys
	_poInputs->m_bButtons[IB_UP]    = (GetAsyncKeyState(VK_UP)		!= 0) || (GetAsyncKeyState('W') != 0);
	_poInputs->m_bButtons[IB_DOWN]	= (GetAsyncKeyState(VK_DOWN)	!= 0) || (GetAsyncKeyState('S') != 0);
	_poInputs->m_bButtons[IB_LEFT]	= (GetAsyncKeyState(VK_LEFT)	!= 0) || (GetAsyncKeyState('A') != 0);
	_poInputs->m_bButtons[IB_RIGHT] = (GetAsyncKeyState(VK_RIGHT)	!= 0) || (GetAsyncKeyState('D') != 0);

	_poInputs->m_bButtons[IB_MIG] = (GetAsyncKeyState(VK_ESCAPE) != 0);

	_poInputs->m_bButtons[IB_A] = (GetAsyncKeyState(VK_SPACE) != 0);
	_poInputs->m_bButtons[IB_B] = (GetAsyncKeyState('M') != 0);
	_poInputs->m_bButtons[IB_C] = (GetAsyncKeyState('N') != 0);
	_poInputs->m_bButtons[IB_D] = (GetAsyncKeyState('B') != 0);
	_poInputs->m_bButtons[IB_E] = (GetAsyncKeyState('Q') != 0);
	_poInputs->m_bButtons[IB_F] = (GetAsyncKeyState('E') != 0);

	_poInputs->m_bButtons[IB_G] = (GetAsyncKeyState(VK_RETURN) != 0);

	_poInputs->m_bButtons[IB_H] = (GetAsyncKeyState('+') != 0);
	_poInputs->m_bButtons[IB_I] = (GetAsyncKeyState(VK_F11) != 0);
	_poInputs->m_bButtons[IB_J] = (GetAsyncKeyState(VK_NEXT) != 0);
	_poInputs->m_bButtons[IB_K] = (GetAsyncKeyState(VK_PRIOR) != 0);

	// show/hide cursor
	#if (!defined(DEVELOPMENT)) && (!defined(PLAYTEST))
	bool bInside = CFEMath::bBetween(FECglobals.m_uiViewportWMargin,FECglobals.m_uiViewportWidth +FECglobals.m_uiViewportWMargin,oCursorCoords.x)
				&& CFEMath::bBetween(FECglobals.m_uiViewportHMargin,FECglobals.m_uiViewportHeight+FECglobals.m_uiViewportHMargin,oCursorCoords.y);

	ShowCursor(!bInside);
	#endif

	/*
	static int iD = 0;
	if (GetAsyncKeyState(VK_SPACE) != 0)
	{
		char szStr[1024];
		sprintf(szStr,"%d - lolo!!!\n",iD++);
		OutputDebugStr(szStr);
	}
	*/
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
