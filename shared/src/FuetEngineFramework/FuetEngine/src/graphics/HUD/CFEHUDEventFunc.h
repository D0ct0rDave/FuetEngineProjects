// ----------------------------------------------------------------------------
/*! \class CFEEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEEventFuncH
#define CFEEventFuncH
//-----------------------------------------------------------------------------
#include "types/CFEKFBEventFunc.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFEHUDObject;
//-----------------------------------------------------------------------------
typedef void (*FEHUDEventHandler)(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj,FEPointer _pParam);
//-----------------------------------------------------------------------------
class CFEHUDEventFunc : public CFEKFBEventFunc
{
	public:

		/// Default constructor for the class.
		CFEHUDEventFunc() : 
			m_poObj(NULL),
			m_pHandlerFunc(NULL),
			m_pParam(NULL)
		{
		};

		/// Sets the HUD object associated with this event function
		void SetObject(CFEHUDObject* _poObj)
		{
			m_poObj = _poObj;
		}

		void SetEventHandler(FEHUDEventHandler _pHandlerFunc,FEPointer _pParam)
		{
			m_pHandlerFunc = _pHandlerFunc;
			m_pParam = _pParam;
		}

	protected:
		
		virtual void TriggerEvent(const CFEEvent& _oEvent);
	
	protected:

		/// Object associated to this event function
		CFEHUDObject*			m_poObj;

		/// Specific event handler for this function
		FEHUDEventHandler		m_pHandlerFunc;

		/// Parameter to be passed to the specific event handler function
		FEPointer				m_pParam;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
