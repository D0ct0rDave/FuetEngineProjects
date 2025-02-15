// ----------------------------------------------------------------------------
/*! \class
 *  \brief Config files functionality through BinaryConfigFiles.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (CONFIG == USE_BINCONFIG)
// ----------------------------------------------------------------------------

#include <stdlib.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"
#include "CBinConfigFile.h"
// ----------------------------------------------------------------------------
/// Opens a config file.
// ----------------------------------------------------------------------------
FEHandler CFESystem::Config::hOpen(const CFEString& _sConfigFile)
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
void CFESystem::Config::Close(FEHandler _hConfig)
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
CFEString CFESystem::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
{
    if (_hConfig == NULL) return(_sDefaultValue);

   	char* szValue = NULL;
   	bool bRes = ((CBinConfigFile*)_hConfig)->ReadString(_sVar.szString(),&szValue);

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
FEReal CFESystem::Config::rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue)
{		
    if (_hConfig == NULL) return(_rDefaultValue);

	float fValue = 0.0;
	if ( ((CBinConfigFile*)_hConfig)->ReadFloat(_sVar.szString(),&fValue) == true)
		return( FEReal(fValue) );
	else
		return(_rDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves a integer variable value from the configuration file.
// ----------------------------------------------------------------------------
int CFESystem::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
{
    if (_hConfig == NULL) return(_iDefaultValue);

	int iValue = 0;
	if ( ((CBinConfigFile*)_hConfig)->ReadInt(_sVar.szString(),&iValue) == true)
		return (iValue );
	else
		return(_iDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves a bool variable value from the configuration file.
// ----------------------------------------------------------------------------
bool CFESystem::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,bool _bDefaultValue)
{
    if (_hConfig == NULL) return(_bDefaultValue);

	bool bValue = false;
	if ( ((CBinConfigFile*)_hConfig)->ReadBool(_sVar.szString(),&bValue) == true)
		return (bValue );
	else
		return(_bDefaultValue);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
