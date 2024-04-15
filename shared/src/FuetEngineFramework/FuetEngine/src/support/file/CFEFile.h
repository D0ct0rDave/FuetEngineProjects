// ----------------------------------------------------------------------------
/*! \class CFEFile
 *  \brief SFile class implementation
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
        virtual FEBool bOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode);

        virtual void Close ();

        virtual uint uiRead(FEPointer _pData, uint _uiSize);

        virtual uint uiWrite(FEPointer _pData, uint _uiSize);

		virtual uint uiReadArray32(FEPointer _pData, uint _uiSize);

		virtual uint uiReadArray16(FEPointer _pData, uint _uiSize);

        virtual int iSeek(int _iOffset, EFEFileSeekMode _eMode);

        virtual uint uiLength();

        virtual uint uiPos();

        FEHandler hGetHandler() { return (m_hFD); };

		/// Returns a pointer of allocated data with the contents of the file.
		static FEPointer pReadFile(const CFEString& _sFilename, uint* _puiSize = NULL);

  protected:
        // Data Members for Class Attributes
        FEHandler    m_hFD;
        CFEString    m_sFilename;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
