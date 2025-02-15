// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------

#include "CFEConfigFile.h"
#include "System/CFESystem.h"
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
    CFESystem::Config::Close(m_hConfigObj);
}
// ----------------------------------------------------------------------------
bool CFEConfigFile::bLoad(const CFEString& _sConfigFile)
{
    m_hConfigObj = CFESystem::Config::hOpen(_sConfigFile);
    return( bInitialized() );
}
// ----------------------------------------------------------------------------
bool CFEConfigFile::bInitialized() const
{
    return(m_hConfigObj != NULL);
}
// ----------------------------------------------------------------------------
CFEString CFEConfigFile::sGetString(const CFEString& _sVar,const CFEString& _sDefaultValue) const
{
    return(CFESystem::Config::sGetString(m_hConfigObj,_sVar,_sDefaultValue));
}
// ----------------------------------------------------------------------------
FEReal CFEConfigFile::rGetReal(const CFEString& _sVar,FEReal _rDefaultValue) const
{
    return(CFESystem::Config::rGetReal(m_hConfigObj,_sVar,_rDefaultValue));
}
// ----------------------------------------------------------------------------
int CFEConfigFile::iGetInteger(const CFEString& _sVar,int _iDefaultValue) const
{
    return(CFESystem::Config::iGetInteger(m_hConfigObj,_sVar,_iDefaultValue));
}
// ----------------------------------------------------------------------------
bool CFEConfigFile::bGetBool(const CFEString& _sVar,bool _bDefaultValue) const
{
    return(CFESystem::Config::bGetBool(m_hConfigObj,_sVar,_bDefaultValue));
}
// ----------------------------------------------------------------------------
