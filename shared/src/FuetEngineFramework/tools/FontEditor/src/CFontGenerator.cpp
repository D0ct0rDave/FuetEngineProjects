//-----------------------------------------------------------------------------
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//-----------------------------------------------------------------------------
#include "CFontGenerator.h"

#include <FreeImage.h>
#include <FreeImagePlus.h>
#include "CFrameOptimizer.h"
#include "CFastFrameOptimizer.h"
#include "CBSPFrameOptimizer.h"
#include "CConfigFileWriter.h"
#include "CLibConfigFileWriter.h"
#include "edutils.h"

#define USE_CAIRO
#ifdef USE_CAIRO
#define LIBCAIRO_EXPORTS
#include <cairo.h>
#include <cairo-win32.h>
#endif
//-----------------------------------------------------------------------------
// Generator properties.
//-----------------------------------------------------------------------------
float	CFontGenerator::m_fPointScale = 1.0f;
int		CFontGenerator::m_iForcedTracking = 0;
int		CFontGenerator::m_iBorderPixels = 0;
int		CFontGenerator::m_iForcedYOffset = 0;
int		CFontGenerator::m_iConstraintType = 0;
bool	CFontGenerator::m_bGenerateFontTexture = true;
bool	CFontGenerator::m_bGenerateFontDefinition = true;
bool	CFontGenerator::m_bFastCompact = false;
bool	CFontGenerator::m_bUTF8Font= false;
int		CFontGenerator::m_iAntialiasType = 1;
float	CFontGenerator::m_fStrokeSize = 0.0f;
unsigned int CFontGenerator::m_uiStrokeColor = 0xffffffff;
unsigned int CFontGenerator::m_uiTypefaceColor = 0xffffffff;
//-----------------------------------------------------------------------------
// const uint MAX_CHARS = 256;
fipImage* poGetImage(wxBitmap* _poBmp,uint _uiWidth,uint _uiHeight);
//-----------------------------------------------------------------------------

/*
Generate Kerning classes and kerning table.
usamos la O (o mayúscula) para generar las clases por la izquierda y por la derecha

typdef struct {
	
	...
	int m_iRightClass;
	int m_iLeftClass;

	...
	
}TCharInfo;

int LeftOffsets[MAX_CHARS] =
{
}

int RightOffsets[MAX_CHARS] =
{
}

Hacemos un histograma de esto para obtener cuantas clases salen de la izquierda y de la derecha.

Rellenamos la tabla:
para cada valor del histograma de la derecha(clase) cogemos un caracter representativo de dicha clase
	para cada valor del histograma de la izquierda(clase) cogemos un caracter representativo de dicha clase
	{
		// obtenemos el desplazamiento a realizar entre dichos caracteres:
		KernTable[right_class][left_class] = Width("right_char" + "left_char") - (Width(right_char) + Width(left_char))
	}
*/
//-----------------------------------------------------------------------------
class CFontGenCharInfo
{
	public:

		bool			m_bSpecial;		// is this character special
		wxString		m_sKCEq_UTF8Char;		// what is the character who's kerning class is equivalent to this char?
		
		uint			m_uiIdx;
		int				m_iYOfs;
		
		wxString		m_sChar;	// UTF-8 Character
		
	//	int				m_iXOfs;
		uint			m_uiLeftClass;
		uint			m_uiRightClass;
};
//-----------------------------------------------------------------------------
class CClassInfo
{
	public:
		bool operator < (const CClassInfo& b)
		{
			return(m_uiCount>b.m_uiCount);
		}

