// ----------------------------------------------------------------------------
/*! \class CFEKFBEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#include "CFEKFBEventFunc.h"
#include "CFEKFBFunc.h"
//-----------------------------------------------------------------------------
static CFEEvent gsoDefaultEvent("","");
CFEEvent default_value(const CFEEvent& v) { return(CFEEvent::oNULL()); };

template <>
CFEEvent default_value(CFEEvent const&v)
{
	return( CFEEvent::oNULL() );
};
//-----------------------------------------------------------------------------
const CFEEvent& CFEEvent::oNULL()
{
	return(gsoDefaultEvent);
}
//-----------------------------------------------------------------------------
void CFEKFBEventFunc::Check(const FEReal& _rTime)
{
	if (m_oKeyFrames.size() == 0 ) return;
	if ((m_oKeyFrames.size() == 1) && (m_oKeyFrames[0].m_oKey.m_sEventName.uiLen() == 0)) return;
	// if (m_oKeyFrames[0].m_oKey.m_sEventName == CFEString::sNULL()) return;
	
	// Ensure m_rLastEventT <= _rTime
	if (_rTime < m_rLastT) m_rLastT = _rTime;

	switch (m_eWrapMode)
	{
		case KFBFWM_LOOP:
		{
			FEReal rLastT,rTime;
			if (m_rLastT == -_1r)
			{
				rLastT = -_1r;
				rTime  = _rTime;
			}
			else
			{
				uint uiLoops  = 0;
				if (m_rMaxLimit>_0r)
					uiLoops = m_rLastT / m_rMaxLimit;

				rLastT = m_rLastT - (m_rMaxLimit*uiLoops);
				rTime  = _rTime - (m_rMaxLimit*uiLoops);
			}

			uint uiInitial = 0;
			while ((uiInitial<uiGetNumKeyFrames()) && (m_oKeyFrames[uiInitial].rGetKeyTime()<=rLastT))
				uiInitial++;
			
			if (uiInitial<uiGetNumKeyFrames()) // is the opposite possible?
			{
				uint i = uiInitial;
				while (rTime >= m_oKeyFrames[i].rGetKeyTime())
				{
					const CFEEvent& oVal = m_oKeyFrames[i].m_oKey;

					if (oVal.m_sEventName != "")
						TriggerEvent(oVal);

					i++;
					if (i==uiGetNumKeyFrames())
					{
						i = 0;
						if (m_rMaxLimit > _0r)
							rTime -= m_rMaxLimit;
						else
						{
							rTime = _0r;
							break;
						}
					}
				}
			}
		}
		break;

		default:
		case KFBFWM_INITIALVALUE:
		case KFBFWM_FINALVALUE:
		{
			if (m_rLastT > rGetMaxLimit())
			{
				// do nothing.
			}
			else
			{
				uint uiInitial = 0;
				while ((uiInitial<uiGetNumKeyFrames()) && (m_oKeyFrames[uiInitial].rGetKeyTime()<=m_rLastT))
					uiInitial++;

				uint i = uiInitial;
				while ( (i<uiGetNumKeyFrames()) && (_rTime >= m_oKeyFrames[i].rGetKeyTime()) )
				{
					const CFEEvent& oVal = m_oKeyFrames[i].m_oKey;

					if (oVal.m_sEventName != "")
						TriggerEvent(oVal);

					i++;
				}
			}
		}
		break;
	
		case  KFBFWM_PINGPONG:
		{
			/*
			if (rFact >= _2r*m_rMaxLimit)
			rFact = CFEMath::rMod(_rT,_2r*m_rMaxLimit);

			// WARNING:!!!! si pongo este condicional debajo del siguiente, CW peta!!!!!
            if (rFact == _0r)
            {
				return(oGetKeyValue(0));
            }

            // 0 <= rFact < 2*m_rMaxLimit
			if (rFact >= m_rMaxLimit)
				rFact = _2r*m_rMaxLimit - rFact;
				*/
		}
		break;	
	}

	m_rLastT = _rTime;
}
//-----------------------------------------------------------------------------
