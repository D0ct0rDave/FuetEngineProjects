// ----------------------------------------------------------------------------
/*! \class CFEChrono
 *  \brief A class to provide timming meassurement.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEChronoH
#define CFEChronoH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFEChrono
{
    public:

		/// Default constructor for the chrono object.
		CFEChrono() : m_rTime(_0r), m_bStarted(false)
		{
		}

        /// Starts the chronometer.
        void Start();

        /// Stops the chronometer.
        void Stop();

        /// Retrieves the elapsed time since the start time until the chrono was stopped, or until the calling moment if not stopped. 
        FEReal rGetElapsedTime();

    protected:

        /// 
        FEReal  m_rTime;
        bool    m_bStarted;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
