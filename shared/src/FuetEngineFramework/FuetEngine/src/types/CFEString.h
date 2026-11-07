// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 *  \par This string implementation comes directle from the GammaE Engine.
 */
// ----------------------------------------------------------------------------
#ifndef CFEStringH
#define CFEStringH
// ----------------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFEStringData
{
	public:

		CFEStringData() : m_szData(NULL), m_uiLen(0), m_uiHash(0), m_bDirty(true)
	    {
	    }

		char*			m_szData;
		uint			m_uiLen;
		uint			m_uiHash;
		FEBool			m_bDirty;
};
// ----------------------------------------------------------------------------
class CFEString
{
    public:

        /// Creates an empty string
        CFEString()
        {
			 m_poData = new CFEStringData;
        }

        /// Frees the resources possibly allocated by the object
        ~CFEString();

        /// Creates a string with the same contents as the char* string passed as parameter.
        CFEString(const char* _szData)
        {
			m_poData = new CFEStringData;
            Assign(_szData);
        }

        /// Copy constructor: Creates a new string with the same contents as the string supplied as parameter
        CFEString(const CFEString& _oStr)
        {
			m_poData = new CFEStringData;
            Assign( _oStr.szString() );
        }

        /// Creates a string containing the only character passed as parameter
        CFEString(char _cChar)
        {
            m_poData = new CFEStringData;
            char szStr[4] = {_cChar,0};
            Assign(szStr);
        }

        /// Creates a string containing the ascii representation of the integer value
        CFEString(int _iValue)
        {
			m_poData = new CFEStringData;

			// max uint = 4294967295 = 10 chars put 12 to align to 4 byte stack, just in case ...
            char szStr[12];
			#ifdef _MSC_VER
            sprintf_s(szStr,12,"%d",_iValue);
			#else
			sprintf(szStr,"%d",(int)_iValue);
			#endif

            Assign(szStr);
        }
/*

        /// Creates a string containing the ascii representation of the uinteger value
        CFEString(uint _uiValue)
        {

        }
*/

        /// Links the string contents to the given char* string.
        // void Link(char* _szStr);

        /// Copy the string contents to the current
        void Assign(const char* _szStr);

        /// Copy the formatted string to the string object
        void Format(const char* fmt,...);

        /// Returns a formated string.
        static CFEString sFormat(const char* fmt,...);
        
		/// Returns the length of the string
		uint uiLen() const;

        /// Returns the position of a substring inside the string, -1 if not found
        int iPos(const CFEString& _oSubStr) const;

        /// Returns the position of a c substring inside the string, -1 if not found
        int iPos(const char* _szStr) const;

        /// Returns the position of a c character inside the string, -1 if not found
        int iPos(char _cChar)
        {
            char szStr[4] = {_cChar,0};
            return ( iPos(szStr) );
        }

        /// Returns the position of a substring inside the string starting from the end in backwards direction, -1 if not found
        int iRPos(const CFEString& _oSubStr) const;

        /// Returns the position of a c substring inside the string starting from the end in backwards direction, -1 if not found
        int iRPos(const char* _szStr) const;

        /// Returns the position of a c character inside the string starting from the end in backwards direction, -1 if not found
        int iRPos(char _cChar)
        {
            char szStr[4] = {_cChar,0};
            return ( iRPos(szStr) );
        }

        /// Returns the c representation of the string
        const char* szString() const
        {
            return(m_poData->m_szData);
        }

        /// Returns a Substring of length _uiLen contained within the current string at pos _uiPos
        CFEString SubString(uint _uiPos,uint _uiLen) const;

        /// Concatenates the contents of the given string
        void Concat(const CFEString& _oStr);

		/// Concatenates the contents of the given string
		void Concat(const char* _szStr);

		// Performs fast comparaison.
		/// Compares equality to a given string
		FEBool operator == (const char* _right) const;
		FEBool operator == (const CFEString& _right) const;

		/// Compares inequality to a given string
		FEBool operator != (const CFEString& _right) const;
		FEBool operator != (const char* _right) const;
		
		
		/// Compares equality to a given string ignoring case
		FEBool operator |=(const char* _szStr) const;
		FEBool operator |=(const CFEString& _oStr) const;

        /// Returns the character on the string into the index position
        char operator[](const uint index) const
        {
            if ( m_poData->m_szData == NULL ) return(0);
            #ifdef _DEBUG
            uint uiLen = this->uiLen();
            if ( index >= uiLen )
                return(0);
            #endif

            return( m_poData->m_szData[index] );
        }

        /// Copies the contents of a given string
        CFEString& operator = (const CFEString& _oRight)
        {
            if ( this == &_oRight )   // Same object?
                return(*this);       // Yes, so skip assignment, and just return *this.

            Assign( (char*)_oRight.szString() );
            return(*this);
        }

		/// Copies the contents of a given string
		CFEString& operator = (const char* _szRight)
		{
			if (m_poData->m_szData == _szRight)	// Same object?
				return(*this);					// Yes, so skip assignment, and just return *this.

			Assign(_szRight);
			return(*this);
		}

        /// Concatenates the contents of the given string
        CFEString& operator += (const CFEString& _oRight)
        {
            Concat(_oRight);
            return(*this);
        }

		/// Concatenates the contents of the given string
		CFEString& operator += (const char* _szRight)
		{
			Concat(_szRight);
			return(*this);
		}

        /// Concatenates the contents of the given string
        CFEString operator + (const CFEString& _oStr) const;

		/// Concatenates the contents of the given string
		CFEString operator + (const char* _szStr) const;

        /// Concatenates the char
        CFEString operator + (const char _cChar) const;

        /// Returns a copy of the string in upper case
        CFEString UpperCase() const;

        /// Converts to upper case
        void ToUpperCase();

        /// Returns a copy of the string in lower case
        CFEString LowerCase() const;

        /// Converts to lower case
        void ToLowerCase();
        
        /// Returns the NULL String.
        static const CFEString& sNULL();

    private:

        /// Free the buffer that holds the data
        void Free();
        void Alloc(uint _uiSize);

    public:
		
		/// Retrieves the hash of the object.
		uint uiGetHash() const
		{
			if (bIsDirty())
				ComputeHash();

			return(m_poData->m_uiHash);
		}

	protected:

		/// Retrieves whether the contents needs to be recomputed or not
		FEBool bIsDirty() const
		{
			return(m_poData->m_bDirty);
		};

		/// Sets the contents to be dirty and need to be updated.
		void SetDirty()
		{
			m_poData->m_bDirty = true;
		}
		
		/// Recomputes the hasj
		void ComputeHash() const;

	protected:

		CFEStringData*	m_poData;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
