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
#if (INPUTSYS == USE_NATIVE_SUPPORT)
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
	
	// Mouse buttons
	_poInputs->m_oCursor.m_rPressure[IPB_A] = (GetAsyncKeyState(VK_LBUTTON) != 0)?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_B] = (GetAsyncKeyState(VK_RBUTTON) != 0)?1.0f:0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_C] = (GetAsyncKeyState(VK_MBUTTON) != 0)?1.0f:0.0f;

	POINT oCursorCoords;		
	::GetCursorPos(&oCursorCoords);
	
	RECT  oWindowRect;
	::GetWindowRect((HWND)((CInputData*)_hInputHandler)->m_hHandler, &oWindowRect);
	_poInputs->m_oCursor.m_oCoords.x = (FEReal)(oCursorCoords.x - oWindowRect.left)/(FEReal)(oWindowRect.right - oWindowRect.left);
	_poInputs->m_oCursor.m_oCoords.y = (FEReal)(oCursorCoords.y - oWindowRect.top)/(FEReal)(oWindowRect.bottom - oWindowRect.top);
	
	// keyboard keys
	_poInputs->m_bButtons[IB_UP] = (GetAsyncKeyState(VK_UP) != 0) || (GetAsyncKeyState('W') != 0);	
	_poInputs->m_bButtons[IB_DOWN] = (GetAsyncKeyState(VK_DOWN) != 0) || (GetAsyncKeyState('S') != 0);
	_poInputs->m_bButtons[IB_LEFT] = (GetAsyncKeyState(VK_LEFT) != 0) || (GetAsyncKeyState('A') != 0);
	_poInputs->m_bButtons[IB_RIGHT] = (GetAsyncKeyState(VK_RIGHT) != 0) || (GetAsyncKeyState('D') != 0);

	_poInputs->m_bButtons[IB_MIG] = (GetAsyncKeyState(VK_ESCAPE) != 0);
	
	_poInputs->m_bButtons[IB_A] = (GetAsyncKeyState(VK_SPACE) != 0);
	_poInputs->m_bButtons[IB_B] = (GetAsyncKeyState('M') != 0);
	_poInputs->m_bButtons[IB_C] = (GetAsyncKeyState('N') != 0);
	_poInputs->m_bButtons[IB_D] = (GetAsyncKeyState('B') != 0);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
