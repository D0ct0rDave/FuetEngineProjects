// ----------------------------------------------------------------------------
/*! \class CFESkelAnimEventMgr
 *  \brief A class to manage the logic of event handling.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimEventMgrH
#define CFESkelAnimEventMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "CFESkelAnimEventFunc.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
//-----------------------------------------------------------------------------
class CFESkelAnimEventMgr
{
	friend class CFESkelAnimEventFunc;

	public:

		/// Registers the event handling function. Called by the application. Can be NULL to disable event handling.
		static void RegisterEventHandler(FESkelAnimEventHandler _pHandlerFunc);
	
	protected:

		/// Performs the call to the event handling function. Usually only to be called by the engine.
		static void TriggerEvent(const CFEString& _sEventName,const CFEString& _sEventValue,CFESkelAnimNode* _poNode,FEPointer _pParam);

	protected:

		static FESkelAnimEventHandler m_pHandlerFunc;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
