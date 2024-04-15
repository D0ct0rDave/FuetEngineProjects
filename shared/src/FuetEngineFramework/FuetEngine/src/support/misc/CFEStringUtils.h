// ----------------------------------------------------------------------------
/*! \class CFELogger
 *  \brief Logging racilities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#ifndef CFEStringUtilsH
#define CFEStringUtilsH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
//-----------------------------------------------------------------------------
namespace CFEStringUtils
{
	/// Retrieves the filename portion of a full qualified filename.
	CFEString sGetFilename(const CFEString& _sFilename);

	/// Retrieves the extension portion of a full qualified filename.
	CFEString sGetExtension(const CFEString& _sFilename);

	/// Retrieves the path portion of a full qualified filename.
	CFEString sGetPath(const CFEString& _sFilename);
	
	/// Retrieves an absolute unique string equivalent to the given path.
	CFEString sGetCanonicalPath(const CFEString& _sPath);

	/// Retrieves the full path without the extension portion of a full qualified filename.
	CFEString sRemoveExtension(const CFEString& _sFilename);	

	/// Changes all the backslashes to slashes in the path and returns the result.
	CFEString sNormalizePath(const CFEString& _sFilename);

	/// Retrieves a CRC value from the given string.
	uint uiGetCRC32(const char* _szBuffer, uint _uiSize);

	/// Retrieves a hashed value from the given string.
	uint uiGetHash(const CFEString& _sString);

	/// Retrieves a hashed value from the given string.
	uint uiGetHash(const char* _szString);
}
//-----------------------------------------------------------------------------
#endif // CFEStringUtilsH
//-----------------------------------------------------------------------------
