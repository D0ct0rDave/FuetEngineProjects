// ----------------------------------------------------------------------------
#include "DSTexFormatUtils.h"
#include "CColorList.h"
#include "EdUtils.h"
//---------------------------------------------------------------------------
static std::vector <TTextureFormat>  m_oTexList;
//---------------------------------------------------------------------------
char* TEX_FORMATS[MAX_TEX_FORMATS] = 
{
//	"palette4",			// 0
	"palette16",		// 1
	"palette256",		// 2
	"a3i5",				// 3
	"a5i3",				// 4
	"comp4x4",			// 6
	"direct",			// 5
};
// ----------------------------------------------------------------------------
// Returns true if _sB is contained in _sA
// ----------------------------------------------------------------------------
bool bFilenameMatches(const std::string& _sA,const std::string& _sB)
{
	size_t iPos = _sA.rfind(_sB);
	if (iPos == std::string::npos)
		return(false);
	else
		return( iPos == (_sA.size() - _sB.size()) );
}
// ----------------------------------------------------------------------------
void DSTexFormatUtils::ReadTextureListFile(const std::string& _sFilename)
{	

	FILE* fd = fopen(_sFilename.c_str(),"rt");
	if (fd == NULL) return;

	m_oTexList.clear();

	while (! feof(fd))
	{

		//
		char szString[1024];	
		if (fgets(szString,1024,fd))
		{
			const unsigned int MAX_OPTIONS = 7;
			char szOptions[MAX_OPTIONS][1024];
			for (unsigned int  i=0;i<MAX_OPTIONS;i++) szOptions[i][0] = 0;

			bool b4x4Compress   = false;
			bool b4x4Lerp		= false;
			bool bDither        = false;
			bool bColorKey		= false;
			bool bBuildSprite	= false;

			if ((szString[0] == szString[1]) && (szString[0]=='/'))
			{
				continue;
			}
			else
			{
				unsigned int uiReadOpts = sscanf(szString,"%s %s %s %s %s %s %s",szOptions[0],szOptions[1],szOptions[2],szOptions[3],szOptions[4],szOptions[5]);
				if (uiReadOpts > 2)
				{
					for (int i=2;i<uiReadOpts;i++)
					{
						if (! stricmp("-4x4compress",szOptions[i]))
							b4x4Compress = true;

				   else if (! stricmp("-4x4lerp",szOptions[i]))
							b4x4Lerp = true;

					else if (! stricmp("-dither",szOptions[i]))
							bDither = true;

					else if (! stricmp("-colorkey",szOptions[i]))
							bColorKey = true;

					else if (! stricmp("-buildsprite",szOptions[i]))
							bBuildSprite = true;
					}
				}
			}

			TTextureFormat oTF;
						
			oTF.m_sTexture  = std::string(szOptions[0]);
			oTF.m_sFormat   = std::string(szOptions[1]);
			oTF.m_bColorKey = bColorKey;
			oTF.m_b4x4Lerp  = b4x4Lerp;
			oTF.m_b4x4Compress = b4x4Compress;
			oTF.m_bDither   = bDither;
			oTF.m_bBuildSprite = bBuildSprite;

			m_oTexList.push_back(oTF);

		} // end if fgets

	} // End of while(!feof)

	fclose(fd);
}
// ----------------------------------------------------------------------------
float fColorDistance(const RGBQUAD& oA,const RGBQUAD& oB)
{
/*
	RGBQUAD oRGBColor;						
				oImg.getPixelColor(i, j, &oRGBColor);
				CFEColor oColor((FEReal)oRGBColor.rgbRed/255.0,
								(FEReal)oRGBColor.rgbGreen/255.0,
								(FEReal)oRGBColor.rgbBlue/255.0,
								1.0);
								
								
	float fRD = (float)(oA.r - oB.r);
	float fGD = (float)(oA.g - oB.g);
	float fBD = (float)(oA.b - oB.b);

	return ( sqrtf(fRD*fRD + fGD*fGD + fBD*fBD) );
*/
	
	return(0);
}
// ----------------------------------------------------------------------------
double dGetAverage(const fipImage& oImg)
{
/*
	Average = (sumatorio de pixel) / total_pixels

	sumatorio de pixels: 

		inicializar acumulador(r,g,b,a) = (0,0,0,0)

		Para todos los pixles:
			convertir color en color_flotante(r,g,b,a)
			acumulador = acumulador + color_flotante
		
	Average = acumulador / total_pixels
*/
	return(0);
}
// ----------------------------------------------------------------------------
double dGetVarianza(const fipImage& oImg)
{
	double dAverage = dGetAverage(oImg);

/*
	Varianza = (sumatorio de (pixel - dAverage)^2) / total_pixels
	
	inicializar acumulador(r,g,b,a) = (0,0,0,0)

		Para todos los pixles:
			convertir color en color_flotante(r,g,b,a)
			acumulador = acumulador + (color_flotante - dAverage)^2
		
		Varianza = acumulador / total_pixels

	for (int j=0;j<oImg.getHeight();j++)
			for (int i=0;i<oImg.getWidth();i++)
			{
				RGBQUAD oRGBColor;						
				oImg.getPixelColor(i, j, &oRGBColor);
			}	
*/	
	return(0);
}

