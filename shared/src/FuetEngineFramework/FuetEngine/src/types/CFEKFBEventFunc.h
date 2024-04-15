// ----------------------------------------------------------------------------
/*! \class CFEKFBEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEKFBEventFuncH
#define CFEKFBEventFuncH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "types/CFEKFBFunc.h"
//-----------------------------------------------------------------------------
class CFEEvent
{
	public:

		/// Default constructor
		CFEEvent() : 
			m_sEventName(""),
			m_sEventValue("")
		{
		};

		/// Constructor with initialization.
		CFEEvent(const CFEString& _sEventName,const CFEString& _sEventValue)
		{
			m_sEventName	= _sEventName;
			m_sEventValue	= _sEventValue;
		}

		// Marranada para poder reaprovechar CFEKFBFuncs para manejar funciones de eventos.
        /// CFEEvent operator *(const FEReal& _rFactor)
        CFEEvent operator *(const FEReal&) const {return *this; } 		// <- avoid unused parameter warning

        /// CFEEvent& operator *=(const FEReal& _rFactor)
        CFEEvent& operator *=(const FEReal&) {return *this;} 			// <- avoid unused parameter warning
        
        CFEEvent operator +(const CFEEvent&) const { return *this; }

        CFEEvent& operator +=(const CFEEvent&) { return *this; }

		static const CFEEvent& oNULL();

	public:

		CFEString	m_sEventName;
		CFEString	m_sEventValue;
};
//-----------------------------------------------------------------------------
template<> CFEEvent default_value<CFEEvent>(const CFEEvent& v);
// ----------------------------------------------------------------------------
// explicit specialization for CFEEvent type
template <>
// inline CFEEvent CFEKFBFunc<CFEEvent>::oLerp(uint _uiCurKey,uint _uiNextKey,const FEReal& _rFact,EFEKFLerpFunc _eLerpFunc)
inline CFEEvent CFEKFBFunc<CFEEvent>::oLerp(uint _uiCurKey,uint,const FEReal&,EFEKFLerpFunc)
{
	// always:º
	return(m_oKeyFrames[_uiCurKey].m_oKey);
}
//-----------------------------------------------------------------------------
class CFEKFBEventFunc : public CFEKFBFunc<CFEEvent>
{
	public:
		
		/// Default constructor for the class.
		CFEKFBEventFunc() : 
			m_rLastT(-_1r)
		{
		};

		/// Resets the event function to its initial state
		void Reset()
		{
			m_rLastT = -_1r;
		}

		/// Check whether to trigger the event or not.
		void Check(const FEReal& _rTime);


	protected:

		// This function triggers the event. Can be overriden for specific objects.
		virtual void TriggerEvent(const CFEEvent& _oEvent) = 0;

		/// The T of the previously called event.
		FEReal					m_rLastT;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
