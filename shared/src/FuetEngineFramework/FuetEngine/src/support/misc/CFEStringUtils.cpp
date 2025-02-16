// ----------------------------------------------------------------------------
/*! \class CFEStringUtils
 *  \brief String Utilities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#include "CFEStringUtils.h"
#include "crc32.h"

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
//-----------------------------------------------------------------------------
/// Retrieves a CRC value from the given string.
uint CFEStringUtils::uiGetCRC32(const char* _szBuffer, uint _uiSize)
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
// uses SAX hashing
uint CFEStringUtils::uiGetHash(const CFEString& _sString)
{
    unsigned char* p = (unsigned char*)_sString.szString();
    uint h = 0;

    while (*p != 0)
    {
        h ^= (h << 5) + (h >> 2) + *p;
        p++;
    }

    return(h);
}
//-----------------------------------------------------------------------------
};// CFEStringUtilsH
//-----------------------------------------------------------------------------
