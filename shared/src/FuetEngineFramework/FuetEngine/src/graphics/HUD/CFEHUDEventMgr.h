// ----------------------------------------------------------------------------
/*! \class CFEEventMgr
 *  \brief A class to manage the logic of event handling.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDEventMgrH
#define CFEHUDEventMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "CFEHUDEventFunc.h"
//-----------------------------------------------------------------------------
class CFEHUDObject;
//-----------------------------------------------------------------------------
class CFEHUDEventMgr
{
	public:

		/// Performs the call to the event handling function. Usually only to be called by the engine.
		static void TriggerEvent(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj,FEPointer _pParam);

		/// Registers the event handling function. Called by the application. Can be NULL to disable event handling.
		static void RegisterEventHandler(FEHUDEventHandler _pHandlerFunc);

	protected:

		static FEHUDEventHandler m_pHandlerFunc;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
