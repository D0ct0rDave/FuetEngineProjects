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
#include "CActionSpawnEntity.h"
#include "Game/Entity/CEntityRegistry.h"
// -----------------------------------------------------------------------------
void CActionSpawnEntity::Init(const CFEString& _sEntityName,const CFEVect2& _oPos,bool _bLeft)
{
	m_sEntityName	= _sEntityName;
	m_oPos			= _oPos;
	m_bLeft			= _bLeft;
}
// -----------------------------------------------------------------------------
bool CActionSpawnEntity::bUpdate(FEReal _rDeltaT)
{
	CEntity* poEnt = CEntityRegistry::poGetEntity(m_sEntityName);
	poEnt->Spawn(m_oPos);

	return(false);
}
// -----------------------------------------------------------------------------
