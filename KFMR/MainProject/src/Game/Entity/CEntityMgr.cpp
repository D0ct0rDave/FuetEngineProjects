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
#include "CEntityMgr.h"
// -----------------------------------------------------------------------------
CEntity* CEntityMgr::m_poList[MAX_ENTITIES];
uint CEntityMgr::m_uiCurEnt = 0;
// -----------------------------------------------------------------------------
void CEntityMgr::Reset()
{
	// Delete all entities
	for (uint e=0; e<MAX_ENTITIES;e++)
		m_poList[e] = NULL;

	m_uiCurEnt = 0;
}
// -----------------------------------------------------------------------------
void CEntityMgr::Update(FEReal _rDeltaT)
{
	for (uint e=0;e<MAX_ENTITIES;e++)
		if ((m_poList[e] != NULL) && (m_poList[e]->bEnabled()))
			m_poList[e]->Update(_rDeltaT);
}
// -----------------------------------------------------------------------------
int CEntityMgr::iAddEntity(CEntity* _poEnt)
{
   // handle circular queue
   uint uiTestEnts = 0;

   do {

       if (m_poList[m_uiCurEnt] == NULL)
       {
           m_poList[m_uiCurEnt] = _poEnt;
           return(m_uiCurEnt);
       }

       m_uiCurEnt = (m_uiCurEnt + 1) % MAX_ENTITIES;
       uiTestEnts++;

   } while ( uiTestEnts < MAX_ENTITIES );

   return(-1);
}
// -----------------------------------------------------------------------------
void CEntityMgr::DeleteEntity(CEntity* _poEnt)
{
   for (uint e=0;e<MAX_ENTITIES;e++)
       if ( m_poList[e] == _poEnt )
           m_poList[e] = NULL;
}
// -----------------------------------------------------------------------------
CEntity* CEntityMgr::poGetEntity(uint _uiEntID)
{
   return (m_poList[_uiEntID]);
}
// -----------------------------------------------------------------------------
