// ----------------------------------------------------------------------------
/*! \class CFEChrono
 *  \brief A class to provide timming meassurement.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "core/CFECore.h"
#include "FEConsts.h"
#include "CFEChrono.h"
// ----------------------------------------------------------------------------
class CFEChronoData
{
	public:

		CFEChronoData() : m_rTime(_0r), m_bStarted(false)
		{
		
		}

		FETick	m_oStartTick;
		FEReal	m_rTime;
		FEBool	m_bStarted;
};
// ----------------------------------------------------------------------------
CFEChrono::CFEChrono()
{
	m_poData = new CFEChronoData;
}
// ----------------------------------------------------------------------------
CFEChrono::~CFEChrono()
{
	delete m_poData;
}
// ----------------------------------------------------------------------------
void CFEChrono::Start()
{
    m_poData->m_oStartTick = CFECore::oGetEngineTicks();
    m_poData->m_bStarted    = true;
}
// ----------------------------------------------------------------------------
void CFEChrono::Stop()
{
    m_poData->m_bStarted = false;
    m_poData->m_rTime    = CFECore::rGetTickTime(CFECore::oGetEngineTicks() - m_poData->m_oStartTick);
}
// ----------------------------------------------------------------------------
FEBool CFEChrono::bActive()
{
	return(m_poData->m_bStarted);
}
// ----------------------------------------------------------------------------
FEReal CFEChrono::rGetElapsedTime()
{
    if (m_poData->m_bStarted)
        return(CFECore::rGetTickTime(CFECore::oGetEngineTicks() - m_poData->m_oStartTick));
    else
        return(m_poData->m_rTime);
}
// ----------------------------------------------------------------------------
