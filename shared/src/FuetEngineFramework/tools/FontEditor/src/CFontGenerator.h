//-----------------------------------------------------------------------------
//
// Name:        GTSViewerApp.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//-----------------------------------------------------------------------------
#ifndef CFontGeneratorH
#define CFontGeneratorH
//-----------------------------------------------------------------------------
#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif
#include <vector>
//---------------------------------------------------------------------------
class CSpecialChar
{
	public:

	/// Character to type, only valid for characters in the (32..127) range
	unsigned int	m_cID;			// 

	/// Margins	
	unsigned int	m_uiLeft;
	unsigned int	m_uiRight;
	unsigned int	m_uiTop;
	unsigned int	m_uiBottom;

	/// Character to type, only valid for characters in the (32..127) range
	unsigned int	m_cKCEq;		// kerning class equivalence.

	/// Just to identify easily the color.
	unsigned int	m_uiColor;		// To ease edition.

};
//-----------------------------------------------------------------------------
class COptiFrameSequence;
class CFontGenCharInfo;
class fipImage;
//-----------------------------------------------------------------------------
class CFontGenerator
{
	public:

		static void Generate(const wxFont& _oFont,
							const wxString& _sFilename,
							const wxString& _sCharSet,
							const std::vector<CSpecialChar>& _oSpecialChars);

		/// Sets the kind of constraint restricts the texture generated. (0:none, 1:multiple of 4, 2:multple of 8, 3:power of 2)
		static void SetConstraintType(int _iConstraintType)
		{
			m_iConstraintType = _iConstraintType;
		}

		/// Sets the kind of antialias to use when renderin the font (0: none, 1: light, 2: strong)
		static void SetAntialiasType(int _iAntialiasType)
		{
			m_iAntialiasType = _iAntialiasType;
		}

		// Amount of pixels to leave as a margin around every character. for instance to draw effects like strokes, shadows ...
		static void SetBorderPixels(int _iBorderPixels)
		{
			m_iBorderPixels = _iBorderPixels;
		}

		// Extra pixels to add between characters...
		static void SetForcedTracking(int _iForcedTracking)
		{
			m_iForcedTracking = _iForcedTracking;
		}
		
		// Sets whether we want to produce a fast texture output
		static void SetFastCompact(bool _bFastCompact)
		{
			m_bFastCompact = _bFastCompact;
		}
	
		// Sets the point scale factor, i.e. what scale this font is supposed to represent.
		static void SetPointScale(float _fPointScale)
		{
			m_fPointScale = _fPointScale;
		}

		// Sets a forced Y offset for character rendering.
		static void SetForcedYOffset(int _iForcedYOffset)
		{
			m_iForcedYOffset = _iForcedYOffset;
		}

		// Generation will produce the font texture.
		static void GenerateFontTexture(bool _bGenerateFontTexture)
		{
			m_bGenerateFontTexture = _bGenerateFontTexture;
		}

		// Generation will produce the font definition file.
		static void GenerateFontDefinition(bool _bGenerateFontDefinition)
		{
			m_bGenerateFontDefinition = _bGenerateFontDefinition;
		}

		// Sets the typeface color
		static void SetTypefaceColor(unsigned int _uiTypefaceColor)
		{
			m_uiTypefaceColor = _uiTypefaceColor;
		}

		// Sets the stroke size
		static void SetStrokeSize(float _fStrokeSize)
		{
			m_fStrokeSize = _fStrokeSize;
		}
		
		// Sets the stroke color
		static void SetStrokeColor(unsigned int _uiStrokeColor)
		{
			m_uiStrokeColor = _uiStrokeColor;
		}
		
		// Generation will produce the font texture.
		static void SetUTF8Font(bool _bUTF8Font)
		{
			m_bUTF8Font = _bUTF8Font;
		}

	protected:
	
		static void GenerateFontTexture(const wxString& _sFilename,fipImage* _poImage);
		static void GenerateFontDefinition(std::vector<CFontGenCharInfo>* ,wxMemoryDC* _poDC,COptiFrameSequence* _poFS,const wxString& _sFilename);
		static fipImage* poDrawChar(const wxString& _c,wxDC& _oDC,wxBitmap& _oBmp);

	protected:	

		static int			m_iAntialiasType;
		static bool			m_bFastCompact;
		static float		m_fPointScale;
		static int			m_iForcedYOffset;
		static int			m_iForcedTracking;
		static int			m_iBorderPixels;
		static int			m_iConstraintType;
		static bool			m_bUTF8Font;
		static bool			m_bGenerateFontTexture;		
		static bool			m_bGenerateFontDefinition;
		static float		m_fStrokeSize;
		static unsigned int m_uiStrokeColor;
		static unsigned int m_uiTypefaceColor;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
