// ----------------------------------------------------------------------------
/*! \class CoreTypes: Data Types exposed only to the FuetEngine API,
 *         not the user application.
 *         These are shared between the core and the high level api.
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFECoreTypesH
#define CFECoreTypesH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "types/CFEVect2.h"
#include "FEConfig.h"

// ----------------------------------------------------------------------------
// For inputs
// ----------------------------------------------------------------------------
typedef struct TFEInputCursor
{

    CFEVect2 m_oCoords;
    FEReal   m_rPressure[IPB_NUMIPBS];

}TFEInputCursor;

typedef struct TFEInputStruct
{
	/// Digital Buttons
	FEBool m_bButtons[IB_NUM_BUTTONS];

	/// Axis
	FEReal m_rAxis[IA_NUMAXIS];

	/// Cursor
	TFEInputCursor m_oCursor;

	/// Other miscelaneous controls
	/// keyboard ???

}TFEInputStruct;

// ----------------------------------------------------------------------------

#if (TARGETPLATFORM == USE_DEVELOPMENT)
    #include "Windows/FECTypes.h"

#elif (TARGETPLATFORM == USE_WINDOWS)
    #include "Windows/FECTypes.h"

#elif (TARGETPLATFORM == USE_LINUX)
    #include "Linux/FECTypes.h"

#elif (TARGETPLATFORM == USE_MACOS)
    #include "MacOS/FECTypes.h"

#elif (TARGETPLATFORM == USE_DS)
    #include "DS/FECTypes.h"

#elif (TARGETPLATFORM == USE_WII)
    #include "Wii/FECTypes.h"

#elif (TARGETPLATFORM == USE_N3DS)
    #include "N3DS/FECTypes.h"

#elif (TARGETPLATFORM == USE_COCOS2D_X)
    #include "COCOS2D_X/FECTypes.h"

#elif (TARGETPLATFORM == USE_ANDROID)
    #include "Android/FECTypes.h"

#elif (TARGETPLATFORM == USE_CAFE)
	#include "WiiU\FECTypes.h"

#else
	#pragma error("No target platform defined to compile engine")
#endif

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