	int			m_iOfs;
	uint		m_uiCount;
	wxString	m_sChar;	
};
//-----------------------------------------------------------------------------
uint uiGetStringWidth(const wxString& _sStr ,wxMemoryDC* _poDC)
{
	const uint MAX_WIDTH = 1024;
	const uint MAX_HEIGHT = 1024;

	int XSafeOfs = MAX_WIDTH/4;
	int YSafeOfs = MAX_HEIGHT/4;

	_poDC->Clear();
	_poDC->DrawText(_sStr,XSafeOfs,YSafeOfs);

	wxBitmap oBitmap = _poDC->GetAsBitmap();

	// Get FIP image from canvas
	// fipImage* poImg = poGetImage(&oBitmap,MAX_WIDTH,MAX_HEIGHT);
	fipImage* poImg = poGetImage(&oBitmap,MAX_WIDTH,MAX_HEIGHT);
	CFrame* poFrame = new CFrame( *poImg,0xff000000,0);

	uint uiWidth = poFrame->oGetRect().m_oEnd.x - poFrame->oGetRect().m_oIni.x;
	
	delete poFrame;
	
	return (uiWidth);
	
}
//-----------------------------------------------------------------------------
int iGetCharWidth(const wxString& _sChar,wxMemoryDC* _poDC)
{
	wxString sStr = _sChar;
	sStr = sStr + sStr + sStr + sStr + sStr;

	wxCoord lCharWidth, lCharHeight, lCharDescent, lExternalLeading;
	_poDC->GetTextExtent(sStr, &lCharWidth, &lCharHeight, &lCharDescent, &lExternalLeading);

	int iCW = lCharWidth / 5;
	return(iCW);
}
//-----------------------------------------------------------------------------
int iGetKerning(const wxString& _cLeft,const wxString& _cRight,wxMemoryDC* _poDC)
{
	int iLW,iRW,iLRW;
	
	wxString sL = _cLeft;
	wxString sR = _cRight;
	wxString sLR = sL + sR;

	wxCoord lCharWidth, lCharHeight, lCharDescent, lExternalLeading;
	_poDC->GetTextExtent(sL, &lCharWidth, &lCharHeight, &lCharDescent, &lExternalLeading);
	iLW = lCharWidth;
	_poDC->GetTextExtent(sR, &lCharWidth, &lCharHeight, &lCharDescent, &lExternalLeading);
	iRW = lCharWidth;
	_poDC->GetTextExtent(sLR, &lCharWidth, &lCharHeight, &lCharDescent, &lExternalLeading);
	iLRW = lCharWidth;	

	/*
	iLW = iGetStringWidth(sL,_poDC);
	iRW = iGetStringWidth(sR,_poDC);
	iLRW  = iGetStringWidth(sLR,_poDC);
	*/

	return( iLRW - (iLW + iRW) );
}
//-----------------------------------------------------------------------------
void GenerateClasses(int* _iOffsets,std::vector<CFontGenCharInfo>* _poCI,std::vector <CClassInfo>* _poCIList)
{
	for (uint i=0;i<_poCI->size();i++)
	{
		if (_poCI->at(i).m_bSpecial) continue;

		// exists the offset in the vector?
		int iIdx = -1;
		for (uint j=0;j<_poCIList->size();j++)
		{
			if (_poCIList->at(j).m_iOfs == _iOffsets[i])
			{
				iIdx = j;
				break;
			}
		}

		if (iIdx == -1)
		{
			CClassInfo oInfo;
			oInfo.m_iOfs    = _iOffsets[i];
			oInfo.m_uiCount = 1;
			oInfo.m_sChar   = _poCI->at(i).m_sChar;

			_poCIList->push_back(oInfo);
		}
		else
		{
			_poCIList->at(iIdx).m_uiCount++;
		}
	}	
	
	std::sort(_poCIList->begin(),_poCIList->end());
}
//-----------------------------------------------------------------------------
CFEKernTable* poGenerateKerning(std::vector<CFontGenCharInfo>* _poCI,wxMemoryDC* _poDC)
{
	uint uiSize = sizeof(int)*_poCI->size();
	int* iLeftOffsets  = (int*)malloc(uiSize);
	int* iRightOffsets = (int*)malloc(uiSize);
	memset(iLeftOffsets,0,uiSize);
	memset(iRightOffsets,0,uiSize);

	// build offset table
	uint c,l,r;
	for (c=0;c<_poCI->size();c++)
	{
		if (! _poCI->at(c).m_bSpecial)
		{
			iLeftOffsets[c]  = iGetKerning(_poCI->at(c).m_sChar,_T('O'),_poDC);
			iRightOffsets[c] = iGetKerning(_T('O'),_poCI->at(c).m_sChar,_poDC);
		}
		else
		{
			iLeftOffsets[c]  = iGetKerning(_poCI->at(c).m_sKCEq_UTF8Char,_T('O'),_poDC);
			iRightOffsets[c] = iGetKerning(_T('O'),_poCI->at(c).m_sKCEq_UTF8Char,_poDC);
		}
	}

	std::vector <CClassInfo> oLeftClasses;
	GenerateClasses(iLeftOffsets,_poCI,&oLeftClasses);

	std::vector <CClassInfo> oRightClasses;
	GenerateClasses(iRightOffsets,_poCI,&oRightClasses);

	// Assign character classes
	for (c=0;c<_poCI->size();c++)
	{
		for (r=0;r<oRightClasses.size();r++)
		{
			if (iRightOffsets[c] == oRightClasses[r].m_iOfs)
			{
				_poCI->at(c).m_uiRightClass = r;
				break;
			}
		}

		for (l=0;l<oLeftClasses.size();l++)
		{
			if (iLeftOffsets[c] == oLeftClasses[l].m_iOfs)
			{
				_poCI->at(c).m_uiLeftClass = l;
				break;
			}
		}
	}

	// Build the kern table
	CFEKernTable* poKT = new CFEKernTable(oLeftClasses.size(),oRightClasses.size());
	for (r=0;r<oRightClasses.size();r++)
		for (l=0;l<oLeftClasses.size();l++)
		{
			// Any of the letters belonging to the left class "l" when are followed by any of the letters belonging to the right class "r"
			// will have the same kerning value.
			int iKerning = iGetKerning(oLeftClasses[l].m_sChar,oRightClasses[r].m_sChar,_poDC);
			poKT->SetKern(l,r,iKerning);
		}

	/// free offset arrays
	free(iLeftOffsets);
	free(iRightOffsets);

	return(poKT);
}
//---------------------------------------------------------------------------
uint uiRoundUP(uint	_uiValue,uint	_uiRUVal)
{
	uint	 uiVal = ((_uiValue + (_uiRUVal-1)) / _uiRUVal) * _uiRUVal;
	return(uiVal);
}
//---------------------------------------------------------------------------
uint uiNextPO2(uint _uiValue)
{
    uint  uiPO2 = 1;
    while (uiPO2 < _uiValue) uiPO2 <<= 1;
    return(uiPO2);
}
//---------------------------------------------------------------------------
// Retrieves a value using a given parameter and the specified dimensions 
// constraint type
//---------------------------------------------------------------------------
uint  uiGetDim(uint  _uiValue,int _iConstraintType)
{
	switch (_iConstraintType)
	{
	    // no constraint
		case 0:
		return(_uiValue);		
		break;
		
		// multiple of 4
        case 1:
        {
            return (uiRoundUP(_uiValue,4));
        }
        break;

		// multiple of 8
        case 2:
        {
            return (uiRoundUP(_uiValue,8));
        }
        break;

		// power of 2
        case 3:
        {
            return (uiNextPO2(_uiValue));
        }
        break;
	}
	
	return(_uiValue);
}
//---------------------------------------------------------------------------
// Retrieves minimum canvas size dimensions needed to store the given char list
// taking into account the given font and selected chars.
//---------------------------------------------------------------------------
void ComputeCanvasSize(const wxFont& _oFont,const wxString& _sSelectedChars,uint * _puiWidth,uint * _puiHeight,int _iConstraintType)
{
	// Take into account already checked chars
	bool bUsed[256];
	memset(bUsed,0,sizeof(bUsed));

	// Setup the display context
    wxBitmap oBitmap(256,256);
    wxMemoryDC oDC(oBitmap);
	oDC.SetFont(_oFont);

    // Retrieve max char size and total number of selected chars.
	int iMaxCharWidth  = 0;
	int iMaxCharHeight = 0;
	int iSelectedChars = 0;

	for (uint i=0;i<_sSelectedChars.Length();i++)
	{
		wxString sChar = _sSelectedChars.SubString(i,i);
		const char* szChar = sChar.ToAscii();
		char cChar = *szChar;

		if (bUsed[cChar] == false)
		{
			bUsed[cChar] = true;

			wxCoord lCharWidth, lCharHeight, lCharDescent, lExternalLeading;
		    oDC.GetTextExtent(sChar, &lCharWidth, &lCharHeight, &lCharDescent, &lExternalLeading, (wxFont*)&_oFont);

			lCharWidth += 1; // +1 pix of margin
			lCharHeight+= 1;

			if (lCharWidth > iMaxCharWidth)
			    iMaxCharWidth = lCharWidth;

			if (lCharHeight > iMaxCharHeight)
			    iMaxCharHeight = lCharHeight;

			iSelectedChars ++;
		}
	}
	
	// Supper rough approx.
	float fArea = iSelectedChars * (iMaxCharWidth*iMaxCharHeight);

	*_puiWidth  = uiGetDim((int)sqrtf(fArea) + 1, _iConstraintType);
	*_puiHeight  = uiGetDim(*_puiWidth * 2, _iConstraintType);
}
//---------------------------------------------------------------------------
fipImage* poGetImage(wxBitmap* _poBmp,uint  _uiWidth,uint  _uiHeight)
{
	
	fipImage* poImage = new fipImage;
	poImage->setSize(FIT_BITMAP,_uiWidth,_uiHeight,32);

    wxImage oImg = _poBmp->ConvertToImage();
    uint uiDrawnPixels = 0;

    if (oImg.HasAlpha())
    {
		// The bitmap to be saved oh disk    
		uint uiDrawnPixels = 0;
		for (uint j=0;j<_uiHeight;j++)
		{
			for (uint i=0;i<_uiWidth;i++)
			{
				unsigned char ucR = oImg.GetRed(i,j);
				unsigned char ucG = oImg.GetGreen(i,j);
				unsigned char ucB = oImg.GetBlue(i,j);
				unsigned char ucA = oImg.GetAlpha(i,j);
	               
				RGBQUAD oColor;
				oColor.rgbRed   = ucR;
				oColor.rgbGreen = ucG;
				oColor.rgbBlue  = ucB;
				oColor.rgbReserved = ucA;

				if (ucG!=0)
					uiDrawnPixels++;

				poImage->setPixelColor(i,(_uiHeight-j-1),&oColor);
			}
		}
	}
	else
	{
		// The bitmap to be saved oh disk    
		for (uint j=0;j<_uiHeight;j++)
		{
			for (uint i=0;i<_uiWidth;i++)
			{
				unsigned char ucR = 255;
				unsigned char ucG = 255;
				unsigned char ucB = 255;
				unsigned char ucA = oImg.GetRed(i,j);
	               
				RGBQUAD oColor;
				oColor.rgbRed		= ucR;
				oColor.rgbGreen		= ucG;
				oColor.rgbBlue		= ucB;
				oColor.rgbReserved  = ucA;

				if (ucA != 0)
					uiDrawnPixels++;

				poImage->setPixelColor(i,(_uiHeight-j-1),&oColor);
			}
		}
	}

    if (uiDrawnPixels == 0)
    {
		delete poImage;
		poImage = NULL;
    }

    return( poImage);
}
//---------------------------------------------------------------------------
void FillColorRect(fipImage* _poImage,uint  _uiX,uint  _uiY,uint  _uiWidth,uint  _uiHeight,uint  _uiColor)
{
	for (uint j=_uiY;j<_uiY+_uiHeight;j++)
    {
        for (uint i=_uiX;i<_uiX+_uiWidth;i++)
        {
            RGBQUAD oColor;
            _poImage->getPixelColor(i,(_uiHeight-j-1),&oColor);

            oColor.rgbRed   = _uiColor & 0x000000ff;
			oColor.rgbGreen = (_uiColor >> 8)& 0x000000ff;;
			oColor.rgbBlue  = (_uiColor >> 16)& 0x000000ff;;

            _poImage->setPixelColor(i,(_uiHeight-j-1),&oColor);
        }
    }
}
//-----------------------------------------------------------------------------
const uint  MAX_WIDTH  = 1024;
const uint  MAX_HEIGHT = 1024;
const int XSafeOfs = MAX_WIDTH/4;
const int YSafeOfs = MAX_HEIGHT/4;
//-----------------------------------------------------------------------------
fipImage* CFontGenerator::poDrawChar(const wxString& _c,wxDC& _oDC,wxBitmap& _oBmp)
{
	#ifdef USE_CAIRO

		wxFont oFont = _oDC.GetFont();

		// Create a cairo surface we can draw on directly
		fipImage* poImage = new fipImage;
		poImage->setSize(FIT_BITMAP,MAX_WIDTH,MAX_HEIGHT,32);
		cairo_surface_t * cairo_surface = cairo_image_surface_create_for_data (
														 poImage->accessPixels(),
                                                         CAIRO_FORMAT_ARGB32,
                                                         poImage->getWidth(),
                                                         poImage->getHeight(),
                                                         poImage->getScanWidth());
		cairo_t* cr = cairo_create(cairo_surface);
		
		/*
		cairo_font_face_t* font = cairo_win32_font_face_create_for_hfont( (HFONT)_oDC.GetFont().GetHFONT() );
		// cairo_set_font_face(cr,font);
		
		cairo_matrix_t				m,u;
		cairo_matrix_init_identity(&m);
		cairo_matrix_init_identity(&u);
		cairo_matrix_scale(&m,fFontSize,fFontSize);
		cairo_matrix_scale(&u,10,10);

		cairo_font_options_t* font_opt = cairo_font_options_create();
		cairo_font_options_set_antialias(font_opt,CAIRO_ANTIALIAS_DEFAULT);

		cairo_scaled_font_t* scaled_font = cairo_scaled_font_create(font,&m,&u,font_opt);
		cairo_set_scaled_font(cr,scaled_font);
		*/

		cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);
		cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
		
		// Draw the character into the cairo surface!
		double dFontScale = (oFont.GetPointSize()*100)/75;
		cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;		
		switch (oFont.GetWeight())
		{
			case wxFONTWEIGHT_NORMAL:
			weight = CAIRO_FONT_WEIGHT_NORMAL;
			break;
			
			case wxFONTWEIGHT_BOLD:
			weight = CAIRO_FONT_WEIGHT_BOLD;
			break;

			case wxFONTWEIGHT_LIGHT:
			weight = CAIRO_FONT_WEIGHT_NORMAL;
			break;
		
		}

		cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL;
		switch (oFont.GetStyle())
		{
			case wxFONTSTYLE_NORMAL:
			slant = CAIRO_FONT_SLANT_NORMAL;
			break;

			case wxFONTSTYLE_ITALIC:
			slant = CAIRO_FONT_SLANT_ITALIC;
			break;
		
			case wxFONTSTYLE_SLANT:
			slant = CAIRO_FONT_SLANT_OBLIQUE;
			break;
		}

        
		cairo_select_font_face (cr, oFont.GetFaceName().char_str(), slant, weight);
		cairo_set_font_size (cr, dFontScale );

		cairo_font_extents_t fe;
		cairo_font_extents(cr,&fe);
		double x = 0;
		double y = fe.ascent;
		cairo_user_to_device(cr,&x,&y);
		// y = 0;


		// CAIRO_ANTIALIAS_NONE,
		// CAIRO_ANTIALIAS_GRAY,
		// CAIRO_ANTIALIAS_SUBPIXEL
		cairo_font_options_t* font_opt = cairo_font_options_create();
		
		switch (m_iAntialiasType)
		{
			// none
			case 0:
			cairo_font_options_set_antialias(font_opt,CAIRO_ANTIALIAS_NONE);
			break;
			
			// light
			case 1:
			cairo_font_options_set_antialias(font_opt,CAIRO_ANTIALIAS_SUBPIXEL);
			break;
			
			// hard
			case 2:
			// super
			case 3:
			cairo_font_options_set_antialias(font_opt,CAIRO_ANTIALIAS_GRAY);
			break;
		}

		wxCharBuffer cUTF8Char = _c.ToUTF8();
		// char cTheChar[2] = {_c,0};

		if (m_fStrokeSize == 0.0f)
		{
			FillColorRect(poImage,0,0,MAX_WIDTH,MAX_HEIGHT,m_uiTypefaceColor);
		}
		else
		{
			FillColorRect(poImage,0,0,MAX_WIDTH,MAX_HEIGHT,m_uiStrokeColor);

			cairo_move_to (cr, XSafeOfs+m_iBorderPixels, y + YSafeOfs+m_iBorderPixels);
			cairo_text_path (cr,cUTF8Char);

			// cairo_set_source_rgb (cr, 1, 1, 1);
			// cairo_fill_preserve (cr);

			float fR = ( m_uiStrokeColor       & 0x000000ff) / 255.0f;
			float fG = ((m_uiStrokeColor >>  8)& 0x000000ff) / 255.0f;
			float fB = ((m_uiStrokeColor >> 16)& 0x000000ff) / 255.0f;

			cairo_set_source_rgb (cr, fR,fG,fB);
			cairo_set_line_width (cr, m_fStrokeSize*2.0f);
			cairo_stroke (cr);
		}

		cairo_font_options_set_hint_style(font_opt,CAIRO_HINT_STYLE_FULL);
		cairo_set_font_options(cr,font_opt);
		cairo_set_line_width(cr, 0.0f);

		float fR = ( m_uiTypefaceColor       & 0x000000ff) / 255.0f;
		float fG = ((m_uiTypefaceColor >>  8)& 0x000000ff) / 255.0f;
		float fB = ((m_uiTypefaceColor >> 16)& 0x000000ff) / 255.0f;

		cairo_set_source_rgba (cr, fR,fG,fB,1);
		cairo_move_to (cr, XSafeOfs+m_iBorderPixels, y + YSafeOfs+m_iBorderPixels);
		cairo_show_text (cr,cUTF8Char );
        
		if (m_iAntialiasType == 3)
		{
			// perform another step with light antialias
			cairo_move_to (cr, XSafeOfs+m_iBorderPixels, y + YSafeOfs+m_iBorderPixels);
			cairo_font_options_set_antialias(font_opt,CAIRO_ANTIALIAS_SUBPIXEL);
			cairo_show_text (cr,cUTF8Char);
		}
		
		// Tear down the cairo object now that we don't need
		// it anymore.
		cairo_destroy(cr);
		cairo_surface_destroy(cairo_surface);
		cairo_font_options_destroy(font_opt);

		/*
		cairo_font_face_destroy(font);
		cairo_scaled_font_destroy(scaled_font);
		*/

		// flip image
		poImage->flipVertical();
		return( poImage );
	#else

		wxString sChar = _c;

		// Draw a single character into the canvas.
		_oDC.Clear();
		_oDC.DrawText(sChar,XSafeOfs+m_iBorderPixels,YSafeOfs+m_iBorderPixels);

		// Get FIP image from canvas
		return( poGetImage(&_oBmp,MAX_WIDTH,MAX_HEIGHT) );
	#endif
}

