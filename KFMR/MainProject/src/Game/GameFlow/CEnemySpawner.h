//-----------------------------------------------------------------------------
/*! \class CEnemySpawner
 *  \brief A generic class that will spawn enemies in the level according to a set of predefined rules.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEnemySpawnerH
#define CEnemySpawnerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CEntity;
//-----------------------------------------------------------------------------
class CEnemySpawner
{
	public:

		/// 
		virtual void Reset() = 0;

		/// 
		virtual void Update(FEReal _rDeltaT) = 0;
		
		//
		virtual void NotifyDeath(CEntity* _poEnt) = 0;

	protected:

		FEReal m_rTime;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
