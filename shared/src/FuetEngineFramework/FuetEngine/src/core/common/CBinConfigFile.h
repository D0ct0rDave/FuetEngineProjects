// ----------------------------------------------------------------------------
/*! \class CBinConfigFile
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CBinConfigFileH
#define CBinConfigFileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
#ifndef MASTER_RELEASE_VERSION

	#ifdef WIN32
	#define CHECK_COLLISIONS
	#endif
	

	#ifdef CHECK_COLLISIONS
	#include "types/CFEHashTable.h"
	#endif

#endif
// ----------------------------------------------------------------------------
class CBinConfigFile
{
	public:
		CBinConfigFile();
		CBinConfigFile(const CFEString& _sFilename);
		~CBinConfigFile();

		/// Open a binary config file. Returns false if not possible.
		FEBool bOpen(const CFEString& _sFilename);

		/// Closes and deallocates resources after opening a binary config file.
		void Close();

		/// Retrieves the string identified by the given variable name. If it exists, copies
		/// the address of the string into the second argument, and returns true. If not, returns false.
		FEBool ReadString(const CFEString& _sVar,char** _pszValue);

		/// Retrieves the integer identified by the given variable name. If it exists, copies
		/// the value of the integer into the second argument, and returns true. If not, returns false.
		FEBool ReadInt(const CFEString& _sVar,int* _piValue);

		/// Retrieves the float identified by the given variable name. If it exists, copies
		/// the value of the float into the second argument, and returns true. If not, returns false.
		FEBool ReadFloat(const CFEString& _sVar,float* _pfValue);

		/// Retrieves the FEBoolean identified by the given variable name. If it exists, copies
		/// the value of the FEBoolean into the second argument, and returns true. If not, returns false.
		FEBool ReadBool(const CFEString& _sVar,FEBool* _pbValue);

		/// Retrieves whether a given variable is defined in the config file or not.
        FEBool bExists(const CFEString& _sVar);

	private:

		#ifdef CHECK_COLLISIONS
		class CColHashEntry
		{
			public:
				uint				m_uiHash;
				CFEArray<CFEString>	m_sVars;
		};

		CFEHashTable<CColHashEntry> m_oColCheckTable; 
		CFEString m_sFilename;
		#endif

        /// 
        typedef struct
        {
            uint m_uiHashID;
            uint m_uiValue;

        }TVariable;

        typedef struct{

            uint        m_uiNumElems;
            TVariable*  m_poVars;

        }THashedTable;
        
        /// Array of strings in the dictionary
        CFEArray<char*> m_oDictionary;

        unsigned int    m_uiTableWidth;
		THashedTable*   m_oTable;

		//
		FEPointer       m_pData;
		FEPointer       m_pDictData;

		// walks along the data and returns a pointer to the data contained withing the given variable
		// or returns NULL in case it's not found.
		void* pFindVar(const CFEString& _sVar);
		
		// The same as before but using the given data pointer as starting pos.
		void* pFindVar(const CFEString& _sVar,void* _pData);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