//-----------------------------------------------------------------------------
void CFontGenerator::Generate(const wxFont& _oFont,const wxString& _sFilename,const wxString& _sCharSet,const std::vector<CSpecialChar>& _oSpecialChars)
{
	// Setup the maximum alloable dimensions for a char.
    // Create bitmap to write font letters
    wxBitmap oBitmap(MAX_WIDTH,MAX_HEIGHT);
    wxMemoryDC oDC(oBitmap);

	// Setup the display context
	oDC.SetFont(_oFont);
	oDC.SetBackground(wxBrush(wxT("black"), wxSOLID));
	oDC.SetTextForeground(*wxWHITE);

	// Create and initialize chars structure
	std::vector<CFontGenCharInfo> oChars;

	// Parse char by char
	CFrameSequence oFS("font");
	
	/*
	TEXTMETRIC txtMet;
	GetTextMetrics((HDC)oDC.GetHDC(),&txtMet);
	txtMet.tmAscent 
	*/

	uint i;
	for (i=0;i<_sCharSet.Length();i++)
	{
		wxString sChar = _sCharSet.SubString(i,i);

        // ignore spaces
        if (sChar == _T(" ")) continue;
		
		// see if the character is already in the list:
		bool bIsThere = false;
		for (uint j=0;((j<oChars.size()) && (!bIsThere));j++)
			bIsThere = (oChars[j].m_sChar == sChar);

		if (bIsThere == false)
		{
			fipImage* poImg = poDrawChar(sChar,oDC,oBitmap);
			
			// CFEString sFilenameToSave = CFEString("c:\\")+CFEString(szChar)+".png";
			// poImg->save(sFilenameToSave.szString());
			if (poImg != NULL)
			{
				oFS.AddFrame( new CFrame( *poImg,0xff000000,m_iBorderPixels) );
				delete poImg;

				// consider this char has been written
				CFontGenCharInfo oChar;
				oChar.m_uiIdx    = oFS.m_oFrames.size()-1;
				oChar.m_iYOfs    = oFS.m_oFrames[ oChar.m_uiIdx ]->oGetRect().m_oIni.y - YSafeOfs;
				oChar.m_bSpecial = false;
				oChar.m_sChar    = sChar;

				// add character to the list
				oChars.push_back(oChar);
			}
		}
		else
		{
		
		}
	}

	/// ---------------------------------------------------
	/// add special chars to the sequence of frames...
	/// ---------------------------------------------------
	for (i=0;i<_oSpecialChars.size();i++)
	{
		wxString sChar = wxString::Format(wxT("%c"),_oSpecialChars[i].m_cID);

		// see if the character is already in the list
		bool bIsThere = false;
		for (uint j=0;((j<oChars.size()) && (!bIsThere));j++)
			bIsThere = (oChars[j].m_sChar == sChar);

		if (bIsThere == false)
		{
			// ---------------------------
			// To ease locating the character in the texture ....			
			// ---------------------------
			CFrame* poFrame = new CFrame();
			int iSpecialCharYOfs = 0;

			// Draw a single character into the canvas.
			wxString sUTF8Char = wxString::Format(wxT("%c"),_oSpecialChars[i].m_cKCEq);
			fipImage* poImg = poDrawChar(sUTF8Char,oDC,oBitmap);

			// Get FIP image from canvas
			if (poImg != NULL)
			{
				// create the equivalent character frame.
				CFrame* poEqCharFrame = new CFrame( *poImg,0xff000000,m_iBorderPixels);

				// Retrieve the YOfs
				CFrameSequence oSFS("special");
				oSFS.AddFrame( new CFrame( *poImg,0xff000000,m_iBorderPixels) );
				iSpecialCharYOfs = (oSFS.m_oFrames[0]->oGetRect().m_oIni.y - YSafeOfs) - _oSpecialChars[i].m_uiTop;

				// create the special character frame.
				poFrame->Create(poEqCharFrame->oGetImage().getWidth()  + _oSpecialChars[i].m_uiLeft + _oSpecialChars[i].m_uiRight,
								poEqCharFrame->oGetImage().getHeight() + _oSpecialChars[i].m_uiBottom + _oSpecialChars[i].m_uiTop,
								0xff000000);

				int iXOfs = _oSpecialChars[i].m_uiLeft;
				int iYOfs = _oSpecialChars[i].m_uiTop;
				poFrame->oGetImage().pasteSubImage(poEqCharFrame->oGetImage(),iXOfs,iYOfs);
				FillColorRect(&poFrame->oGetImage(),0,0,poFrame->oGetImage().getWidth(),poFrame->oGetImage().getHeight(), _oSpecialChars[i].m_uiColor );

				delete poImg; // ???
				delete poEqCharFrame;

				oFS.AddFrame(poFrame);

				CFontGenCharInfo oChar;
				oChar.m_sChar    = wxString::Format(wxT("%c"),_oSpecialChars[i].m_cID);
				oChar.m_uiIdx    = oFS.m_oFrames.size()-1;
				oChar.m_iYOfs    = iSpecialCharYOfs;
				oChar.m_bSpecial = true;
				oChar.m_sKCEq_UTF8Char = wxString::Format(wxT("%c"),_oSpecialChars[i].m_cKCEq);

				// add character to the list
				oChars.push_back(oChar);
			}
		}
	}

	/// ---------------------------------------------------
	// Optimize the font layout.
	/// ---------------------------------------------------
	COptiFrameSequence* poFS = NULL;
	if (m_bFastCompact == false)
	{
		// CFrameOptimizer oOptimizer;
		CBSPFrameOptimizer oOptimizer;
		poFS = oOptimizer.poProcess(oFS);
	}
	else
	{
		CFastFrameOptimizer oOptimizer;
		poFS = oOptimizer.poProcess(oFS);
	}

	/// ---------------------------------------------------
	// Apply texture constraints
	/// ---------------------------------------------------
    if (m_iConstraintType != 3)
    {
        // Compute minimum containment rect.
        FEReal rMaxX = -1e6;
        FEReal rMaxY = -1e6;
 
        for (i=0;i<poFS->m_oRects.size();i++)
        {
            if (poFS->m_oRects[i].m_oEnd.x > rMaxX) rMaxX = poFS->m_oRects[i].m_oEnd.x;
            if (poFS->m_oRects[i].m_oEnd.y > rMaxY) rMaxY = poFS->m_oRects[i].m_oEnd.y;
        }

        // Recompute new image dimensions according constraint.
        uint uiMAX_WIDTH = CFEMath::rMin(uiGetDim(rMaxX+1,m_iConstraintType),poFS->m_oFrameSeq.getWidth());
        uint uiMAX_HEIGHT= CFEMath::rMin(uiGetDim(rMaxY+1,m_iConstraintType),poFS->m_oFrameSeq.getHeight());

        // Crop the image depending on new dimensions.
        poFS->m_oFrameSeq.crop(0,0,uiMAX_WIDTH,uiMAX_HEIGHT);
    }

	/// ---------------------------------------------------
	// Save pixel data
	/// ---------------------------------------------------
	if (m_bGenerateFontTexture)
		GenerateFontTexture(_sFilename,&poFS->m_oFrameSeq);

	/// ---------------------------------------------------
	// Generate and save definition data
	/// ---------------------------------------------------
	if (m_bGenerateFontDefinition)
		GenerateFontDefinition(&oChars,&oDC,poFS,_sFilename);
}
//-----------------------------------------------------------------------------
void CFontGenerator::GenerateFontTexture(const wxString& _sFilename,fipImage* _poImage)
{
	wxString sFilename = _sFilename + wxT(".png");

	if (! _poImage->save(sFilename.ToAscii()))
		ShowFileErrorMessage(sFilename);
}
// ----------------------------------------------------------------------------
uint uiGetCurrentChar(char *_szStr)
{
/*
	uint uiValue = *(uint*)_szStr;
	if (uiValue <     0x7f) return(uiValue &= 0x000000ff);
	if (uiValue <   0x7fff) return(uiValue &= 0x0000ffff);
	if (uiValue < 0x7fffff) return(uiValue &= 0x00ffffff);
	return(uiValue);
*/
	// ((char & 11000000) == 10000000) -> continue
	// range(char) == range(_szStr[x]) € (-128..127)

	// if _szStr[0] >= 0, _szStr[0]<0x80 -> simple ASCII char
	if (_szStr[0] >= 0) return( (uint) *((uint *)_szStr) & 0x000000ff );  // return directly
	
	// now szStr[0] > 0x80 -> 
	// in UTF8 if szStr[0] == 11xx xxxx -> there is always a second char
	// if ((_szStr[1] & 0xc0) != 0x80)) // comment because there is always a second char

	// But a third may be not!
	if ((_szStr[2] & 0xc0) != 0x80) return( (uint) *((uint *)_szStr) & 0x0000ffff );
	if ((_szStr[3] & 0xc0) != 0x80) return( (uint) *((uint *)_szStr) & 0x00ffffff );
	return( *(uint*)_szStr );
}
//-----------------------------------------------------------------------------
void CFontGenerator::GenerateFontDefinition(std::vector<CFontGenCharInfo>* _poCI,wxMemoryDC* _poDC,COptiFrameSequence* _poFS,const wxString& _sFilename)
{
	uint  MAX_WIDTH  = _poFS->m_oFrameSeq.getWidth();
	uint  MAX_HEIGHT = _poFS->m_oFrameSeq.getHeight();
	int iDefaultCharWidth   = _poDC->GetCharWidth();
	int iDefaultCharHeight  = _poDC->GetCharHeight();

	// Generate the kerning table.
	CFEKernTable* poKT = poGenerateKerning(_poCI,_poDC);

	// Save coordinate data.
	wxString sFilename = _sFilename + wxT(".fon");

	CLibConfigFileWriter oFile;;
	if (oFile.bCreate( std::string(sFilename.char_str()) ))
	{
        // Write dimensions.
		oFile.OpenSection("Font");
			oFile.AddVar("Type","OpenType","");
			oFile.OpenSection("OpenType");

				oFile.AddVar("UTF8", (bool)m_bUTF8Font,false);

				oFile.AddVar("BaseWidth", (int)(m_fPointScale*_poFS->m_oFrameSeq.getWidth()),0);
				oFile.AddVar("BaseHeight",(int)(m_fPointScale*_poFS->m_oFrameSeq.getHeight()),0);

				// Write default values.
				oFile.AddVar("DefCharWidth",(int)(m_fPointScale*iDefaultCharWidth),0);
				oFile.AddVar("DefCharHeight",(int)(m_fPointScale*iDefaultCharHeight),0);

				int iSpaceWidth = iGetCharWidth(_T(" "),_poDC); // se dice que la longitud del espacio es la de una letra '·' o una letra 'a'.
				oFile.AddVar("SpaceCharWidth",(int)(m_fPointScale*iSpaceWidth),(int)(m_fPointScale*iDefaultCharWidth));

				// Write the rest of the chars...
				uint j = 0;
				float f1Pixel = 1.0f / (float)MAX_WIDTH;
				for (uint i=0;i<_poCI->size();i++)
				{
					uint uiIdx = _poCI->at(i).m_uiIdx;

    				unsigned short usCharX      =  _poFS->m_oRects[ uiIdx ].m_oIni.x+1;
    				unsigned short usCharY      =  _poFS->m_oRects[ uiIdx ].m_oIni.y+1;
    				unsigned short usCharEX     =  _poFS->m_oRects[ uiIdx ].m_oEnd.x-1;
    				unsigned short usCharEY     =  _poFS->m_oRects[ uiIdx ].m_oEnd.y-1;
    				unsigned short usCharWidth  = usCharEX - usCharX + 1;
    				unsigned short usCharHeight = usCharEY - usCharY + 1;

					// Write char number.
					uint iCharID = 0;
					if (m_bUTF8Font)
					{
						wxCharBuffer cb = _poCI->at(i).m_sChar.ToUTF8();
						iCharID = uiGetCurrentChar(cb.data());

						if ((iCharID>32) && (iCharID<128))
						{
							char szStr[16];
							sprintf(szStr,"%c character",iCharID);
							oFile.AddComment(szStr);
						}
					}
					else
					{
						// wxCharBuffer cb = _poCI->at(i).m_sChar.ToAscii();
						wxCharBuffer cb = _poCI->at(i).m_sChar.To8BitData();
						if (cb != NULL)
						{
							iCharID = (unsigned char)cb[(uint)0];
						
							if (iCharID>32)
							{
								char szStr[16];
								sprintf(szStr,"%c character",iCharID);
								oFile.AddComment(szStr);
							}
						}
						else
							iCharID = 0;
					}

					oFile.OpenSection("C",j);

						oFile.AddVar("ID",iCharID,-1);
						oFile.AddVar("S",(int)(m_fPointScale*usCharX),0);
						oFile.AddVar("T",(int)(m_fPointScale*usCharY),0);
						oFile.AddVar("W",(int)(m_fPointScale*usCharWidth),0);
						oFile.AddVar("H",(int)(m_fPointScale*usCharHeight),0);
						oFile.AddVar("YOfs",(int)(m_fPointScale*(_poCI->at(i).m_iYOfs+m_iForcedYOffset)),0);

						if (! _poCI->at(i).m_bSpecial)
						{
							oFile.AddVar("LClass",(int)_poCI->at(i).m_uiLeftClass,(int)-1);
							oFile.AddVar("RClass",(int)_poCI->at(i).m_uiRightClass,(int)-1);
						}
						else
						{
							// use the equivalent char kerning info.
							for (uint sc=0;sc<_poCI->size();sc++)
							{
								if (_poCI->at(i).m_sKCEq_UTF8Char == _poCI->at(sc).m_sChar)
								{
									oFile.AddVar("LClass",(int)_poCI->at(sc).m_uiLeftClass,(int)-1);
									oFile.AddVar("RClass",(int)_poCI->at(sc).m_uiRightClass,(int)-1);
									break;
								}
							}
						}

					oFile.CloseSection();

					// new char
					j++;
				}

				// Write the number of characters being written to the file.
				oFile.AddVar("NumChars",(int)j,(int)0);

				// Kerning table
				oFile.OpenSection("KerningTable");
					oFile.AddVar("NumLClasses",poKT->m_uiNumLeftClasses,(int)0);
					oFile.AddVar("NumRClasses",poKT->m_uiNumRightClasses,(int)0);

					uint r,l;
					for (r=0;r<poKT->m_uiNumRightClasses;r++)
						for (l=0;l<poKT->m_uiNumLeftClasses;l++)
						{
							char szStr[16];
							sprintf(szStr,"m%d_%d",l,r);
							oFile.AddVar(szStr,(int)(m_fPointScale*(poKT->iGetKern(l,r)+m_iForcedTracking)),0);
						}

				oFile.CloseSection(); // Kerning table

			oFile.CloseSection(); // OpenType

		oFile.CloseSection(); // Font
		oFile.Write();
	}
	else
		ShowFileErrorMessage(sFilename);
}
//-----------------------------------------------------------------------------
