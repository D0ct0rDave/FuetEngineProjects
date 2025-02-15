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
#ifndef CFESoundH
#define CFESoundH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CFESound
{
    public:
        CFESound();
    
        bool            m_bLoop;    
        void*            m_pData;
        uint            m_uiSampleRate;
        EFESampleFormat    m_eSampleFormat;
        FEHandler        m_hHandle;
    
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
