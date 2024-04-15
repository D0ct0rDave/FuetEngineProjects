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
		
		/// Retrieves whether the dictionary is valid or not.
		FEBool bInitialized();

		// Dictionary resource release procedure.
		~CFEDictionary();
	
		/// Retrieves the value of the given variable, or it's default given value if the variable is not found. 
		CFEString sGetString(const CFEString& _sVariable,const CFEString& _sDefaultValue);
		
		/// Retrieves wether the given variable exists or not in the dictionary database.
		FEBool bExists(const CFEString& _sVariable);

	protected:

		// Database to store key/value pairs in the form of a config file.
		CFEConfigFile* m_poDict;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
