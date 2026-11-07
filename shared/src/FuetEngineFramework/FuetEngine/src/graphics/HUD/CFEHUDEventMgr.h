// ----------------------------------------------------------------------------
/*! \class CFEHUDEventMgr
 *  \brief A class to manage the logic of event handling.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDEventMgrH
#define CFEHUDEventMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFEHUDObject;
typedef void (*FEHUDEventHandler)(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj);
//-----------------------------------------------------------------------------
class CFEHUDEventMgr
{
	public:

		/// Performs the call to the event handling function. Usually only to be called by the engine.
		static void TriggerEvent(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj);

		/// Registers the event handling function. Called by the application. Can be NULL to disable event handling.
		static void RegisterEventHandler(FEHUDEventHandler _pHandlerFunc);

	protected:

		static FEHUDEventHandler m_pHandlerFunc;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
