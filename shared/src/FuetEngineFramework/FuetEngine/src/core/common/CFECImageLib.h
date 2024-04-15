// ----------------------------------------------------------------------------
/*! \class TexHelper
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFECImageLibH
#define CFECImageLibH
// ----------------------------------------------------------------------------
#include "FEEnums.h"
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFECImageInfo
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
    CFECImageInfo* poLoadImage(char* _szFilename);
    void MakePowerOf2(CFECImageInfo* _poImageInfo);
    void FreeImage(CFECImageInfo* _poImageInfo);
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

