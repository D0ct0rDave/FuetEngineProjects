// -----------------------------------------------------------------------------
/*! \class CActionPlayHUDAnim
 *  \brief This action moves an entity from an initial place to a destination in a given amount of time.
 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionPlayHUDAnimH
#define CActionPlayHUDAnimH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/Helpers/HUDActions.h"
// -----------------------------------------------------------------------------
class CActionPlayHUDAnim : public CFEAction
{
	public:

		void Init(const CFEString& _sActionName);
		
		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

	protected:
        
        EHUDAction m_eHUDAction;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------