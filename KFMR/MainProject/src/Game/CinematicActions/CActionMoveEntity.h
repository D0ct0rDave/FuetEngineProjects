// -----------------------------------------------------------------------------
/*! \class CActionMoveEntity
 *  \brief This action moves an entity from an initial place to a destination in a given amount of time.
 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionMoveEntityH
#define CActionMoveEntityH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
// -----------------------------------------------------------------------------
class CEntity;
// -----------------------------------------------------------------------------
class CActionMoveEntity : public CFEAction
{
	public:

		void Init(const CFEString& _sEntityName,const CFEVect2& _oInitialPos,const CFEVect2& _oFinalPos,FEReal _rTime);
		
		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();
	
	protected:

		CEntity*	m_poEntity;
		CFEString	m_sEntityName;
		CFEVect2	m_oInitialPos;
		CFEVect2	m_oFinalPos;
		FEReal		m_rTotalTime;
		FEReal		m_rTime;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------