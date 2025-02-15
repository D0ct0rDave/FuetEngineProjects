// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (IMAGELIB == USE_FREEIMAGE)
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <FreeImage.h>

#include "../CFESystem.h"
#include "FEEnums.h"            // for pixel format
#include "FEBasicTypes.h"

#include "CFESImageLib.h"
//-----------------------------------------------------------------------------
static bool gbInitialized = false;
//-----------------------------------------------------------------------------
FIBITMAP* GenericLoaderINT(const char* lpszPathName, int flag)
{
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    // check the file signature and deduce its format
    // (the second argument is currently not used by FreeImage)
    fif = FreeImage_GetFileType(lpszPathName, 0);
    if(fif == FIF_UNKNOWN) {
        // no signature ?
        // try to guess the file format from the file extension
        fif = FreeImage_GetFIFFromFilename(lpszPathName);
    }

    // check that the plugin has reading capabilities ...
    if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
        // ok, let's load the file
        FIBITMAP *dib = FreeImage_Load(fif, lpszPathName, flag);
        // unless a bad file format, we are done !
        return dib;
    }
    return NULL;
}
//-----------------------------------------------------------------------------
FIBITMAP* GenericLoader(const char* lpszPathName, int flag)
{
	// WARNING: Not very solid!!! "./lala.tga" will fail
    const char* szStr = strstr(lpszPathName,".");
	if (szStr != NULL)
	{
		if (strstr(lpszPathName,"./") != NULL)
			szStr = NULL;
	}

    // Has the filename any extension?
    if (szStr == NULL)
    {
        char* szDefaultExtensions[] = {(char*)"png",(char*)"tga",(char*)"jpg",(char*)"bmp",NULL};

        uint uiExt = 0;
        while (szDefaultExtensions[uiExt] != NULL)
        {
            // Filename has no extension, so try appending some standard extensions like TGA, JPG, PNG
            FIBITMAP* poBmp = NULL;
            char szStr[1024];

            sprintf_s(szStr,1024,"%s.%s",lpszPathName,szDefaultExtensions[uiExt]);
            poBmp = GenericLoaderINT(szStr,flag);

            if (poBmp != NULL)
                // Ok we've found it!
                return(poBmp);

            // Nop, try next extension
            uiExt ++;
        }
    }
    else
        // Filename has extension so let's load normaly
        return( GenericLoaderINT(lpszPathName,flag) );

    return(NULL);
}
//-----------------------------------------------------------------------------
void FlipImage(FIBITMAP* dib)
{
    uint uiHeight  = FreeImage_GetHeight(dib);
    uint uiHHeight = uiHeight>>1;
    uint uiSize    = FreeImage_GetPitch(dib);
    void* pLine    = malloc(uiSize);    // DMC WARNING: malloc usado, deber�a usarse el memory management de system ???

    for (uint i=0;i<uiHHeight;i++)
    {
        void* pSLine = FreeImage_GetScanLine(dib,i);
        void* pDLine = FreeImage_GetScanLine(dib,uiHeight-i-1);

        memcpy(pLine,pSLine,uiSize);
        memcpy(pSLine,pDLine,uiSize);
        memcpy(pDLine,pLine,uiSize);
    }

    free(pLine);    // DMC WARNING: free usado, deber�a usarse el memory management de system ???
}
//-----------------------------------------------------------------------------
void SwapRBChannels(FIBITMAP* dib)
{
    uint uiBPP     = FreeImage_GetBPP(dib);
    if ((uiBPP!=24) && (uiBPP!=32)) return;

    uint uiHeight  = FreeImage_GetHeight(dib);
    uint uiHHeight = uiHeight>>1;
    uint uiSize    = FreeImage_GetPitch(dib);
    uint uiStep = uiBPP /8;

    for (uint i=0;i<uiHeight;i++)
    {
        void* pLine = FreeImage_GetScanLine(dib,i);

        for (uint j=0;j<uiSize;j+=uiStep)
        {
            char* pPixel = (char*)pLine + j;
            char aux = pPixel[0];
            pPixel[0] = pPixel[2];
            pPixel[2] = aux;
        }
    }
}
//-----------------------------------------------------------------------------
CFESImageInfo* ImageLib::poLoadImage(char* _szFilename)
{
    if (! gbInitialized)
    {
        FreeImage_Initialise();
        gbInitialized = true;
    }

    if (_szFilename == NULL) return(NULL);

    // Try loading the image
    FIBITMAP *dib = GenericLoader(_szFilename, 0);

    // Try with alternate path
    if (dib == NULL) return(NULL);

    FlipImage(dib);
    SwapRBChannels(dib);

    CFESImageInfo* poII = new CFESImageInfo;
    poII->m_hHandler = (FEHandler)dib;

    poII->m_uiTX    = FreeImage_GetWidth(dib);
    poII->m_uiTY    = FreeImage_GetHeight(dib);
    poII->m_uiFixedTX = poII->m_uiTX;
    poII->m_uiFixedTY = poII->m_uiTY;
    poII->m_pData   = FreeImage_GetBits(dib);
    poII->m_uiPitch = FreeImage_GetPitch(dib);

    switch(FreeImage_GetColorType(dib))
    {
        case FIC_MINISWHITE:
        case FIC_MINISBLACK:
        case FIC_CMYK:
                // ???
        break;

        case FIC_RGB:
        {
            BITMAPINFOHEADER *bih = FreeImage_GetInfoHeader(dib);

            if (bih->biBitCount == 32)
                poII->m_eFormat = PF_RGBA32;
            else
                poII->m_eFormat = PF_RGB24;
        }
        break;

        case FIC_RGBALPHA:
        poII->m_eFormat = PF_RGBA32;
        break;
    }

    return( poII );
}
// ----------------------------------------------------------------------------
uint uiGreaterPowerOf2(uint _uiValue)
{
	uint uiLog = 0;
	while ((uint)(1 << uiLog) < _uiValue)
        uiLog++;		 

    return(1<<uiLog);
}
// ----------------------------------------------------------------------------
void ImageLib::MakePowerOf2(CFESImageInfo* _poImageInfo)
{
    uint uiNewTX = uiGreaterPowerOf2(_poImageInfo->m_uiTX);
    uint uiNewTY = uiGreaterPowerOf2(_poImageInfo->m_uiTY);
    
    if ((uiNewTX == _poImageInfo->m_uiTX) && (uiNewTY == _poImageInfo->m_uiTY)) return;
    FIBITMAP *dib = FreeImage_Rescale((FIBITMAP *)_poImageInfo->m_hHandler,uiNewTX,uiNewTY,FILTER_BILINEAR);
    FreeImage_Unload((FIBITMAP *)_poImageInfo->m_hHandler);

    _poImageInfo->m_hHandler  = (FEHandler)dib;
    _poImageInfo->m_uiFixedTX = FreeImage_GetWidth(dib);
    _poImageInfo->m_uiFixedTY = FreeImage_GetHeight(dib);
    _poImageInfo->m_pData     = FreeImage_GetBits(dib);
    _poImageInfo->m_uiPitch   = FreeImage_GetPitch(dib);
}
// ----------------------------------------------------------------------------
void ImageLib::FreeImage(CFESImageInfo* _poImageInfo)
{
    if (_poImageInfo == NULL) return;

    if (_poImageInfo->m_hHandler!= NULL)
    {
        FreeImage_Unload((FIBITMAP *)_poImageInfo->m_hHandler);
    }
    
    delete _poImageInfo;
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
