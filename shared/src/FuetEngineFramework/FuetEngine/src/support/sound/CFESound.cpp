// ----------------------------------------------------------------------------
/*! \class CFESound
 *  \brief Sound Class Declaration
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESound.h"
// ----------------------------------------------------------------------------
CFESound::CFESound():
    
    m_bLoop(false),
    m_pData(NULL),
    m_uiSampleRate(0),
    m_eSampleFormat(SF_NONE),
    m_hHandle(NULL)
{

}
// ----------------------------------------------------------------------------
