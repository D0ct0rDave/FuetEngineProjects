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
#include "CFEMemFile.h"
#include "core/CFECore.h"
// -----------------------------------------------------------------------------
CFEMemFile::CFEMemFile() : m_pData(NULL), m_uiSize(0), m_uiPos(0)
{
}
// -----------------------------------------------------------------------------
CFEMemFile::~CFEMemFile()
{
}
// -----------------------------------------------------------------------------
FEBool CFEMemFile::bOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
	m_pData = CFECore::File::pReadFile(_sFilename,&m_uiSize);
	m_uiPos = 0;
	if (m_pData != NULL)
	{
		m_sFilename = _sFilename;
	}

	return(m_pData!=NULL);
}
// -----------------------------------------------------------------------------
void CFEMemFile::Close()
{
	CFECore::Mem::Free(m_pData);
	m_pData  = NULL;
	m_uiSize = 0;
	m_uiPos  = 0;
}
// -----------------------------------------------------------------------------
uint CFEMemFile::uiRead(FEPointer _pData, uint _uiSize)
{
	uint uiBytes = _uiSize;
	if ((m_uiPos + _uiSize) > m_uiSize)
		uiBytes = m_uiSize - m_uiPos;

	memcpy(_pData,m_pData + m_uiPos,uiBytes);
	m_uiPos += uiBytes;

	return(uiBytes);
}
// -----------------------------------------------------------------------------
int CFEMemFile::iSeek(int _iOffset, EFEFileSeekMode _eMode)
{
	switch (_eMode)
    {
        case FSM_CUR:
		{
			m_uiPos += _iOffset;
			if (m_uiPos > m_uiSize)
				m_uiPos = m_uiSize;
		}
        break;
		
		case FSM_END:
		{
			if (_iOffset<0)
			{
				if ( (uint)(-_iOffset) > m_uiPos)
					m_uiPos = 0;
				else
					m_uiPos += _iOffset;
			}
			else
			{
				m_uiPos += _iOffset;
				if (m_uiPos > m_uiSize)
					m_uiPos = m_uiSize;
			}
        }
        break;

        case FSM_SET:
        {
			if (_iOffset<0)
			{
				m_uiPos = 0;
			}
			else
			{
				m_uiPos = _iOffset;
				if (m_uiPos > m_uiSize)
					m_uiPos = m_uiSize;
			}
        }
        break;
    }

    return(m_uiPos);
}
// -----------------------------------------------------------------------------
uint CFEMemFile::uiLength()
{
	return(m_uiSize);
}
// -----------------------------------------------------------------------------
uint CFEMemFile::uiPos()
{
	return (m_uiPos);
}
// -----------------------------------------------------------------------------
