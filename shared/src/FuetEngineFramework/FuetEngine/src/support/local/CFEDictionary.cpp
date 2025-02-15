// ----------------------------------------------------------------------------
/*! \class CFEDictionary
 *  \brief Dictionary class to store key/value pairs related to local text data.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEDictionary.h"
// ----------------------------------------------------------------------------
/// Default constructor to build a dictionary around the given file data.
CFEDictionary::CFEDictionary(const CFEString& _sDictionary)
{
	m_poDict = new CFEConfigFile(_sDictionary);
}
// ----------------------------------------------------------------------------
// Dictionary resource release procedure.
CFEDictionary::~CFEDictionary()
{
	delete m_poDict;
}
// ----------------------------------------------------------------------------
/// Retrieves the value of the given variable, or it's default given value if the variable is not found.
const CFEString& CFEDictionary::sGetString(const CFEString& _sVariable,const CFEString& _sDefaultValue)
{
	static CFEString sStr;
	sStr = m_poDict->sGetString(CFEString("Dictionary.")+_sVariable,_sDefaultValue);
	return( sStr );
}
// ----------------------------------------------------------------------------
