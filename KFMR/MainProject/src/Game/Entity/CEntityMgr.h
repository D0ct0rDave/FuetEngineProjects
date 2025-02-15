// -----------------------------------------------------------------------------
/*! \class CEntityMgr
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CEntityMgrH
#define CEntityMgrH
// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "CEntity.h"
// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
const uint MAX_ENTITIES = 64;

class CEntityMgr
{
	public:

		/// Resets the entity manager.
		static void Reset();
		
		/// Performs an updates step over all the active entities.
		static void Update(FEReal _rDeltaT);
		
		/// Adds a new entity into the system.
		static int iAddEntity(CEntity* _poEnt);
		
		/// Deletes an entity from the system.
		static void DeleteEntity(CEntity* _poEnt);
		
		/// Retrieves an entity linked to the given entity ID.
		static CEntity* poGetEntity(uint _uiEntID);

   public:

		static CEntity* m_poList[MAX_ENTITIES];
		static uint	m_uiCurEnt;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
