// -----------------------------------------------------------------------------
/*! \class CActionPerturbCamera
 *  \brief This action moves an entity from an initial place to a destination in a given amount of time.
 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionPerturbCameraH
#define CActionPerturbCameraH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/Helpers/HUDActions.h"
// -----------------------------------------------------------------------------
class CActionPerturbCamera : public CFEAction
{
	public:

		void Init(FEReal _rTime,FEReal _rXMag,FEReal _rYMag);
		
		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

	protected:
        
        FEReal m_rPerturbTime;
        FEReal m_rXMag;
        FEReal m_rYMag;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------