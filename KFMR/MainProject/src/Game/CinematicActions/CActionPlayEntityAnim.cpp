// -----------------------------------------------------------------------------
/*! \class CActionPlayEntityAnim
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionPlayEntityAnim.h"
#include "Game/Entity/CEntityRegistry.h"
#include "Game/Entity/CActor.h"
// -----------------------------------------------------------------------------
void CActionPlayEntityAnim::Init(const CFEString& _sEntityName,const CFEString& _sAnim)
{
	m_sEntityName	= _sEntityName;
	m_sAnim			= _sAnim;
}
// -----------------------------------------------------------------------------
bool CActionPlayEntityAnim::bUpdate(FEReal _rDeltaT)
{
	CActor* poActor = CEntityRegistry::poGetActor(m_sEntityName);

	if (poActor != NULL)
	{
		FEHandler hAnimInst = poActor->hGetAnimInst();
		if (hAnimInst != NULL)
		{
			int iAction = CFESkelAnimInstMgr::poGetAnim(hAnimInst)->poGetAnimActionSet()->iGetActionIdx(m_sAnim);
			if (iAction != -1)
				CFESkelAnimInstMgr::SetAction(hAnimInst,iAction);
		}
	}

	return(false);
}
// -----------------------------------------------------------------------------
void CActionPlayEntityAnim::Reset()
{
}
// -----------------------------------------------------------------------------
