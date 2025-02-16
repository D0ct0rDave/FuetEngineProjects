// ----------------------------------------------------------------------------
/*! \class CFEChrono
 *  \brief A class to provide timming meassurement.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "System/CFESystem.h"
#include "FEConsts.h"
#include "CFEChrono.h"
// ----------------------------------------------------------------------------
void CFEChrono::Start()
{
    m_rTime     = CFESystem::rGetEngineTime();
    m_bStarted  = true;
}
// ----------------------------------------------------------------------------
void CFEChrono::Stop()
{
    m_bStarted   = false;
    m_rTime = CFESystem::rGetEngineTime() - m_rTime;
}
// ----------------------------------------------------------------------------
FEReal CFEChrono::rGetElapsedTime()
{
    if (m_bStarted)
        return(CFESystem::rGetEngineTime() - m_rTime);
    else
        return(m_rTime);
}
// ----------------------------------------------------------------------------
