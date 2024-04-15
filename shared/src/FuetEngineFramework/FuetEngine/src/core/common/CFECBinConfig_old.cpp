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

#include "../CFECore.h"
#include "FEBasicTypes.h"

#include "libconfig.h"
// ----------------------------------------------------------------------------
FIOHANDLER FEFIOOpen(const char* _szFilename,const char* _szMode)
{
	return( (FIOHANDLER)CFECore::File::hOpen(CFEString(_szFilename),FOM_READ) );
}
// ----------------------------------------------------------------------------
void FEFIOClose(FIOHANDLER _pFD)
{
	CFECore::File::Close(FEHandler(_pFD));
}
// ----------------------------------------------------------------------------
unsigned int FEFIORead(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD)
{
	return( CFECore::File::uiRead((FEHandler)_pFD,_pData,_uiSize*_uiElems) / _uiSize );
}
// ----------------------------------------------------------------------------
/// Opens a config file.
// ----------------------------------------------------------------------------
FEHandler CFECore::Config::hOpen(const CFEString& _sConfigFile)
{
	// Setup callback functions
	FIOSetOpenFunction(FEFIOOpen);
	FIOSetCloseFunction(FEFIOClose);
	FIOSetReadFunction(FEFIORead);

	// Act normally
    config_t* poCfg = (config_t*)CFECore::Mem::pAlloc( sizeof(config_t) );
    config_init(poCfg);

    if (config_read_file(poCfg, _sConfigFile.szString() ) == 0)
    {
        config_destroy(poCfg);
        CFECore::Mem::Free((FEPointer)poCfg);

        return(NULL);
    }

    return( FEHandler(poCfg) );
}
// ----------------------------------------------------------------------------
/// Closes the configuration file associated with the given handler.
// ----------------------------------------------------------------------------
void CFECore::Config::Close(FEHandler _hConfig)
{
    if (_hConfig != NULL)
    {
    	// warning: in debug this takes looooooong time to process
    	#if 1 // (!defined(DS) || !defined(_DEBUG))
        config_destroy((config_t*)_hConfig);
        #endif

        CFECore::Mem::Free((FEPointer)_hConfig);
    }
}
// ----------------------------------------------------------------------------
/// Retrieves a string variable value from the configuration file.
// ----------------------------------------------------------------------------
CFEString CFECore::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
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
FEReal CFECore::Config::rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue)
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
int CFECore::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
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
bool CFECore::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,bool _bDefaultValue)
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