// ----------------------------------------------------------------------------
// Pre: Num Image colors are <= 256
// ----------------------------------------------------------------------------
// Possible return formats are a3i5, a5i3, palette, and direct
// ----------------------------------------------------------------------------
TTextureFormat DSTexFormatUtils::oGuessTextureFormat(const fipImage& _oImg,const fipImage& _oAlpha)
{
	// Count colors.
	CColorList oColorList;
	bool bFinished = false;
	for (int j=0;((j<_oImg.getHeight()) && (!bFinished));j++)
		for (int i=0;((i<_oImg.getWidth()) && (!bFinished));i++)
		{
			RGBQUAD oRGBColor;
			_oImg.getPixelColor(i, j, &oRGBColor);

			unsigned int uiIdx = oColorList.uiAdd( (unsigned int)oRGBColor.rgbRed 
												| ((unsigned int)oRGBColor.rgbGreen << 8) 
												| ((unsigned int)oRGBColor.rgbBlue << 16));

			// If there are more than 256 colors... it's not worth keep counting...
			bFinished = (uiIdx == (oColorList.uiGetMaxColors()-1));
		}

	// Get Histograms.
	#if 0
	DWORD dwRed[256];
	DWORD dwGreen[256];
	DWORD dwBlue[256];
	unsigned char ucRedModa = 0;			// most repeated red value.
	unsigned char ucGreenModa = 0;			// most repeated green value.
	unsigned char ucBlueModa = 0;			// most repeated blue value.
	for (int i=1;i<256;i++)
	{
		if (dwRed[i] > dwRed[ucRedModa]) ucRedModa = i;
		if (dwGreen[i] > dwGreen[ucGreenModa]) ucGreenModa = i;
		if (dwBlue[i] > dwBlue[ucBlueModa]) ucBlueModa = i;
	}

	oImg.getHistogram(dwRed, FICC_RED);
	oImg.getHistogram(dwGreen, FICC_GREEN);
	oImg.getHistogram(dwBlue, FICC_BLUE);
	#endif


	DWORD dwAlpha[256];
	_oAlpha.getHistogram(dwAlpha, FICC_RED);
	unsigned char ucAlphaModa = 0;			// most repeated alpha value.
	for (int i=1;i<256;i++)
	{
		if (dwAlpha[i] > dwAlpha[ucAlphaModa])
			ucAlphaModa = i;
	}

	unsigned int uiTotalPixels = _oAlpha.getHeight() * _oAlpha.getWidth();
	double dPercent = (double)dwAlpha[ucAlphaModa] / (double)uiTotalPixels;
	unsigned int uiNumColors = oColorList.uiSize();

	TTextureFormat oTF;
	oTF.m_uiNumColors  = uiNumColors;
	oTF.m_dAlphaFact   = dPercent;
	oTF.m_b4x4Compress = false;
	oTF.m_b4x4Lerp     = false;
	oTF.m_bDither      = false;
	oTF.m_bColorKey    = false;
	oTF.m_bBuildSprite = false;

	// all alpha pixels are the same
	if (dPercent == 1.0)
	{
		// black or white alpha channel
		if ((ucAlphaModa == 0) || (ucAlphaModa == 255))
		{
			/*
			// Avoid palette4 format!
			if (uiNumColors<=4)			
				oTF.m_sFormat =TEX_FORMATS[TEX_FORMAT_PAL4];
			else*/ 

			     if (uiNumColors<=16)
				oTF.m_sFormat =TEX_FORMATS[TEX_FORMAT_PAL16];

			else
			{
				if ( ((_oImg.getWidth() & 0x03) ==0) && ((_oImg.getHeight() & 0x03) ==0) )
					oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_COMP4x4];
				else
					oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_PAL256];
			}				
		}
		else
			oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_A3I5];
	}
	else
	{
		if (uiNumColors <= 8)
		{
			oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_A5I3];
		}
		else if (uiNumColors <= 32)
		{
			oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_A3I5];
		}
		else 
		{
			oTF.m_bColorKey = true;

			if ( ((_oImg.getWidth() & 0x03) ==0) && ((_oImg.getHeight() & 0x03) ==0) )
				oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_COMP4x4];
			else
				oTF.m_sFormat = TEX_FORMATS[TEX_FORMAT_PAL256];
		}
	}

	return(oTF);
}
// ----------------------------------------------------------------------------
TTextureFormat DSTexFormatUtils::oGuessTextureFormat(const fipImage& _oImg)
{
	// build alpha image
	fipImage oAImg(_oImg);
	oAImg.getChannel(oAImg,FICC_ALPHA);

	return( DSTexFormatUtils::oGuessTextureFormat(_oImg,oAImg) );
}
// ----------------------------------------------------------------------------
TTextureFormat DSTexFormatUtils::oGuessTextureFormat(const std::string& _sFilename)
{
	// ---------------------
	// STEP 1: Look if the texture is in the database.
	// ---------------------
	std::string sFilenameOnly = sNormalizePath( sRemoveExtension(_sFilename) );
	for(unsigned int i=0;i<m_oTexList.size();i++)
	{
		if ( bFilenameMatches(sFilenameOnly, m_oTexList[i].m_sTexture) )
		{
			// printf("found file %s with format %s\n",m_oTexList[i].m_sTexture.c_str(),m_oTexList[i].m_sFormat.c_str());
			return( m_oTexList[i] );
		}
	}
	
	
	// ---------------------
	// STEP 2: Analyse the image
	// ---------------------
	TTextureFormat oTF;
	
	fipImage oImg;	
	if (! oImg.load(_sFilename.c_str()))
	{
		oTF.m_b4x4Compress = false;
		oTF.m_b4x4Lerp     = false;
		oTF.m_bDither      = false;
		oTF.m_bBuildSprite = false;
		oTF.m_bColorKey    = false;
		oTF.m_sFormat      = TEX_FORMATS[DEFAULT_TEXTURE_FORMAT];
		return(oTF);
	}

	oImg.convertTo32Bits();
	oTF = oGuessTextureFormat(oImg);

	// printf("File %s not found in database guessed format %s\n",_sFilename.c_str(),oTF.m_sFormat.c_str());
	return(oTF);
}
// ----------------------------------------------------------------------------
