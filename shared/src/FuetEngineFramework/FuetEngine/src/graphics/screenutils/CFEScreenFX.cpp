// ----------------------------------------------------------------------------
/*! \class CFEScreenFX
 *  \brief CFEScreenFX Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEScreenFX.h"
#include "CFEScreenUtils.h"
// ----------------------------------------------------------------------------
FEHandler   CFEScreenFX::m_hMaterial = NULL;
CFEColor    CFEScreenFX::m_oFadeColor;
FEReal      CFEScreenFX::m_rIniTime = _0r;
FEReal      CFEScreenFX::m_rTime = _0r;
CFEColor    CFEScreenFX::m_oIColor;
CFEColor    CFEScreenFX::m_oFColor;
CFEColor    CFEScreenFX::m_oColor;
// ----------------------------------------------------------------------------
void CFEScreenFX::SetBackground(FEHandler _hMat,const CFEColor& _oColor)
{
    m_hMaterial  = _hMat;
    m_oFadeColor = _oColor;
}
// ----------------------------------------------------------------------------
void CFEScreenFX::FadeIn(FEReal _rTime)
{
    m_rIniTime= _rTime;
    m_rTime   = _rTime;
    m_oIColor = m_oFadeColor;	m_oIColor.a = _0r;
    m_oFColor = m_oFadeColor;
    m_oColor  = m_oIColor;
}
// ----------------------------------------------------------------------------
void CFEScreenFX::FadeOut(FEReal _rTime)
{
    m_rIniTime= _rTime;
    m_rTime   = _rTime;
    m_oIColor = m_oFadeColor;
    m_oFColor = m_oFadeColor;	m_oFColor.a = _0r;
    m_oColor  = m_oIColor;
}
// ----------------------------------------------------------------------------
void CFEScreenFX::Update(FEReal _rDeltaT)
{
    if (m_rTime > _0r)
    {
        m_rTime -= _rDeltaT;
        if (m_rTime < _0r)
			m_rTime = _0r;

        m_oColor.Lerp(m_oFColor,m_oIColor,m_rTime / m_rIniTime);
    }
}
// ----------------------------------------------------------------------------
void CFEScreenFX::Render(CFERenderer* _poRenderer)
{
    if (m_oColor.a > _0r)
        CFEScreenUtils::FSRender(_poRenderer,m_hMaterial,m_oColor);
}
// ----------------------------------------------------------------------------
bool CFEScreenFX::bPlaying()
{
	return (m_rTime > _0r);
}
// ----------------------------------------------------------------------------
