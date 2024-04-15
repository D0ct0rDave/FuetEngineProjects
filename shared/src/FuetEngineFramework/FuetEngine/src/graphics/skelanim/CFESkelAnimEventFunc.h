// ----------------------------------------------------------------------------
/*! \class CFESkelAnimEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimEventFuncH
#define CFESkelAnimEventFuncH
//-----------------------------------------------------------------------------
#include "types/CFEKFBEventFunc.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
//-----------------------------------------------------------------------------
typedef void (*FESkelAnimEventHandler)(const CFEString& _sEventName,const CFEString& _sEventValue,CFESkelAnimNode* _poNode,FEPointer _pParam);               
//-----------------------------------------------------------------------------
class CFESkelAnimEventFunc : public CFEKFBEventFunc
{
	public:

		/// Default constructor for the class.
		CFESkelAnimEventFunc() : 
			m_poNode(NULL),
			m_pHandlerFunc(NULL),
			m_pParam(NULL)
		{
		};

		/// Sets the HUD object associated with this event function
		void SetNode(CFESkelAnimNode* _poNode)
		{
			m_poNode = _poNode;
		}

		void SetEventHandler(FESkelAnimEventHandler _pHandlerFunc,FEPointer _pParam)
		{
			m_pHandlerFunc	= _pHandlerFunc;
			m_pParam		= _pParam;
		}

	protected:

		virtual void TriggerEvent(const CFEEvent& _oEvent);

	protected:

		/// Object associated to this event function
		CFESkelAnimNode*		m_poNode;

		/// Specific event handler for this function
		FESkelAnimEventHandler	m_pHandlerFunc;

		/// Parameter to be passed to the specific event handler function
		FEPointer				m_pParam;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
