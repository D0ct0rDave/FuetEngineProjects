// ----------------------------------------------------------------------------
/*! \class TexHelper
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESImageLibH
#define CFESImageLibH
// ----------------------------------------------------------------------------
#include "FEEnums.h"
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFESImageInfo
{
    public:
        FEHandler       m_hHandler;
        void*           m_pData;
        uint            m_uiTX;
        uint            m_uiTY;
        uint            m_uiFixedTX;
        uint            m_uiFixedTY;
        uint            m_uiPitch;
        EFEPixelFormat  m_eFormat;
};
// ----------------------------------------------------------------------------

namespace ImageLib
{
    CFESImageInfo* poLoadImage(char* _szFilename);
    void MakePowerOf2(CFESImageInfo* _poImageInfo);
    void FreeImage(CFESImageInfo* _poImageInfo);
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

