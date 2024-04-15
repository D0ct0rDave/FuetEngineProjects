// ----------------------------------------------------------------------------
/*! \class
 *  \brief Config files functionality through BinaryConfigFiles.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (CONFIG == USE_BINCONFIG)
// ----------------------------------------------------------------------------

#include <stdlib.h>

#include "../CFECore.h"
#include "FEBasicTypes.h"
#include "CBinConfigFile.h"
// ----------------------------------------------------------------------------
/// Opens a config file.
// ----------------------------------------------------------------------------
FEHandler CFECore::Config::hOpen(const CFEString& _sConfigFile)
{
	CBinConfigFile* poBF = new CBinConfigFile();

	if (poBF->bOpen(_sConfigFile.szString()) == false)
	{
		delete poBF;
		poBF = NULL;
	}

	return( FEHandler(poBF) );
}
// ----------------------------------------------------------------------------
/// Closes the configuration file associated with the given handler.
// ----------------------------------------------------------------------------
void CFECore::Config::Close(FEHandler _hConfig)
{
	if (_hConfig != NULL)
    {
		// delete the config handler
		delete ( (CBinConfigFile*)_hConfig );
    }
}
// ----------------------------------------------------------------------------
/// Retrieves a string variable value from the configuration file.
// ----------------------------------------------------------------------------
CFEString CFECore::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
{
    if (_hConfig == NULL) return(_sDefaultValue);

   	char* szValue = NULL;
   	FEBool bRes = ((CBinConfigFile*)_hConfig)->ReadString(_sVar,&szValue);

	if (bRes)
	{
		#if defined(__WXMSW__)
		return ( CFEString((const char *)szValue,wxConvLibc) );
		#else
		return ( CFEString((const char *)szValue) );
		#endif
	}
	else
	{
		return( _sDefaultValue );
	}
}
// ----------------------------------------------------------------------------
/// Retrieves a FEReal variable value from the configuration file.
// ----------------------------------------------------------------------------
FEReal CFECore::Config::rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue)
{		
    if (_hConfig == NULL) return(_rDefaultValue);

	float fValue = 0.0;
	if ( ((CBinConfigFile*)_hConfig)->ReadFloat(_sVar,&fValue) == true)
		return( FEReal(fValue) );
	else
		return(_rDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves a integer variable value from the configuration file.
// ----------------------------------------------------------------------------
int CFECore::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
{
    if (_hConfig == NULL) return(_iDefaultValue);

	int iValue = 0;
	if ( ((CBinConfigFile*)_hConfig)->ReadInt(_sVar,&iValue) == true)
		return (iValue );
	else
		return(_iDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves a FEBool variable value from the configuration file.
// ----------------------------------------------------------------------------
FEBool CFECore::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,FEBool _bDefaultValue)
{
    if (_hConfig == NULL) return(_bDefaultValue);

	FEBool bValue = false;
	if ( ((CBinConfigFile*)_hConfig)->ReadBool(_sVar,&bValue) == true)
		return (bValue );
	else
		return(_bDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves whether a given variable is defined in the config file or not.
// ----------------------------------------------------------------------------
FEBool CFECore::Config::bExists(FEHandler _hConfig,const CFEString& _sVar)
{
    if (_hConfig == NULL) return(false);
	return( ((CBinConfigFile*)_hConfig)->bExists(_sVar) );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
