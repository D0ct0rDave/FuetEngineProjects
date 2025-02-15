// -----------------------------------------------------------------------------
/*! \class CEntityRegistry
 *  \brief A class to store unique entities that can be retrieved later.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CEntityRegistry.h"
// -----------------------------------------------------------------------------
CFEArray<CEntityRegistry::TRegistryEntry> CEntityRegistry::m_oRegistry;
// -----------------------------------------------------------------------------
/// Resets the entity registry.
void CEntityRegistry::Reset()
{
	m_oRegistry.clear();
}
// -----------------------------------------------------------------------------
/// Adds a new entity into the registry.
uint CEntityRegistry::uiAddEntity(CEntity* _poEnt,const CFEString& _sEntityName,bool _bActor)
{
    for (uint e=0;e<m_oRegistry.size();e++)
        if (m_oRegistry[e].m_sName |= _sEntityName)
            return(e);

	TRegistryEntry oEntry;
	oEntry.m_poEntity	= _poEnt;
	oEntry.m_sName		= _sEntityName;
	oEntry.m_bActor		= _bActor;

	m_oRegistry.push_back(oEntry);
	return(m_oRegistry.size()-1);
}
// -----------------------------------------------------------------------------
/// Deletes an entity from the registry.
void CEntityRegistry::DeleteEntity(CEntity* _poEnt)
{
	for (uint e=0;e<m_oRegistry.size();e++)
		if ( m_oRegistry[e].m_poEntity == _poEnt )
		{
			m_oRegistry.Delete(e);
			return;
		}
}
// -----------------------------------------------------------------------------
/// Retrieves an entity linked to the given entity name.
CEntity* CEntityRegistry::poGetEntity(const CFEString& _sEntityName)
{
	for (uint e=0;e<m_oRegistry.size();e++)
		if ( m_oRegistry[e].m_sName |= _sEntityName)
			return(m_oRegistry[e].m_poEntity);
	
	return(NULL);
}
// -----------------------------------------------------------------------------
/// Retrieves an actor linked to the given actor name or NULL if it doesn't find any entity or
/// the entity was not stored as an actor into the entity registry.
CActor* CEntityRegistry::poGetActor(const CFEString& _sActorName)
{
	for (uint e=0;e<m_oRegistry.size();e++)
		if ( m_oRegistry[e].m_sName |= _sActorName)
		{
			if (m_oRegistry[e].m_bActor == false)
				return(NULL);
			else
				return((CActor*)m_oRegistry[e].m_poEntity);
		}

	return(NULL);
}
// -----------------------------------------------------------------------------
