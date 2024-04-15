// ----------------------------------------------------------------------------
/*! \class CFEFontUTF8
 *  \brief Font Class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEFontUTF8H
#define CFEFontUTF8H
// ----------------------------------------------------------------------------
#include "CFEFont.h"
#include "types/CFEHashTable.h"
// ----------------------------------------------------------------------------
class CFEFontUTF8 : public CFEFont
{
    public:
		
		/// Default constructor.
		CFEFontUTF8();

		/// Destructor of the class.
		virtual ~CFEFontUTF8();

        /// Retrieves the length of the string in characters. 
        /// Depends on the font support for UTF8 characters and the string contents.
        virtual uint uiStringLen(char* _szStart,char* _szEnd) const;

        /// Retrieves the current char in the given string.
        virtual uint uiGetCurrentChar(char *_szStr) const;

        /// Retrieves the pointer to the next character in the string.
        virtual char* szGotoNextChar(char *_szStr) const;

        /// Adds character information belonging to the given character.
        virtual void AddChar(uint _uiChar,const CCharInfo& _oCI);

		/// Retrieves the character information data belonging to the given character.
		virtual const CCharInfo* poGetCharInfo(uint _uiChar) const;

    protected:

		/// The character table.
		CFEHashTable<CCharInfo>	m_oCharTable;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
