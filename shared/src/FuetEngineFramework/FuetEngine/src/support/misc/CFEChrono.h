// ----------------------------------------------------------------------------
/*! \class CFEChrono
 *  \brief A class to provide timming meassurement.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEChronoH
#define CFEChronoH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFEChronoData;
class CFEChrono
{
    public:

		/// Default constructor for the chrono object.
		CFEChrono();

		/// Destructor for the class.
		~CFEChrono();

        /// Starts the chronometer.
        void Start();

        /// Stops the chronometer.
        void Stop();

        /// Retrieves the elapsed time since the start time until the chrono was stopped, or until the calling moment if not stopped.
        FEReal rGetElapsedTime();

		/// Retrieves whether the chrono is currently active or it is stopped.
		FEBool bActive();

    protected:

		CFEChronoData* m_poData;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
