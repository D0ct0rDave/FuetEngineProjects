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
#ifndef CEntityRegistryH
#define CEntityRegistryH
// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "CEntity.h"
#include "CActor.h"
// -----------------------------------------------------------------------------
class CEntityRegistry
{
	public:

		/// Resets the entity registry.
		static void Reset();

		/// Adds a new entity into the registry.
		static uint uiAddEntity(CEntity* _poEnt,const CFEString& _sEntityName,bool _bActor = false);

		/// Deletes an entity from the registry.
		static void DeleteEntity(CEntity* _poEnt);

		/// Retrieves an entity linked to the given entity name.
		static CEntity* poGetEntity(const CFEString& _sEntityName);

		/// Retrieves an actor linked to the given actor name or NULL if it doesn't find any entity or
		/// the entity was not stored as an actor into the entity registry.
		static CActor* poGetActor(const CFEString& _sActorName);

   protected:
		
		typedef struct TRegistryEntry
		{
			CEntity*	m_poEntity;
			CFEString	m_sName;
			bool		m_bActor;

		}TRegistryEntry;
		
		static CFEArray<TRegistryEntry> m_oRegistry;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
