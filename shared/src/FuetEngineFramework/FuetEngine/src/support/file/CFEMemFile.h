// ----------------------------------------------------------------------------
/*! \class CFEMemFile
 *  \brief Config files functionality through BinaryConfigFiles.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMemFileH
#define CFEMemFileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "types/CFEString.h"
#include "CFEFile.h"
// ----------------------------------------------------------------------------
class CFEMemFile : public CFEFile
{
	public:

        CFEMemFile();
        ~CFEMemFile();

        virtual FEBool bOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode);

        virtual void Close ();

        virtual uint uiRead(FEPointer _pData, uint _uiSize);

        virtual int iSeek(int _iOffset, EFEFileSeekMode _eMode);

        virtual uint uiLength();

        virtual uint uiPos();

	protected:

		FEPointer		m_pData;
		uint			m_uiSize;
		uint			m_uiPos;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
