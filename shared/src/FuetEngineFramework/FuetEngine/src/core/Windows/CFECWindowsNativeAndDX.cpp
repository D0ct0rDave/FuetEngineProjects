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
#if (INPUTSYS == USE_NATIVE_AND_DIRECTINPUT)
// ----------------------------------------------------------------------------
#include "CFECGlobals.h"
#include "../CFECore.h"

// #include "WinJoy.h"
#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>
// #include "input/input.h"

// ----------------------------------------------------------------------------
class CInputData
{
	public:
		CInputData(FEHandler _hHandler)
		{
			m_pDI = NULL;
			m_pJoystick = NULL;
			m_pJoystickDevice2 = NULL;  // needed to poll joystick
			m_hInst = NULL;
			m_hWnd = (HWND)_hHandler;
		}

		IDirectInput*           m_pDI;
		IDirectInputDevice*     m_pJoystick;
		IDirectInputDevice2*    m_pJoystickDevice2;
		HINSTANCE               m_hInst;
		HWND					m_hWnd;
};
//-----------------------------------------------------------------------------
// Local function-prototypes
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst,
	LPVOID lpvContext);

//-----------------------------------------------------------------------------
// Function: InitDirectInput
//
// Description: 
//      Initialize the DirectInput variables.
//
//-----------------------------------------------------------------------------
HRESULT InitDirectInput(CInputData* _poID)
{
	HRESULT hr;

	// HINSTANCE _hInst,
	_poID->m_hInst = (HINSTANCE)GetModuleHandle(NULL); // _hInst;	

	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	hr = DirectInputCreate(_poID->m_hInst, DIRECTINPUT_VERSION, &_poID->m_pDI, NULL);
	if (FAILED(hr))
		return hr;

	// look for a simple joystick we can use for this sample program.
	hr = _poID->m_pDI->EnumDevices(DIDEVTYPE_JOYSTICK,
		EnumJoysticksCallback,
		_poID,
		DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
		return hr;

	if (NULL == _poID->m_pJoystick)
	{
		CFECore::Log::Print("Joystick not found\n");
		return E_FAIL;
	}

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing a
	// DIJOYSTATE structure to IDirectInputDevice::GetDeviceState.
	hr = _poID->m_pJoystick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr))
		return hr;

	// Set the cooperativity level to let DirectInput know how
	// this device should interact with the system and with other
	// DirectInput applications.
	/*
	hr = _poID->m_pJoystick->SetCooperativeLevel( _poID->m_hWnd,
	DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	*/
	if (FAILED(hr))
		return hr;

	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = +1000;

	diprg.diph.dwObj = DIJOFS_X;    // set the x-axis range
	hr = _poID->m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	// if (FAILED(hr)) return hr;

	diprg.diph.dwObj = DIJOFS_Y;    // set the y-axis range
	hr = _poID->m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	// if (FAILED(hr)) return hr;

	diprg.diph.dwObj = DIJOFS_Z;    // set the z-axis range
	hr = _poID->m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	// if (FAILED(hr)) return hr;

	diprg.diph.dwObj = DIJOFS_RZ;   // set the rudder range
	hr = _poID->m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
	// if (FAILED(hr)) return hr;

	_poID->m_pJoystick->Acquire();
	return S_OK;
}
//-----------------------------------------------------------------------------
// Function: EnumJoysticksCallback
//
// Description: 
//      Called once for each enumerated joystick. If we find one, 
//       create a device interface on it so we can play with it.
//
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst,
	LPVOID pvRef)
{
	HRESULT             hr;
	LPDIRECTINPUTDEVICE pDevice;
	CInputData* _poID = (CInputData*)pvRef;

	// obtain an interface to the enumerated force feedback joystick.
	hr = _poID->m_pDI->CreateDevice(pInst->guidInstance, &pDevice, NULL);

	// if it failed, then we can't use this joystick for some
	// bizarre reason.  (Maybe the user unplugged it while we
	// were in the middle of enumerating it.)  So continue enumerating
	if (FAILED(hr))
		return DIENUM_CONTINUE;

	// we successfully created an IDirectInputDevice.  So stop looking 
	// for another one.
	_poID->m_pJoystick = pDevice;

	// query for IDirectInputDevice2 - we need this to poll the joystick 
	pDevice->QueryInterface(IID_IDirectInputDevice2,
		(LPVOID *)&_poID->m_pJoystickDevice2);

	return DIENUM_STOP;
}

