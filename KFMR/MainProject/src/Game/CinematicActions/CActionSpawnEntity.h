// -----------------------------------------------------------------------------
/*! \class CActionSpawnEntity
 *  \brief This action spawns an entity to an initial position.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionSpawnEntityH
#define CActionSpawnEntityH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
// -----------------------------------------------------------------------------
class CActionSpawnEntity : public CFEAction
{
	public:

		void Init(const CFEString& _sEntityName,const CFEVect2& _oPos,bool _bLeft);

		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

	protected:

		CFEString	m_sEntityName;
		CFEVect2	m_oPos;
		bool		m_bLeft;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------