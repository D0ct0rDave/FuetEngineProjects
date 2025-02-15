// -----------------------------------------------------------------------------
/*! \class CActionPerturbCamera
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionPerturbCamera.h"
#include "Game/Helpers/CCameraMgr.h"
// -----------------------------------------------------------------------------
void CActionPerturbCamera::Init(FEReal _rTime,FEReal _rXMag,FEReal _rYMag)
{
    m_rPerturbTime = _rTime;
    m_rXMag = _rXMag;
    m_rYMag = _rYMag;
}
// -----------------------------------------------------------------------------
bool CActionPerturbCamera::bUpdate(FEReal _rDeltaT)
{
    CCameraMgr::Perturb(m_rXMag,m_rYMag,m_rPerturbTime);
    return(false);
}
// -----------------------------------------------------------------------------
