//-----------------------------------------------------------------------------
/*! \class CCameraMgr
 *  \brief Helper class to retrieve the ground level at a certain map place.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CCameraMgr.h"
#include "Game/CGameGlobals.h"
//-----------------------------------------------------------------------------
static FEReal m_rXMag = 0.0f;
static FEReal m_rYMag = 0.0f;
static FEReal m_rTime = 0.0f;
static FEReal m_rTotalTime = 0.0f;
static CFEVect2 m_oPos;
//-----------------------------------------------------------------------------
// ADSR Model??? Attack / Decay / Sustain / Release ??
//-----------------------------------------------------------------------------
void CCameraMgr::Init(uint _uiLevel)
{
    Reset();
}
//-----------------------------------------------------------------------------
void CCameraMgr::Finish()
{
}
//-----------------------------------------------------------------------------
void CCameraMgr::Reset()
{
    m_rXMag         = 0.0f;
    m_rYMag         = 0.0f;
    m_rTime         = 0.0f;
    m_rTotalTime    = 0.0f;
}
//-----------------------------------------------------------------------------
void CCameraMgr::Perturb(FEReal _rTime,FEReal _rXMagnitude,FEReal _rYMagnitude)
{
    m_rXMag         = _rXMagnitude;
    m_rYMag         = _rYMagnitude;
    m_rTotalTime    = _rTime;
    m_rTime         = _rTime;
}
//-----------------------------------------------------------------------------
void CCameraMgr::Perturb(FEReal _rTime,FEReal _rMagnitude)
{
    Perturb(_rTime,_rMagnitude,_rMagnitude);
}
//-----------------------------------------------------------------------------
const CFEVect2& CCameraMgr::oGetPos()
{
    return (m_oPos);
}	
//-----------------------------------------------------------------------------
const FEReal CCameraMgr::rGetZoom()
{
    return (CAMERA_ZOOM);
}
//-----------------------------------------------------------------------------
void CCameraMgr::Update(FEReal _rDeltaT)
{
    m_oPos = goGameGlobals.m_poCamera->oGetPos();
    
    if (m_rTime > _0r)
    {
        CFEVect2 oOfs;
        const FEReal PERT_XFREQ = _2PIr * 11.0; // use prime numbers to avoid cycles
        const FEReal PERT_YFREQ = _2PIr * 17.0;

        oOfs.x = m_rXMag * CFEMath::rSin( m_rTime * PERT_XFREQ );
        oOfs.y = m_rYMag * CFEMath::rSin( m_rTime * PERT_YFREQ );
        FEReal rFact = (m_rTime / m_rTotalTime);

        rFact *= rFact; // <-- Exponential perturbation.
        oOfs *= rFact;

        m_oPos += oOfs;
        m_rTime -= _rDeltaT;
    }
}
//-----------------------------------------------------------------------------
