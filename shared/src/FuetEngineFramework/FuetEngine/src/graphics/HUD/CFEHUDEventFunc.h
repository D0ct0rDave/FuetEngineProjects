// ----------------------------------------------------------------------------
/*! \class CFEHUDEventFunc
 *  \brief A class to store an event function.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDEventFuncH
#define CFEHUDEventFuncH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "types/CFEKFBFunc.h"
#include "CFEHUDEventMgr.h"
//-----------------------------------------------------------------------------
typedef void (*FEHUDSpecEventHandler)(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj,FEPointer _pParam);

class CFEHUDEvent
{
	public:

		/// Default constructor
		CFEHUDEvent() : m_sEventName(""),m_sEventValue(""), m_rEventTime(_0r)
		{
		};

		/// Constructor with initialization.
		CFEHUDEvent(const CFEString& _sEventName,const CFEString& _sEventValue)
		{
			m_sEventName	= _sEventName;
			m_sEventValue	= _sEventValue;
		}

		/// Sets the event time
		void SetEventTime(FEReal _rEventTime)
		{
			m_rEventTime = _rEventTime;
		}

		/// Sets the event time
		FEReal rGetEventTime()
		{
			return(m_rEventTime);
		}

		// Marranada para poder reaprovechar CFEKFBFuncs para manejar funciones de eventos.
        /// CFEHUDEvent operator *(const FEReal& _rFactor)
        CFEHUDEvent operator *(const FEReal&) const {return *this; } 		// <- avoid unused parameter warning

        /// CFEHUDEvent& operator *=(const FEReal& _rFactor)
        CFEHUDEvent& operator *=(const FEReal&) {return *this;} 			// <- avoid unused parameter warning
        
        CFEHUDEvent operator +(const CFEHUDEvent&) const { return *this; }

        CFEHUDEvent& operator +=(const CFEHUDEvent&) { return *this; }

	public:

		CFEString	m_sEventName;
		CFEString	m_sEventValue;
	
	protected:

		FEReal		m_rEventTime;
};
// ----------------------------------------------------------------------------
// explicit specialization for CFEHUDEvent type
template <>
// inline CFEHUDEvent CFEKFBFunc<CFEHUDEvent>::oLerp(uint _uiCurKey,uint _uiNextKey,const FEReal& _rFact,EFEKFLerpFunc _eLerpFunc)
inline CFEHUDEvent CFEKFBFunc<CFEHUDEvent>::oLerp(uint _uiCurKey,uint,const FEReal&,EFEKFLerpFunc)
{
	// always:
	return(m_oKeyFrames[_uiCurKey].m_oKey);
}
//-----------------------------------------------------------------------------
class CFEHUDEventFunc : public CFEKFBFunc<CFEHUDEvent>
{
	public:
		
		/// Default constructor for the class.
		CFEHUDEventFunc() : 
			m_poObj(NULL),
			m_rLastEventT(-_1r),
			m_pHandlerFunc(NULL),
			m_pParam(NULL)
		{
		};

		/// Resets the event function to its initial state
		void Reset()
		{
			m_rLastEventT = -_1r;
		}

		/// Check whether to perform an should be triggered or not.
		void Check(const FEReal& _rTime);

		/// Sets the HUD object associated with this event function
		void SetObject(CFEHUDObject* _poObj)
		{
			m_poObj = _poObj;
		}

		/// Retrieves the time when the last event was triggered
		FEReal rGetLastEventT()
		{
			return(m_rLastEventT);
		}

		/// Sets the time when the last event was triggered. 
		void SetLastEventT(FEReal _rLastEventT)
		{
			m_rLastEventT = _rLastEventT;
		}

		void SetSpecEventHandler(FEHUDSpecEventHandler _pHandlerFunc,FEPointer _pParam)
		{
			m_pHandlerFunc = _pHandlerFunc;
			m_pParam = _pParam;
		}

	protected:

		/// Object associated to this event function
		CFEHUDObject*			m_poObj;

		/// The T of the previously called event.
		FEReal					m_rLastEventT;
		
		/// Specific event handler for this function
		FEHUDSpecEventHandler	m_pHandlerFunc;

		/// Parameter to be passed to the specific event handler function
		FEPointer				m_pParam;
};
//-----------------------------------------------------------------------------
inline void CFEHUDEventFunc::Check(const FEReal& _rTime)
{
	if (m_oKeyFrames.size() == 0 ) return;
	if ((m_oKeyFrames.size() == 1) && (m_oKeyFrames[0].m_oKey.m_sEventName.uiLen() == 0)) return;
	// if (m_oKeyFrames[0].m_oKey.m_sEventName == CFEString::sNULL()) return;

	// Handle T of the function...
	CFEHUDEvent oVal = oGetValue(_rTime);
	if ((oVal.rGetEventTime() != m_rLastEventT))
	{
		if (oVal.m_sEventName != "")
		{
			// CALL event handler
			if (m_pHandlerFunc== NULL)
				CFEHUDEventMgr::TriggerEvent(oVal.m_sEventName,oVal.m_sEventValue,m_poObj);
			else
				m_pHandlerFunc(oVal.m_sEventName,oVal.m_sEventValue,m_poObj,m_pParam);
		}

		m_rLastEventT = oVal.rGetEventTime();
	}
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
