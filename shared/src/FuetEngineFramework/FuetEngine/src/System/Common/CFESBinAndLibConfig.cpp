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
#if (CONFIG == USE_BIN_AND_LIBCONFIG)
// ----------------------------------------------------------------------------

#include <stdlib.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"

#include "libconfig.h"
#include "CBinConfigFile.h"

class CCfgHandler
{
	public:	
		CCfgHandler() : m_bBinary(false), m_pConfig(NULL) {};

		bool		m_bBinary;
		FEPointer	m_pConfig;
};
// ----------------------------------------------------------------------------
/*
#ifdef WIN32
void AddVar(const CFEString& _sVar)
{
	// CFEString sCmd = CFEString("echo ")+_sVar+CFEString(" >>c:\\bnbvars.txt");
	// system(sCmd.szString());
	FILE* fd = fopen("c:\\bnbvars.txt","a+");
	if (fd != NULL)
	{
		fprintf(fd,"%s\n",_sVar.szString());
		fclose(fd);
	}
}
#else
void AddVar(const CFEString& _sVar)
{
}
#endif
*/
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
		config_t* poCfg = (config_t*)CFESystem::Mem::pAlloc( sizeof(config_t) );
		config_init(poCfg);

		if (config_read_file(poCfg, _sConfigFile.szString() ) == 0)
		{
			CFESystem::Log::Print("Config file error: %s at line %d\n",(char*)config_error_text(poCfg),config_error_line(poCfg));

			config_destroy(poCfg);
			CFESystem::Mem::Free((FEPointer)poCfg);
			return(NULL);
		}

		CCfgHandler* poHnd = new CCfgHandler;
		poHnd->m_bBinary = false;
		poHnd->m_pConfig = (FEPointer)poCfg;	

		return( FEHandler(poHnd) );
	}
}
// ----------------------------------------------------------------------------
/// Closes the configuration file associated with the given handler.
// ----------------------------------------------------------------------------
void CFESystem::Config::Close(FEHandler _hConfig)
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
			CFESystem::Mem::Free((FEPointer)poHnd->m_pConfig);
		}

		// delete the config handler
		delete poHnd;
    }
}
// ----------------------------------------------------------------------------
/// Retrieves a string variable value from the configuration file.
// ----------------------------------------------------------------------------
CFEString CFESystem::Config::sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue)
{
    // AddVar(_sVar);
    if (_hConfig == NULL) return(_sDefaultValue);

   	CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
   	char* szValue = NULL;
   	bool bRes = false;
	
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
FEReal CFESystem::Config::rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue)
{
    // AddVar(_sVar);
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
int CFESystem::Config::iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue)
{
    // AddVar(_sVar);
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
/// Retrieves a bool variable value from the configuration file.
// ----------------------------------------------------------------------------
bool CFESystem::Config::bGetBool(FEHandler _hConfig,const CFEString& _sVar,bool _bDefaultValue)
{
	// AddVar(_sVar);
    if (_hConfig == NULL) return(_bDefaultValue);

    CCfgHandler* poHnd = (CCfgHandler*)_hConfig;
	if (poHnd->m_bBinary)
	{
		bool bValue = false;

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
#endif
// ----------------------------------------------------------------------------
