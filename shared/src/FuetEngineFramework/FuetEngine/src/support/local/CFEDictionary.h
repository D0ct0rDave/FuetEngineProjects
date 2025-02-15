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
#ifndef CFEDictionaryH
#define CFEDictionaryH
// ----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "support/config/CFEConfigFile.h"
// ----------------------------------------------------------------------------
class CFEDictionary
{
	public:
		
		/// Default constructor to build a dictionary around the given file data.
		CFEDictionary(const CFEString& _sDictionary);
		
		// Dictionary resource release procedure.
		~CFEDictionary();
	
		/// Retrieves the value of the given variable, or it's default given value if the variable is not found.
		const CFEString& sGetString(const CFEString& _sVariable,const CFEString& _sDefaultValue);

	protected:

		// Database to store key/value pairs in the form of a config file.
		CFEConfigFile* m_poDict;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
