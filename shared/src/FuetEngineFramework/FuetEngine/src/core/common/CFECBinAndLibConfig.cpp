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
#if (CONFIG == USE_BIN_AND_LIBCONFIG)
// ----------------------------------------------------------------------------
#pragma warning( disable : 4800 )	// warning C4800: 'int' : forcing value to FEBool 'true' or 'false' (performance warning)

#include <stdlib.h>

#include "../CFECore.h"
#include "FEBasicTypes.h"

#include "libconfig.h"
#include "CBinConfigFile.h"

class CCfgHandler
{
	public:	
		CCfgHandler() : m_bBinary(false), m_pConfig(NULL) {};

		FEBool		m_bBinary;
		FEPointer	m_pConfig;
};
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
	CBinConfigFile* poBF = new CBinConfigFile();

	if (poBF->bOpen(_sConfigFile.szString()) == true)
	{
		CCfgHandler* poHnd = new CCfgHandler;
		poHnd->m_bBinary = true;
		poHnd->m_pConfig = (FEPointer)poBF;
		
		return( FEHandler(poHnd) );
	}
	else
	{
		delete poBF;

		// Setup callback functions
		FIOSetOpenFunction(FEFIOOpen);
		FIOSetCloseFunction(FEFIOClose);
		FIOSetReadFunction(FEFIORead);

		// Act normally
		config_t* poCfg = (config_t*)CFECore::Mem::pAlloc( sizeof(config_t) );
		config_init(poCfg);

		if (config_read_file(poCfg, _sConfigFile.szString() ) == 0)
		{
			/*
			if (config_error_type(poCfg) != CONFIG_ERR_FILE_IO)
			{
				CFECore::Log::Print("Config file error: %s at line %d\n",(char*)config_error_text(poCfg),config_error_line(poCfg));
			}
			*/

			CFEString sErr = CFEString(config_error_text(poCfg));
			if (sErr.iPos("file I/O error")<0)
				CFECore::Log::Print("Config file error: %s at line %d\n",(char*)config_error_text(poCfg),config_error_line(poCfg));
			
			config_destroy(poCfg);
			CFECore::Mem::Free((FEPointer)poCfg);
			return(NULL);
		}

		/// perform silently conversion between types (int <-> float)
		config_set_auto_convert(poCfg,true);

		CCfgHandler* poHnd = new CCfgHandler;
		poHnd->m_bBinary = false;
		poHnd->m_pConfig = (FEPointer)poCfg;	

		return( FEHandler(poHnd) );
	}
}
// ----------------------------------------------------------------------------
/// Closes the configuration file associated with the given handler.
// ----------------------------------------------------------------------------
void CFECore::Config::Close(FEHandler _hConfig)
{
    if (_hConfig != NULL)
    {
    	CCfgHandler* poHnd = (CCfgHandler*)_hConfig;

		if (poHnd->m_bBinary)
		{
			delete ( (CBinConfigFile*)poHnd->m_pConfig );
		}
		else
		{
			config_destroy((config_t*)poHnd->m_pConfig);
			CFECore::Mem::Free((FEPointer)poHnd->m_pConfig);
		}

		// delete the config handler
		delete poHnd;
    }
}
// ----------------------------------------------------------------------------
/// Retrieves a string variable value from the configuration file.
// ----------------------------------------------------------------------------
CFEString CFECore::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
{
    if (_hConfig == NULL) return(_sDefaultValue);

   	CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
   	char* szValue = NULL;
   	FEBool bRes = false;
	
	if (poHnd->m_bBinary)
		bRes = ((CBinConfigFile*)poHnd->m_pConfig)->ReadString(_sVar.szString(),&szValue);
	else
		bRes = config_lookup_string((config_t*)poHnd->m_pConfig,_sVar.szString(),(const char**)&szValue);

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
    
    CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
	if (poHnd->m_bBinary)
	{
		float fValue = 0.0;

		if ( ((CBinConfigFile*)poHnd->m_pConfig)->ReadFloat(_sVar.szString(),&fValue) == true)
			return( FEReal(fValue) );
		else
			return(_rDefaultValue);
	}
	else
	{	
		double dValue = 0.0;

		if (config_lookup_float((config_t*)poHnd->m_pConfig,_sVar.szString(),&dValue) == CONFIG_TRUE)
			return ( (float)dValue );
		else
			return(_rDefaultValue);
	}
}
// ----------------------------------------------------------------------------
/// Retrieves a integer variable value from the configuration file.
// ----------------------------------------------------------------------------
int CFECore::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
{
    if (_hConfig == NULL) return(_iDefaultValue);

    CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
	if (poHnd->m_bBinary)
	{
		int iValue = 0;

		if ( ((CBinConfigFile*)poHnd->m_pConfig)->ReadInt(_sVar.szString(),&iValue) == true)
			return (iValue );
		else
			return(_iDefaultValue);
	}
	else
	{
		long lValue = 0;

		if (config_lookup_int((config_t*)poHnd->m_pConfig,_sVar.szString(),&lValue) == CONFIG_TRUE)
			return ( lValue );
		else
			return(_iDefaultValue);    
	}
}
// ----------------------------------------------------------------------------
/// Retrieves a FEBool variable value from the configuration file.
// ----------------------------------------------------------------------------
FEBool CFECore::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,FEBool _bDefaultValue)
{
    if (_hConfig == NULL) return(_bDefaultValue);

    CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
	if (poHnd->m_bBinary)
	{
		FEBool bValue = false;

		if ( ((CBinConfigFile*)poHnd->m_pConfig)->ReadBool(_sVar.szString(),&bValue) == true)
			return (bValue );
		else
			return(_bDefaultValue);
	}
	else
	{	
		int iValue = 0;

		if (config_lookup_bool((config_t*)poHnd->m_pConfig,_sVar.szString(),&iValue) == CONFIG_TRUE)
			return ( iValue );
		else
			return(_bDefaultValue);
	}
}
// ----------------------------------------------------------------------------
/// Retrieves whether a given variable is defined in the config file or not.
// ----------------------------------------------------------------------------
FEBool CFECore::Config::bExists(FEHandler _hConfig,const CFEString& _sVar)
{
    if (_hConfig == NULL) return(false);

    CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
	if (poHnd->m_bBinary)
	{
		return( ((CBinConfigFile*)poHnd->m_pConfig)->bExists(_sVar.szString()) );
	}
	else
	{
		return( config_lookup((config_t*)poHnd->m_pConfig,_sVar.szString()) != NULL );
	}
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
