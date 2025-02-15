// -----------------------------------------------------------------------------
/*! \class CActionTest
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionTest.h"
#include "Game/Helpers/CSoundHelper.h"
// -----------------------------------------------------------------------------
void CActionTest::Init(CFEInput* _poInput)
{
	m_poInput = _poInput;
}
// -----------------------------------------------------------------------------
bool CActionTest::bUpdate(FEReal _rDeltaT)
{
	if (m_poInput->bDown(IB_A))
	{
		CSoundHelper::hPlay(SND_PLAYER_DEATH);
		return(false);
	}
	else
		return(true);
}
// -----------------------------------------------------------------------------
void CActionTest::Reset()
{
}
// -----------------------------------------------------------------------------