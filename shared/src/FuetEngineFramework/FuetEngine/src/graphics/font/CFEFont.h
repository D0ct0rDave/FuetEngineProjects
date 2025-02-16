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
    unsigned short   m_usOfs;

}TCharInfo;

// #pragma options align= reset
// #pragma pack(pop,"")

// ----------------------------------------------------------------------------
class CFEFont
{
    public:

        /// Initializes the font object by using a given material and a buffer of char rect coords
        void Init(FEHandler _hMaterial,TCharInfo* _pBuff) { SetMaterial(_hMaterial); m_oCharTable = _pBuff; };

        /// Retrieves the length of an specific char in pixels
        FEReal rCharWidth(unsigned char _ucChar) const { return((FEReal)m_oCharTable[_ucChar].m_usCW); };

        /// Retrieves the height of an specific char in pixels.
        FEReal rCharHeight(unsigned char _ucChar) const  { return((FEReal)m_oCharTable[_ucChar].m_usCH); };

        /// Retrieves the offset of an specific char in pixels.
        FEReal rCharOfs(unsigned char _ucChar) const  { return((FEReal)m_oCharTable[_ucChar].m_usOfs); };

		/// Retrieves the texture coordinates of a given character.
		const CFERect& oCharUVs(unsigned char _ucChar) const { return(m_oCharTable[_ucChar].m_oUV); };

        /// Retrieves the length of the string in pixels.
        FEReal rStringLen(const CFEString& _sString) const;

		/// Returns the default char width.
        FEReal rDefCharWidth() const { return(rCharWidth(0)); };

        /// Returns the default char height.
        FEReal rDefCharHeight() const { return(rCharHeight(0)); };

		/// Returns the kerning of the font.
        FEReal rCharKerning() const { return(rCharOfs(0)); };

        /// Returns the material used by the font.
        FEHandler hMaterial() const { return(m_hMaterial); };

    protected:

        /// Sets up the material used by the font
        void SetMaterial(FEHandler _hMaterial) { m_hMaterial = _hMaterial; };

        /// Parses the length of the chars stored in the given buffer
        void ParseCoords(FEPointer _pBuff);

    protected:

		TCharInfo*	m_oCharTable;
        FEHandler	m_hMaterial;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
