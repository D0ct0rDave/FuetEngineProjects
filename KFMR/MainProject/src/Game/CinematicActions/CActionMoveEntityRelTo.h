// -----------------------------------------------------------------------------
/*! \class CActionMoveEntityTo
 *  \brief This action moves an entity from its current position to a destination one in a given amount of time.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionMoveEntityRelToH
#define CActionMoveEntityRelToH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "CActionMoveEntity.h"
// -----------------------------------------------------------------------------
class CEntity;
// -----------------------------------------------------------------------------
class CActionMoveEntityRelTo : public CActionMoveEntity
{
	public:

		void Init(const CFEString& _sEntityName,const CFEVect2& _oFinalPos,FEReal _rTime);

		/// Resets the action to its initial state.
		virtual void Reset();

		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

	private:
		
		bool bStarted;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------