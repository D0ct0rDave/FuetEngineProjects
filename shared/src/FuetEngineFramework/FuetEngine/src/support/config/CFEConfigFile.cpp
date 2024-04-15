// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------

#include "CFEConfigFile.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
CFEConfigFile::CFEConfigFile() : m_hConfigObj(NULL)
{
}
// ----------------------------------------------------------------------------
CFEConfigFile::CFEConfigFile(const CFEString& _sConfigFile) : m_hConfigObj(NULL)
{    
    bLoad(_sConfigFile);
}
// ----------------------------------------------------------------------------
CFEConfigFile::~CFEConfigFile()
{
	if (m_hConfigObj!=NULL)
		CFECore::Config::Close(m_hConfigObj);
}
// ----------------------------------------------------------------------------
FEBool CFEConfigFile::bLoad(const CFEString& _sConfigFile)
{
    m_hConfigObj = CFECore::Config::hOpen(_sConfigFile);
    return( bInitialized() );
}
// ----------------------------------------------------------------------------
FEBool CFEConfigFile::bInitialized() const
{
    return(m_hConfigObj != NULL);
}
// ----------------------------------------------------------------------------
CFEString CFEConfigFile::sGetString(const CFEString& _sVar,const CFEString& _sDefaultValue) const
{
	if (m_hConfigObj != NULL)
		return(CFECore::Config::sGetString(m_hConfigObj,_sVar,_sDefaultValue));
	else
		return(_sDefaultValue);
}
// ----------------------------------------------------------------------------
FEReal CFEConfigFile::rGetReal(const CFEString& _sVar,FEReal _rDefaultValue) const
{
	if (m_hConfigObj != NULL)
		return(CFECore::Config::rGetReal(m_hConfigObj,_sVar,_rDefaultValue));
	else
		return(_rDefaultValue);
}
// ----------------------------------------------------------------------------
int CFEConfigFile::iGetInteger(const CFEString& _sVar,int _iDefaultValue) const
{
	if (m_hConfigObj != NULL)
		return(CFECore::Config::iGetInteger(m_hConfigObj,_sVar,_iDefaultValue));
	else
		return(_iDefaultValue);
}
// ----------------------------------------------------------------------------
FEBool CFEConfigFile::bGetBool(const CFEString& _sVar,FEBool _bDefaultValue) const
{
	if (m_hConfigObj != NULL)
		return(CFECore::Config::bGetBool(m_hConfigObj,_sVar,_bDefaultValue));
	else
		return(_bDefaultValue);
}
// ----------------------------------------------------------------------------
/// Retrieves whether the given variable exists or not.
FEBool CFEConfigFile::bExists(const CFEString& _sVar) const
{
	if (m_hConfigObj != NULL)
		return( CFECore::Config::bExists(m_hConfigObj,_sVar) );
	else
		return(false);
}
// ----------------------------------------------------------------------------
