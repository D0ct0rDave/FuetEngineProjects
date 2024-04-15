// ----------------------------------------------------------------------------
/*! \class CFEFontANSI
 *  \brief Font Class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEFontANSIH
#define CFEFontANSIH
// ----------------------------------------------------------------------------
#include "CFEFont.h"
// ----------------------------------------------------------------------------
class CFEFontANSI : public CFEFont
{
    public:

		using CFEFont::rCharKerning;

		/// Default constructor.
		CFEFontANSI();

		/// Destructor of the class.
		virtual ~CFEFontANSI();

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
		CCharInfo m_oCharTable[MAX_CHAR];
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