//-----------------------------------------------------------------------------
// Function: SetAcquire
//
// Description: 
//      Acquire or unacquire the keyboard, depending on if the app is active
//       Input device must be acquired before the GetDeviceState is called
//
//-----------------------------------------------------------------------------
/*
HRESULT SetAcquire(FEBool _bAquire)
{
// nothing to do if g_pJoystick is NULL
if (NULL == g_pJoystick)
return S_FALSE;

if (_bAquire)
{
// acquire the input device
g_pJoystick->Acquire();
}
else
{
// unacquire the input device
g_pJoystick->Unacquire();
}

return S_OK;
}
*/
// ----------------------------------------------------------------------------
FEHandler CFECore::Input::hInit(FEHandler _hParam)
{
	// Create and initialize the handler.    
	FEHandler hHandler = (FEHandler)new CInputData(_hParam);
	
	if (InitDirectInput((CInputData*)hHandler) != S_OK)
		((CInputData*)hHandler)->m_pJoystick = NULL;

	return(hHandler);
}
// ----------------------------------------------------------------------------
void CFECore::Input::Finish(FEHandler _hInputHandler)
{
	if (_hInputHandler == NULL) return;
}
// ----------------------------------------------------------------------------
void CFECore::Input::Update(FEHandler _hInputHandler, TFEInputStruct* _poInputs)
{
	if (_hInputHandler == NULL) return;

	// Discard inputs if our window doesn't own the focus
	#if (!defined(DEVELOPMENT)) && (!defined(PLAYTEST))
	if (::GetForegroundWindow() != (HWND)((CInputData*)_hInputHandler)->m_hWnd) return;
	#endif

	// Mouse buttons
	_poInputs->m_oCursor.m_rPressure[IPB_A] = (GetAsyncKeyState(VK_LBUTTON) != 0) ? 1.0f : 0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_B] = (GetAsyncKeyState(VK_RBUTTON) != 0) ? 1.0f : 0.0f;
	_poInputs->m_oCursor.m_rPressure[IPB_C] = (GetAsyncKeyState(VK_MBUTTON) != 0) ? 1.0f : 0.0f;

	POINT oCursorCoords;
	::GetCursorPos(&oCursorCoords);
	::ScreenToClient((HWND)((CInputData*)_hInputHandler)->m_hWnd, &oCursorCoords);

	RECT  oWindowRect;
	::GetClientRect((HWND)((CInputData*)_hInputHandler)->m_hWnd, &oWindowRect);

	// FEReal rWindowWidth = (FEReal)(oWindowRect.right - oWindowRect.left);
	// FEReal rWindowHeight= (FEReal)(oWindowRect.bottom - oWindowRect.top);

	// coordinates in the window ref sys 
	FEReal rX = (FEReal)(oCursorCoords.x - oWindowRect.left);
	FEReal rY = (FEReal)(oCursorCoords.y - oWindowRect.top);

	// lets calc the coords in the viewport ref sys (normalized)
	rX -= (FECglobals.m_uiViewportWMargin);
	rY -= (FECglobals.m_uiViewportHMargin);
	rX /= FECglobals.m_uiViewportWidth;
	rY /= FECglobals.m_uiViewportHeight;

	_poInputs->m_oCursor.m_oCoords.x = rX; // CFEMath::rClamp(_0r,_1r,rX);
	_poInputs->m_oCursor.m_oCoords.y = rY; // CFEMath::rClamp(_0r,_1r,rY);

	// keyboard keys
	_poInputs->m_bButtons[IB_UP]    = (GetAsyncKeyState(VK_UP) != 0) || (GetAsyncKeyState('W') != 0);
	_poInputs->m_bButtons[IB_DOWN]  = (GetAsyncKeyState(VK_DOWN) != 0) || (GetAsyncKeyState('S') != 0);
	_poInputs->m_bButtons[IB_LEFT]  = (GetAsyncKeyState(VK_LEFT) != 0) || (GetAsyncKeyState('A') != 0);
	_poInputs->m_bButtons[IB_RIGHT] = (GetAsyncKeyState(VK_RIGHT) != 0) || (GetAsyncKeyState('D') != 0);

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
	_poInputs->m_bButtons[IB_J] = (GetAsyncKeyState(VK_CONTROL) != 0);
	_poInputs->m_bButtons[IB_K] = (GetAsyncKeyState(VK_SHIFT) != 0);

	// Keypad buttons	
	HRESULT     hr;
	DIJOYSTATE  js;          // DirectInput joystick state 

	if (NULL != ((CInputData*)_hInputHandler)->m_pJoystick)
	{
		hr = DIERR_INPUTLOST;

		// if input is lost then acquire and keep trying 
		while (DIERR_INPUTLOST == hr)
		{
			// poll the joystick to read the current state
			hr = ((CInputData*)_hInputHandler)->m_pJoystickDevice2->Poll();
			if (FAILED(hr))
				break;

			// get the input's device state, and put the state in dims
			hr = ((CInputData*)_hInputHandler)->m_pJoystick->GetDeviceState(sizeof(DIJOYSTATE), &js);

			if (hr == DIERR_INPUTLOST)
			{
				// DirectInput is telling us that the input stream has
				// been interrupted.  We aren't tracking any state
				// between polls, so we don't have any special reset
				// that needs to be done.  We just re-acquire and
				// try again.
				hr = ((CInputData*)_hInputHandler)->m_pJoystick->Acquire();
				
				if (FAILED(hr))
					break;
			}
		}

		if (!FAILED(hr))
		{
			_poInputs->m_rAxis[IA_X] = (FEReal)(js.lX )/_1000r;
			_poInputs->m_rAxis[IA_Y] = (FEReal)(js.lY )/_1000r;
			_poInputs->m_rAxis[IA_Z] = (FEReal)(js.lRx)/_1000r;
			_poInputs->m_rAxis[IA_W] = (FEReal)(js.lRy)/_1000r;

			// 12 up
			// 13 right
			// 14 down
			// 15 left
			// 2 A = A on Xbox PAD
			// 3 B = A on Xbox PAD
			// 1 C = X on Xbox PAD
			// 0 D = Y on Xbox PAD
			const LONG STICK_THRESHOLD = 333;

			_poInputs->m_bButtons[IB_UP]    |= (js.lY < -STICK_THRESHOLD);
			_poInputs->m_bButtons[IB_DOWN]  |= (js.lY >  STICK_THRESHOLD);
			_poInputs->m_bButtons[IB_LEFT]  |= (js.lX < -STICK_THRESHOLD);
			_poInputs->m_bButtons[IB_RIGHT] |= (js.lX >  STICK_THRESHOLD);

			_poInputs->m_bButtons[IB_UP]	|= (js.rgdwPOV[0] == 0) || (js.rgdwPOV[0] == (36000 - 4500)) || (js.rgdwPOV[0] == 4500);
			_poInputs->m_bButtons[IB_DOWN]  |= (js.rgdwPOV[0] >= (18000 - 4500)) && (js.rgdwPOV[0] <= (18000 + 4500));
			_poInputs->m_bButtons[IB_LEFT]  |= (js.rgdwPOV[0] >= (27000 - 4500)) && (js.rgdwPOV[0] <= (27000 + 4500));
			_poInputs->m_bButtons[IB_RIGHT] |= (js.rgdwPOV[0] >= ( 9000 - 4500)) && (js.rgdwPOV[0] <=  (9000 + 4500));

			_poInputs->m_bButtons[IB_MIG]	|= (js.rgbButtons[6] != 0);

			_poInputs->m_bButtons[IB_A] |= (js.rgbButtons[ 0] != 0);
			_poInputs->m_bButtons[IB_B] |= (js.rgbButtons[ 1] != 0);
			_poInputs->m_bButtons[IB_C] |= (js.rgbButtons[ 2] != 0);
			_poInputs->m_bButtons[IB_D] |= (js.rgbButtons[ 3] != 0);

			_poInputs->m_bButtons[IB_E] |= (js.rgbButtons[ 4] != 0);
			_poInputs->m_bButtons[IB_F] |= (js.rgbButtons[ 5] != 0);
			_poInputs->m_bButtons[IB_G] |= (js.rgbButtons[ 7] != 0);
			_poInputs->m_bButtons[IB_H] |= (js.rgbButtons[ 8] != 0);
			_poInputs->m_bButtons[IB_I] |= (js.rgbButtons[ 9] != 0);
			_poInputs->m_bButtons[IB_J] |= (js.rgbButtons[10] != 0);
			_poInputs->m_bButtons[IB_K] |= (js.rgbButtons[11] != 0);
		}
	}

	// show/hide cursor
	#if (!defined(DEVELOPMENT)) && (!defined(PLAYTEST))
	bool bInside = CFEMath::bBetween(FECglobals.m_uiViewportWMargin, FECglobals.m_uiViewportWidth + FECglobals.m_uiViewportWMargin, oCursorCoords.x)
		&& CFEMath::bBetween(FECglobals.m_uiViewportHMargin, FECglobals.m_uiViewportHeight + FECglobals.m_uiViewportHMargin, oCursorCoords.y);

	ShowCursor(!bInside);
	#endif
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
