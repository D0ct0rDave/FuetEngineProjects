/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (IMAGELIB == USE_STDIMGLIB)
// ----------------------------------------------------------------------------
#include "CFECImageLib.h"

#include <string.h>
#include <stdlib.h>
#include "../CFECore.h"
// ----------------------------------------------------------------------------
enum {
    TGA_OK,
    TGA_ERROR_FILE_OPEN,
    TGA_ERROR_READING_FILE,
    TGA_ERROR_INDEXED_COLOR,
    TGA_ERROR_MEMORY,
    TGA_ERROR_COMPRESSED_FILE,
};

/** TGA format */
typedef struct sImageTGA {
    int status;
    unsigned char type, pixelDepth;

    /** map width */
    signed short width;

    /** map height */
    signed short height;

    /** raw data */
    unsigned char *imageData;
    int flipped;
} tImageTGA;

// ----------------------------------------------------------------------------
static bool tgaLoadRLEImageData(unsigned char* Buffer, unsigned long bufSize, tImageTGA *psInfo);
void tgaFlipImage( tImageTGA *info );
// ----------------------------------------------------------------------------
// load the image header field from stream
bool tgaLoadHeader(unsigned char* Buffer, unsigned long bufSize, tImageTGA *psInfo)
{
    bool bRet = false;

    do
    {
        size_t step = sizeof(unsigned char) * 2;
        if ((step + sizeof(unsigned char)) > bufSize) return(false);
        memcpy(&psInfo->type, Buffer + step, sizeof(unsigned char));

        step += sizeof(unsigned char) * 2;
        step += sizeof(signed short) * 4;
        if ((step + sizeof(signed short) * 2 + sizeof(unsigned char)) > bufSize) return(false);
        memcpy(&psInfo->width, Buffer + step, sizeof(signed short));
        memcpy(&psInfo->height, Buffer + step + sizeof(signed short), sizeof(signed short));
        memcpy(&psInfo->pixelDepth, Buffer + step + sizeof(signed short) * 2, sizeof(unsigned char));

        step += sizeof(unsigned char);
        step += sizeof(signed short) * 2;
        if ((step + sizeof(unsigned char)) > bufSize) return(false);
        unsigned char cGarbage;
        memcpy(&cGarbage, Buffer + step, sizeof(unsigned char));

        psInfo->flipped = 0;
        if ( cGarbage & 0x20 )
        {
            psInfo->flipped = 1;
        }
        bRet = true;
    } while (0);

    return bRet;
}
// ----------------------------------------------------------------------------
bool tgaLoadImageData(unsigned char *Buffer, unsigned long bufSize, tImageTGA *psInfo)
{
    bool bRet = false;

    do
    {
        int mode,total,i;
        unsigned char aux;
        size_t step = (sizeof(unsigned char) + sizeof(signed short)) * 6;

        // mode equal the number of components for each pixel
        mode = psInfo->pixelDepth / 8;
        // total is the number of unsigned chars we'll have to read
        total = psInfo->height * psInfo->width * mode;

        size_t dataSize = sizeof(unsigned char) * total;
        if ((step + dataSize) > bufSize) return(false);
        memcpy(psInfo->imageData, Buffer + step, dataSize);

        // mode=3 or 4 implies that the image is RGB(A). However TGA
        // stores it as BGR(A) so we'll have to swap R and B.
        if (mode >= 3)
        {
            for (i=0; i < total; i+= mode)
            {
                aux = psInfo->imageData[i];
                psInfo->imageData[i] = psInfo->imageData[i+2];
                psInfo->imageData[i+2] = aux;
            }
        }

        bRet = true;
    } while (0);

    return bRet;
}
// ----------------------------------------------------------------------------
static bool tgaLoadRLEImageData(unsigned char* Buffer, unsigned long bufSize, tImageTGA *psInfo)
{
    unsigned int mode,total,i, index = 0;
    unsigned char aux[4], runlength = 0;
    unsigned int skip = 0, flag = 0;
    size_t step = (sizeof(unsigned char) + sizeof(signed short)) * 6;

    // mode equal the number of components for each pixel
    mode = psInfo->pixelDepth / 8;
    // total is the number of unsigned chars we'll have to read
    total = psInfo->height * psInfo->width;

    for( i = 0; i < total; i++ )
    {
        // if we have a run length pending, run it
        if ( runlength != 0 )
        {
            // we do, update the run length count
            runlength--;
            skip = (flag != 0);
        }
        else
        {
            // otherwise, read in the run length token
            if ((step + sizeof(unsigned char)) > bufSize) return(false);
            memcpy(&runlength, Buffer + step, sizeof(unsigned char));
            step += sizeof(unsigned char);

            // see if it's a RLE encoded sequence
            flag = runlength & 0x80;
            if ( flag )
            {
                runlength -= 128;
            }
            skip = 0;
        }

        // do we need to skip reading this pixel?
        if ( !skip )
        {
            // no, read in the pixel data
            if ((step + sizeof(unsigned char) * mode) > bufSize) return(false);

            memcpy(aux, Buffer + step, sizeof(unsigned char) * mode);
            step += sizeof(unsigned char) * mode;

            // mode=3 or 4 implies that the image is RGB(A). However TGA
            // stores it as BGR(A) so we'll have to swap R and B.
            if ( mode >= 3 )
            {
                unsigned char tmp;

                tmp = aux[0];
                aux[0] = aux[2];
                aux[2] = tmp;
            }
        }

        // add the pixel to our image
        memcpy(&psInfo->imageData[index], aux, mode);
        index += mode;
    }

    return true;
}
// ----------------------------------------------------------------------------
void tgaFlipImage( tImageTGA *psInfo )
{
    // mode equal the number of components for each pixel
    int mode = psInfo->pixelDepth / 8;
    int rowbytes = psInfo->width*mode;
    unsigned char *row = (unsigned char *)CFECore::Mem::pAlloc(rowbytes);
    int y;

    if (row == NULL) return;

    for( y = 0; y < (psInfo->height/2); y++ )
    {
        memcpy(row, &psInfo->imageData[y*rowbytes],rowbytes);
        memcpy(&psInfo->imageData[y*rowbytes], &psInfo->imageData[(psInfo->height-(y+1))*rowbytes], rowbytes);
        memcpy(&psInfo->imageData[(psInfo->height-(y+1))*rowbytes], row, rowbytes);
    }

    CFECore::Mem::Free(row);
    psInfo->flipped = 0;
}
// ----------------------------------------------------------------------------
// this is the function to call when we want to load an image
tImageTGA * tgaLoad(const CFEString& _sFilename)
{
    int mode,total;
    tImageTGA *info = NULL;

    uint nSize = 0;

    CFECore::Log::Print("tgaLoad");

    unsigned char* pBuffer = CFECore::File::pReadFile(_sFilename,&nSize);
    if (pBuffer == NULL) return(NULL);

    do
    {
        info = (tImageTGA *)CFECore::Mem::pAlloc(sizeof(tImageTGA));

        // get the file header info
        if (! tgaLoadHeader(pBuffer, nSize, info))
        {
            info->status = TGA_ERROR_MEMORY;
            break;
        }

        // check if the image is color indexed
        if (info->type == 1)
        {
            info->status = TGA_ERROR_INDEXED_COLOR;
            break;
        }

        // check for other types (compressed images)
        if ((info->type != 2) && (info->type !=3) && (info->type !=10) )
        {
            info->status = TGA_ERROR_COMPRESSED_FILE;
            break;
        }

        // mode equals the number of image components
        mode = info->pixelDepth / 8;
        // total is the number of unsigned chars to read
        total = info->height * info->width * mode;
        // allocate memory for image pixels
        info->imageData = (unsigned char *)CFECore::Mem::pAlloc(sizeof(unsigned char) * total);

        // check to make sure we have the memory required
        if (info->imageData == NULL)
        {
            info->status = TGA_ERROR_MEMORY;
            break;
        }

        bool bLoadImage = false;
        // finally load the image pixels
        if ( info->type == 10 )
        {
            bLoadImage = tgaLoadRLEImageData(pBuffer, nSize, info);
        }
        else
        {
            bLoadImage = tgaLoadImageData(pBuffer, nSize, info);
        }

        // check for errors when reading the pixels
        if (! bLoadImage)
        {
            info->status = TGA_ERROR_READING_FILE;
            break;
        }
        info->status = TGA_OK;

        if (! info->flipped )
        {
            tgaFlipImage( info );
            if ( info->flipped )
            {
                info->status = TGA_ERROR_MEMORY;
            }
        }
    } while(0);

    CFECore::Mem::Free(pBuffer);

    return info;
}
// ----------------------------------------------------------------------------
// releases the memory used for the image
void tgaDestroy(tImageTGA *psInfo)
{
    if (psInfo != NULL)
    {
        if (psInfo->imageData != NULL)
        {
            CFECore::Mem::Free(psInfo->imageData);
        }

        CFECore::Mem::Free((FEPointer)psInfo);
    }
}
// ----------------------------------------------------------------------------
CFECImageInfo* ImageLib::poLoadImage(char* _szFilename)
{
    CFEString sFilename = CFEString(_szFilename) + CFEString(".tga");
    CFECore::Log::Print("poLoadImage");

    tImageTGA* info = tgaLoad(sFilename);

    if (info->status == TGA_OK)
    {
        CFECImageInfo* poII = new CFECImageInfo;

        poII->m_hHandler = (FEHandler)info;
        poII->m_pData    = info->imageData;
        poII->m_uiTX     = info->width;
        poII->m_uiTY     = info->height;
        poII->m_uiFixedTX= info->width;
        poII->m_uiFixedTY= info->height;
        poII->m_uiPitch  = info->width * info->pixelDepth;

        switch (info->pixelDepth)
        {
            case 8:
                poII->m_eFormat = PF_GRAY;
            break;

            case 16:
                poII->m_eFormat = PF_RGBA16;
            break;

            case 24:
                poII->m_eFormat = PF_RGB24;
            break;

            case 32:
                poII->m_eFormat = PF_RGBA32;
            break;
        }

        return(poII);
    }
    else
    {
        tgaDestroy(info);
        return(NULL);
    }
}


void ImageLib::MakePowerOf2(CFECImageInfo* _poImageInfo)
{

}

void ImageLib::FreeImage(CFECImageInfo* _poImageInfo)
{
    tgaDestroy((tImageTGA *)_poImageInfo->m_hHandler);
    delete _poImageInfo;
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
