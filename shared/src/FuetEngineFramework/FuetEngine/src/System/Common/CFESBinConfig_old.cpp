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

#include "libconfig.h"
// ----------------------------------------------------------------------------
FIOHANDLER FEFIOOpen(const char* _szFilename,const char* _szMode)
{
	return( (FIOHANDLER)CFESystem::File::hOpen(CFEString(_szFilename),FOM_READ) );
}
// ----------------------------------------------------------------------------
void FEFIOClose(FIOHANDLER _pFD)
{
	CFESystem::File::Close(FEHandler(_pFD));
}
// ----------------------------------------------------------------------------
unsigned int FEFIORead(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD)
{
	return( CFESystem::File::uiRead((FEHandler)_pFD,_pData,_uiSize*_uiElems) / _uiSize );
}
// ----------------------------------------------------------------------------
/// Opens a config file.
// ----------------------------------------------------------------------------
FEHandler CFESystem::Config::hOpen(const CFEString& _sConfigFile)
{
	// Setup callback functions
	FIOSetOpenFunction(FEFIOOpen);
	FIOSetCloseFunction(FEFIOClose);
	FIOSetReadFunction(FEFIORead);

	// Act normally
    config_t* poCfg = (config_t*)CFESystem::Mem::pAlloc( sizeof(config_t) );
    config_init(poCfg);

    if (config_read_file(poCfg, _sConfigFile.szString() ) == 0)
    {
        config_destroy(poCfg);
        CFESystem::Mem::Free((FEPointer)poCfg);

        return(NULL);
    }

    return( FEHandler(poCfg) );
}
// ----------------------------------------------------------------------------
/// Closes the configuration file associated with the given handler.
// ----------------------------------------------------------------------------
void CFESystem::Config::Close(FEHandler _hConfig)
{
    if (_hConfig != NULL)
    {
    	// warning: in debug this takes looooooong time to process
        config_destroy((config_t*)_hConfig);
        CFESystem::Mem::Free((FEPointer)_hConfig);
    }
}
// ----------------------------------------------------------------------------
/// Retrieves a string variable value from the configuration file.
// ----------------------------------------------------------------------------
CFEString CFESystem::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
{
    if (_hConfig == NULL) return(_sDefaultValue);

    char* szValue = NULL;

    if (config_lookup_string((config_t*)_hConfig,_sVar.szString(),(const char**)&szValue) == CONFIG_TRUE)
	{
        #if defined(__WXMSW__)
        return ( CFEString((const char *)szValue,wxConvLibc) );
        #else
        return ( CFEString((const char *)szValue) );
        #endif        
	}
	else
        return( _sDefaultValue );
}
// ----------------------------------------------------------------------------
/// Retrieves a FEReal variable value from the configuration file.
// ----------------------------------------------------------------------------
FEReal CFESystem::Config::rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue)
{
    if (_hConfig == NULL) return(_rDefaultValue);

    double dValue = 0.0;
    if (config_lookup_float((config_t*)_hConfig,_sVar.szString(),&dValue) == CONFIG_TRUE)
		return ( (float)dValue );
	else
		return(_rDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves a integer variable value from the configuration file.
// ----------------------------------------------------------------------------
int CFESystem::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
{
    if (_hConfig == NULL) return(_iDefaultValue);

    long lValue = 0;

    if (config_lookup_int((config_t*)_hConfig,_sVar.szString(),&lValue) == CONFIG_TRUE)
		return ( lValue );
	else
		return(_iDefaultValue);    
}
// ----------------------------------------------------------------------------
/// Retrieves a bool variable value from the configuration file.
// ----------------------------------------------------------------------------
bool CFESystem::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,bool _bDefaultValue)
{
    if (_hConfig == NULL) return(_bDefaultValue);

    int iValue = 0;

    if (config_lookup_bool((config_t*)_hConfig,_sVar.szString(),&iValue) == CONFIG_TRUE)
		return ( iValue );
	else
		return(_bDefaultValue);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
