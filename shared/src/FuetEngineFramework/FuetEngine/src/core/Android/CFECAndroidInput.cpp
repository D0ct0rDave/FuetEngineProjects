// ----------------------------------------------------------------------------
/*! \class CFECAndroidInput
 *  \brief Android Input Management
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_ANDROID)
// ----------------------------------------------------------------------------
#include "FEDefs.h"
#include "FEBasicTypes.h"
#include "../CFECore.h"
#include <stdarg.h>
#include "CFECGlobals.h"

// for native asset manager
#include <sys/types.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "jnihelper.h"
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Input management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Creates and initializes a input manager
FEHandler CFECore::Input::hInit(FEHandler _hParam)
{
	return( reinterpret_cast<FEHandler>(1) );
}
// ----------------------------------------------------------------------------
/// Finalizes the given input manager
void CFECore::Input::Finish(FEHandler _hInputHandler)
{
}
// ----------------------------------------------------------------------------
/// Retrieves the inputs of a given input manager.
void CFECore::Input::Update(FEHandler _hInputHandler,TFEInputStruct* _poInputs)
{
    TFEInputStruct* poInput = GetInputStruct();
    *_poInputs = *poInput;

    // fix x and y
    FEReal rX = (_poInputs->m_oCursor.m_oCoords.x - (float)FECglobals.m_uiViewportWMargin)/(float)FECglobals.m_uiViewportWidth;
    FEReal rY = (_poInputs->m_oCursor.m_oCoords.y - (float)FECglobals.m_uiViewportHMargin)/(float)FECglobals.m_uiViewportHeight;

    if (! FECglobals.m_bViewportRotated)
    {
        _poInputs->m_oCursor.m_oCoords.x = CFEMath::rClamp(_0r,_1r,rX);
        _poInputs->m_oCursor.m_oCoords.y = CFEMath::rClamp(_0r,_1r,rY);
    }
    else
    {
        // exchange coordS?
        _poInputs->m_oCursor.m_oCoords.x =       CFEMath::rClamp(_0r,_1r,rY);
        _poInputs->m_oCursor.m_oCoords.y = _1r - CFEMath::rClamp(_0r,_1r,rX);
    }
}
// ----------------------------------------------------------------------------
#endif // (TARGETPLATFORM == USE_ANDROID)
// ----------------------------------------------------------------------------
