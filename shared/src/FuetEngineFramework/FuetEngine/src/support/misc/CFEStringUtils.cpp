// ----------------------------------------------------------------------------
/*! \class CFEStringUtils
 *  \brief String Utilities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#include "CFEStringUtils.h"
#include "crc32.h"
#include "types/CFEArray.h"
//-----------------------------------------------------------------------------
namespace CFEStringUtils {
//-----------------------------------------------------------------------------
/// Retrieves the filename portion of a full qualified filename.
CFEString sGetFilename(const CFEString& _sFilename)
{
	CFEString sFilename = sNormalizePath(_sFilename);
    int iSlashPos = sFilename.iRPos("/");

	if (iSlashPos != -1)
		return( _sFilename.SubString(iSlashPos+1, _sFilename.uiLen() - (iSlashPos +1)) );
	else
		return( _sFilename );
}
//-----------------------------------------------------------------------------
CFEString sRemoveExtension(const CFEString& _sFilename)
{
    // Remove extension
    int iDotPos = _sFilename.iRPos(".");

    if (iDotPos == -1)
        return(_sFilename);
    else
    {
        // watch out for relative directories structures inside filenames!
        if ((iDotPos > 0) && (_sFilename[iDotPos-1] == '.'))
            return(_sFilename);

        return( _sFilename.SubString(0,iDotPos) );
    }
}
//-----------------------------------------------------------------------------
/// Retrieves the extension portion of a full qualified filename.
CFEString sGetExtension(const CFEString& _sFilename)
{
    // Retrieve extension
    int iDotPos = _sFilename.iRPos(".");

    if (iDotPos == -1)
        return("");
    else
        return( _sFilename.SubString(iDotPos,_sFilename.uiLen() - iDotPos +1) );
}
//-----------------------------------------------------------------------------
/// Retrieves the path portion of a full qualified filename.
CFEString sGetPath(const CFEString& _sFilename)
{
	CFEString sFilename = sNormalizePath(_sFilename);
    int iSlashPos = sFilename.iRPos("/");

    if (iSlashPos != -1)
		return( _sFilename.SubString(0,iSlashPos) );
	else
		return(".");
}
//-----------------------------------------------------------------------------
/// Changes all the backslashes to slashes in the path and returns the result.
CFEString sNormalizePath(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename;

	char* szFilename = (char*)sFilename.szString();

	for (uint i=0;i<sFilename.uiLen();i++)
		if (szFilename[i] == '\\')
			szFilename[i] = '/';

	return(sFilename);
}
// ----------------------------------------------------------------------------
CFEString sGetCanonicalPath(const CFEString& _sPath)
{
	if (_sPath == "") return(_sPath);

	// ----------------------------------------------
	// Are there really any relative directories?
	// ----------------------------------------------
	// if (_sPath.iRPos("./")==-1) return(_sPath);  // the code below is an optimization for this line
	const char* szOrigin	= _sPath.szString();
	const char* szStr		= szOrigin + _sPath.uiLen();
	bool	bCanonicalize   = false;
	do{
		szStr--;

		bCanonicalize = (szStr[0] == '.') && (szStr[1]=='/');

	}while ((! bCanonicalize) && (szStr != szOrigin));

	if (bCanonicalize == false) return(_sPath);
	// ----------------------------------------------
	const char* szPath = szOrigin;
	CFEArray<CFEString> m_sDirs;
	CFEString sDir = "";

	while (*szPath)
	{
		if (*szPath == '/')
		{
			m_sDirs.push_back(sDir);
			sDir = "";
		}
		else
		{
			sDir += *szPath;
		}

		szPath++;
	}

	uint i;
	for (i=0;i<m_sDirs.size();)
	{
		if (m_sDirs[i] == "..")
		{
			if ((i>0) && (m_sDirs[i-1] != ".."))
			{
				m_sDirs.Delete(i);
				m_sDirs.Delete(i-1);
				i--;
			}
			else
			{
				// can't do anything
				i++;
			}
		}
   else if (m_sDirs[i] == ".")
		{
			m_sDirs.Delete(i);
		}
	else
		{
			i++;
		}
	}

	CFEString sRes = "";
	for (i=0;i<m_sDirs.size();i++)
	{
		sRes += m_sDirs[i] + '/';
	}
	
	sRes += sDir;
	return(sRes);
}
//-----------------------------------------------------------------------------
/// Retrieves a CRC value from the given string.
uint uiGetCRC32(const char* _szBuffer, uint _uiSize)
{
    // This function uses the crc32_table lookup table
    // to generate a CRC for csData

	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.
	unsigned int crc		= 0xffffffff;
	unsigned char* buffer	= (unsigned char*)_szBuffer;
	unsigned int len		= _uiSize;

	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];

	// Exclusive OR the result with the beginning value.
	return crc ^ 0xffffffff;
}
//-----------------------------------------------------------------------------
/*
uint uiGetHash(const CFEString& _sString)
{
	// This function uses the crc32_table lookup table
    // to generate a CRC for csData

	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.
	unsigned int crc		= 0xffffffff;
	unsigned char* buffer	= (unsigned char*)_sString.szString();

	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(*buffer)
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];

	// Exclusive OR the result with the beginning value.
	return crc ^ 0xffffffff;
}
*/
// uses SAX hashing
uint uiGetHash(const char* _szString)
{
	unsigned char* p = (unsigned char*)_szString;
    if (p == NULL) return(0);

    uint h = 0;
    while (*p != 0)
    {
        h ^= (h << 5) + (h >> 2) + *p;
        p++;
    }

    return(h);
}

uint uiGetHash(const CFEString& _sString)
{
	return( _sString.uiGetHash() );
}

/*
static const unsigned char sTable[256] =
{
  0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
  0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
  0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
  0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
  0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
  0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
  0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
  0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
  0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
  0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
  0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
  0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
  0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
  0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
  0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
  0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46
};
#define PRIME_MULT 1717

// http://amsoftware.narod.ru/algo.html
// unsigned int maPrime2cHash (unsigned char *str, unsigned int len)
uint uiGetHash(const CFEString& _sString)
{
	uint len = _sString.uiLen();
	unsigned int hash = len, i;

	for (i = 0; i != len; i++, str++)
	{

	hash ^= sTable[( *str + i) & 255];
	hash = hash * PRIME_MULT;
	}

	return hash;
}
*/
//-----------------------------------------------------------------------------
}// CFEStringUtilsH
//-----------------------------------------------------------------------------
