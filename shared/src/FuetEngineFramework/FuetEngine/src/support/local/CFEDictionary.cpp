// ----------------------------------------------------------------------------
/*! \class CFEDictionary
 *  \brief Dictionary class to store key/value pairs related to local text data.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEDictionary.h"
// ----------------------------------------------------------------------------
/// Default constructor to build a dictionary around the given file data.
CFEDictionary::CFEDictionary(const CFEString& _sDictionary)
{
	CFEString sDict = _sDictionary + ".dic";
	m_poDict = new CFEConfigFile(sDict);
	if (!m_poDict->bInitialized())
	{
		delete m_poDict;
		m_poDict = NULL;
	}
}
// ----------------------------------------------------------------------------
// Dictionary resource release procedure.
CFEDictionary::~CFEDictionary()
{
	if (m_poDict != NULL)
		delete m_poDict;
}
// ----------------------------------------------------------------------------
/// Retrieves the value of the given variable, or it's default given value if the variable is not found.
CFEString CFEDictionary::sGetString(const CFEString& _sVariable,const CFEString& _sDefaultValue)
{
	if (m_poDict != NULL)
		return( m_poDict->sGetString(CFEString("Dictionary.")+_sVariable,_sDefaultValue) );
	else
		return(_sDefaultValue);
}
// ----------------------------------------------------------------------------
FEBool CFEDictionary::bExists(const CFEString& _sVariable)
{
	if (m_poDict != NULL)
		return( m_poDict->bExists( CFEString("Dictionary.")+_sVariable) );
	else
		return(false);
}
// ----------------------------------------------------------------------------
FEBool CFEDictionary::bInitialized()
{
	return(m_poDict != NULL);
}
// ----------------------------------------------------------------------------
