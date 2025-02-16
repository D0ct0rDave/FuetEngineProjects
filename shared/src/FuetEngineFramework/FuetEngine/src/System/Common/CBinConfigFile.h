// ----------------------------------------------------------------------------
/*! \class CBinConfigFile
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CBinConfigFileH
#define CBinConfigFileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
class CBinConfigFile
{
	public:
		CBinConfigFile();
		CBinConfigFile(const char* _szFilename);
		~CBinConfigFile();

		/// Open a binary config file. Returns false if not possible.
		bool bOpen(const char* _szFilename);

		/// Closes and deallocates resources after opening a binary config file.
		void Close();

		/// Retrieves the string identified by the given variable name. If it exists, copies
		/// the address of the string into the second argument, and returns true. If not, returns false.
		bool ReadString(const char* _szVar,char** _pszValue);

		/// Retrieves the integer identified by the given variable name. If it exists, copies
		/// the value of the integer into the second argument, and returns true. If not, returns false.
		bool ReadInt(const char* _szVar,int* _piValue);

		/// Retrieves the float identified by the given variable name. If it exists, copies
		/// the value of the float into the second argument, and returns true. If not, returns false.
		bool ReadFloat(const char* _szVar,float* _pfValue);

		/// Retrieves the boolean identified by the given variable name. If it exists, copies
		/// the value of the boolean into the second argument, and returns true. If not, returns false.
		bool ReadBool(const char* _szVar,bool* _pbValue);

	private:
        
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
		void* pFindVar(const char* _szVar);
		
		// The same as before but using the given data pointer as starting pos.
		void* pFindVar(const char* _szVar,void* _pData);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

