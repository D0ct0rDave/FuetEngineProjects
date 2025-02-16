// ----------------------------------------------------------------------------
#ifndef DSTexFormatUtilsH
#define DSTexFormatUtilsH
// ----------------------------------------------------------------------------
#include <string>
#include <windows.h>
#include <FreeImage.h>
#include <FreeImagePlus.h>
// ----------------------------------------------------------------------------
const unsigned int MAX_TEX_FORMATS = 8;
extern char* TEX_FORMATS[MAX_TEX_FORMATS];

enum ETexFormat
{
//	TEX_FORMAT_PAL4,
	TEX_FORMAT_PAL16,
	TEX_FORMAT_PAL256,
	TEX_FORMAT_A3I5,
	TEX_FORMAT_A5I3,
	TEX_FORMAT_COMP4x4,
	TEX_FORMAT_DIRECT,
	TEX_FORMAT_NUM_FORMATS,
};

const unsigned int DEFAULT_TEXTURE_FORMAT = TEX_FORMAT_PAL256;
// ----------------------------------------------------------------------------
typedef struct TTextureFormat
{
	std::string m_sTexture;
	std::string m_sFormat;

	bool m_bDither;
	bool m_bColorKey;
	bool m_b4x4Compress;
	bool m_b4x4Lerp;
	bool m_bBuildSprite;
	
	double m_dAlphaFact;
	unsigned int m_uiNumColors;

}TTextureFormat;
// ----------------------------------------------------------------------------
namespace DSTexFormatUtils
{
	void ReadTextureListFile(const std::string& _sFilename);

	TTextureFormat oGuessTextureFormat(const fipImage& _oImg,const fipImage& _oAlpha);
	TTextureFormat oGuessTextureFormat(const fipImage& _oImg);
	TTextureFormat oGuessTextureFormat(const std::string& _sFilename);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
