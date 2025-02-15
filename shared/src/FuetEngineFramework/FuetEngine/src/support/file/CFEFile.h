// ----------------------------------------------------------------------------
/*! \class CFEFile
 *  \brief SFile class implementation
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------

#ifndef CFEFileH
#define CFEFileH
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CFEFile
{
  public:
        CFEFile();
        ~CFEFile();
        virtual bool bOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode);

        virtual void Close ();

        virtual uint uiRead(FEPointer _pData, uint _uiSize);

        virtual uint uiWrite(FEPointer _pData, uint _uiSize);

		virtual uint uiReadArray32(FEPointer _pData, uint _uiSize);

		virtual uint uiReadArray16(FEPointer _pData, uint _uiSize);

        virtual int iSeek(int _iOffset, EFEFileSeekMode _eMode);

        virtual uint uiLength();

        virtual uint uiPos();

        FEHandler hGetHandler() { return (m_hFD); };

  private:
        // Data Members for Class Attributes
        FEHandler    m_hFD;
        CFEString    m_sFilename;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
