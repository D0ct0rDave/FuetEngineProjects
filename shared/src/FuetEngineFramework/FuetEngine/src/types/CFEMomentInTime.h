// ----------------------------------------------------------------------------
/*! \class CFEMomentInTime
 *  \brief A class that holds information that represents a moment in time.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMomentInTimeH
#define CFEMomentInTimeH
// ----------------------------------------------------------------------------
class CFEMomentInTime
{
    public:
		
        CFEMomentInTime() :
			m_iYear(0),
			m_ucMonth(0),
			m_ucDay(0),
			m_ucHour(0),
			m_ucMinute(0),
			m_ucSecond(0),
			m_usMilis(0)
        {
        }        

		/// Retrieves the percentage of day passed
        FEReal rGetDayFactor()
        {
			return( FEReal (m_ucHour*60*60 + m_ucMinute*60+m_ucSecond) / FEReal (24*60*60));
        }

		/// Retrieves the number of miliseconds since the month started.
		uint uiGetMonthMilis()
		{
			uint uiRes = m_usMilis;
			uiRes += m_ucSecond*1000;
			uiRes += m_ucMinute*(60*1000);
			uiRes += m_ucHour*(60*60*1000);
			uiRes += m_ucDay*(24*60*60*1000);
			// uiRes += m_ucMonth*(31*24*60*60);
			// uiRes += m_iYear*(12*31*24*60*60);
			return(uiRes);
		}

		/// Retrieves the number of seconds since the month started.
		uint uiGetMonthSecs()
		{
			uint uiRes = m_ucSecond;
			uiRes += m_ucMinute*(60);
			uiRes += m_ucHour*(60*60);
			uiRes += m_ucDay*(24*60*60);
			return(uiRes);
		}

	public:

		int			  m_iYear;		// -inf..inf

		unsigned char m_ucMonth;	// 0..11
		unsigned char m_ucDay;		// 0..30
		unsigned char m_ucHour;		// 0..23
		unsigned char m_ucMinute;	// 0..59

		unsigned char  m_ucSecond;	// 0..59
		unsigned char  m_ucPAD;		//
		unsigned short m_usMilis;	// 0..999
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
