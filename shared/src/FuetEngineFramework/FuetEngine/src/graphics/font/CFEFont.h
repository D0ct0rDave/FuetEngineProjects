// ----------------------------------------------------------------------------
/*! \class CFEFont
 *  \brief Font Class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEFontH
#define CFEFontH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFERect.h"
#include "Types/CFEString.h"
// ----------------------------------------------------------------------------
/// Maximum number of chars stored in a font file.
const uint MAX_CHAR = 256;
// ----------------------------------------------------------------------------
// #pragma pack(push,"",1)
// #pragma options align= 1

class CCharInfo
{

	public:
		CCharInfo() : 
			m_oUV( CFERect(CFEVect2::ZERO(),CFEVect2::ZERO()) ),
			m_usCW(0),
			m_usCH(0),
			m_sYOfs(0),
			m_cLC(0),
			m_cRC(0)
		{
			
		};

		/// UVs of this char.
		CFERect          m_oUV;

		/// Width in pixels of the char.
		unsigned short   m_usCW;

		/// Height in pixels of the char.
		unsigned short   m_usCH;

		/// Baseline offset of the char.
		short			m_sYOfs;

		/// Left class of the character
		unsigned char m_cLC;

		/// Right class of the character
		unsigned char m_cRC;

};

// #pragma options align= reset
// #pragma pack(pop,"")
//-----------------------------------------------------------------------------
class CFEKernTable
{
	public:

		CFEKernTable(uint _uiNumLeftClasses,uint _uiNumRightClasses)
		{
			m_uiNumLeftClasses  = _uiNumLeftClasses;
			m_uiNumRightClasses = _uiNumRightClasses;

			if (_uiNumLeftClasses*_uiNumRightClasses == 0)
				m_sKernTable = NULL;
			else
				m_sKernTable = new short[m_uiNumLeftClasses*m_uiNumRightClasses];
		}
		
		~CFEKernTable()
		{
			delete []m_sKernTable;
		}

		void SetKern(uint _uiLeftClass,uint _uiRightClass,int _iOfs)
		{
			m_sKernTable[_uiRightClass*m_uiNumLeftClasses + _uiLeftClass] = (short)_iOfs;
		}

		int iGetKern(uint _uiLeftClass,uint _uiRightClass)
		{
			return( m_sKernTable[_uiRightClass*m_uiNumLeftClasses + _uiLeftClass]);
		}

	public:

		short*	m_sKernTable;
		uint	m_uiNumLeftClasses;
		uint	m_uiNumRightClasses;
};
// ----------------------------------------------------------------------------
class CFEFont
{
    public:
		
		/// Default constructor.
		CFEFont();

		/// Destructor of the class.
		virtual ~CFEFont();

        /// Initializes the font object by using a given material, a buffer of char rect coords, and an optional kerning table.
        void Init(FEHandler _hMaterial,CFEKernTable* _poKT = NULL) { SetMaterial(_hMaterial); m_poKT = _poKT; };

        /// Retrieves the length of an specific char in pixels
        FEReal rCharWidth(uint _uiChar) const
        {
			return(poGetCharInfo(_uiChar)->m_usCW);
        };

        /// Retrieves the height of an specific char in pixels.
        FEReal rCharHeight(uint _uiChar) const
        {
			return(poGetCharInfo(_uiChar)->m_usCH);
        }

        /// Retrieves the Y offset to add to the start of the character.
        FEReal rCharLeading(uint _uiChar) const
        {
			return(poGetCharInfo(_uiChar)->m_sYOfs);
        }

		/// Retrieves the texture coordinates of a given character.
		const CFERect& oCharUVs(uint _uiChar) const
		{
			return(poGetCharInfo(_uiChar)->m_oUV);
		};

        /// Retrieves the length of the string in pixels.
        FEReal rStringLen(const CFEString& _sString,uint _uiFrom = 0,int _iLen = -1) const;

        /// Retrieves the length of the string in characters. 
        /// Depends on the font support for UTF8 characters and the string contents.
        /// Made virtual for optimization purposes.
        virtual uint uiStringLen(char* _szStart,char* _szEnd) const = 0;

        /// Retrieves the current char in the given string.
        virtual uint uiGetCurrentChar(char *_szStr) const = 0;

        /// Retrieves the pointer to the next character in the string.
        virtual char* szGotoNextChar(char *_szStr) const = 0;

        /// Adds character information belonging to the given character.
        virtual void AddChar(uint _uiChar,const CCharInfo& _oCI) = 0;

		/// Retrieves the character information data belonging to the given character.
		virtual const CCharInfo* poGetCharInfo(uint _uiChar) const = 0;

		/// Returns the default char width.
        FEReal rDefCharWidth() const { return(rCharWidth(0)); };

        /// Returns the default char height.
        FEReal rDefCharHeight() const { return(rCharHeight(0)); };

		/// Returns the kerning of the font.
        FEReal rCharKerning() const { return(rCharLeading(0)); };

		/// Returns the kerning of the font between the given chars.
        FEReal rCharKerning(uint _uiLeft,uint _uiRight) const
        {
			return( rCharKerning(poGetCharInfo(_uiLeft),poGetCharInfo(_uiRight)) );
        }

		/// Returns the kerning of the font between the given chars.
        FEReal rCharKerning(const CCharInfo* _poLeft,const CCharInfo* _poRight) const
        {
			if (m_poKT==NULL) return(rCharKerning());
			return( FEReal(m_poKT->iGetKern(_poLeft->m_cLC, _poRight->m_cRC)) );
        };

        /// Returns the material used by the font.
        FEHandler hMaterial() const { return(m_hMaterial); };

    protected:

        /// Sets up the material used by the font
        void SetMaterial(FEHandler _hMaterial) { m_hMaterial = _hMaterial; };

    protected:

		/// The kerning table of the font.
		CFEKernTable* m_poKT;

		/// The material used by this font.
        FEHandler	m_hMaterial;        
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
