// -----------------------------------------------------------------------------
/*! \class CActionMoveEntity
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionMoveEntity.h"
#include "Game/Entity/CEntityRegistry.h"
// -----------------------------------------------------------------------------
void CActionMoveEntity::Init(const CFEString& _sEntityName,const CFEVect2& _oInitialPos,const CFEVect2& _oFinalPos,FEReal _rTime)
{
	m_sEntityName	= _sEntityName;
	m_oInitialPos	= _oInitialPos;
	m_oFinalPos		= _oFinalPos;
	m_rTotalTime    = _rTime;
}
// -----------------------------------------------------------------------------
bool CActionMoveEntity::bUpdate(FEReal _rDeltaT)
{
	if (m_rTotalTime>_0r)
	{
		m_rTime -= _rDeltaT;
		if (m_rTime<=_0r) m_rTime = _0r;

		if (m_poEntity != NULL)
		{
			CFEVect2 oNewPos;
			oNewPos.Lerp(m_oFinalPos,m_oInitialPos,m_rTime / m_rTotalTime);
			m_poEntity->SetPos(oNewPos);

		}

		return(m_rTime>_0r);
	}
	else
	{
		if (m_poEntity != NULL)
			m_poEntity->SetPos(m_oInitialPos);

		return(false);
	}	
}
// -----------------------------------------------------------------------------
void CActionMoveEntity::Reset()
{
	m_poEntity = CEntityRegistry::poGetEntity(m_sEntityName);
	m_rTime = m_rTotalTime;
}
// -----------------------------------------------------------------------------
