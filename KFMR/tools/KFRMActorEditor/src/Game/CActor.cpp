//-----------------------------------------------------------------------------
/*! \class CActor
 *  \brief The base class for the actor characters in the game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CActor.h"
//-----------------------------------------------------------------------------
// Player actions
//-----------------------------------------------------------------------------
CActor::CActor() : m_hAnimInst(NULL), m_poFace(NULL), m_rDirMult(1), m_rScale(1)
{
}
//-----------------------------------------------------------------------------
/// Sets the position of the entity.
void CActor::SetPos(const CFEVect2& _oPos)
{
	CLivingEntity::SetPos(_oPos);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
}
//-----------------------------------------------------------------------------
