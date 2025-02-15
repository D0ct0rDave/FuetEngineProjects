// ----------------------------------------------------------------------------
/*! \class CFEFont
 *  \brief Font Class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

typedef struct TCharInfo{

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

}TCharInfo;

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
		~CFEFont();

        /// Initializes the font object by using a given material, a buffer of char rect coords, and an optional kerning table.
        void Init(FEHandler _hMaterial,TCharInfo* _pBuff,CFEKernTable* _poKT = NULL) { SetMaterial(_hMaterial); m_oCharTable = _pBuff; m_poKT = _poKT; };

        /// Retrieves the length of an specific char in pixels
        FEReal rCharWidth(unsigned char _ucChar) const { return(FEReal(m_oCharTable[_ucChar].m_usCW)); };

        /// Retrieves the height of an specific char in pixels.
        FEReal rCharHeight(unsigned char _ucChar) const  { return(FEReal(m_oCharTable[_ucChar].m_usCH)); };

        /// Retrieves the Y offset to add to the start of the character.
        FEReal rCharLeading(unsigned char _ucChar) const  { return(FEReal(m_oCharTable[_ucChar].m_sYOfs)); };

		/// Retrieves the texture coordinates of a given character.
		const CFERect& oCharUVs(unsigned char _ucChar) const { return(m_oCharTable[_ucChar].m_oUV); };

        /// Retrieves the length of the string in pixels.
        FEReal rStringLen(const CFEString& _sString) const;

		/// Returns the default char width.
        FEReal rDefCharWidth() const { return(rCharWidth(0)); };

        /// Returns the default char height.
        FEReal rDefCharHeight() const { return(rCharHeight(0)); };

		/// Returns the kerning of the font.
        FEReal rCharKerning() const { return(rCharLeading(0)); };

		/// Returns the kerning of the font between the given chars.
        FEReal rCharKerning(unsigned char _ucLeft,unsigned char _ucRight) const
        {
			if (m_poKT==NULL) return(rCharKerning());
			return( FEReal(m_poKT->iGetKern( m_oCharTable[_ucLeft].m_cLC, m_oCharTable[_ucRight].m_cRC)) );
        };

        /// Returns the material used by the font.
        FEHandler hMaterial() const { return(m_hMaterial); };

    protected:

        /// Sets up the material used by the font
        void SetMaterial(FEHandler _hMaterial) { m_hMaterial = _hMaterial; };

        /// Parses the length of the chars stored in the given buffer
        void ParseCoords(FEPointer _pBuff);

    protected:

		/// The character table.
		TCharInfo*	m_oCharTable;

		/// The kerning table of the font.
		CFEKernTable* m_poKT;

		/// The material used by this font.
        FEHandler	m_hMaterial;        
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
