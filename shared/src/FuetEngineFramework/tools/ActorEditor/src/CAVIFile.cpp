//---------------------------------------------------------------------------
#include <windows.h>
#include <vfw.h>		        // Header file for video for windows
#include "CAVIFile.h"


AVISTREAMINFO 		psi;		        // Pointer to a structure containing stream info
PAVISTREAM 			pavi;		        // Handle to an open stream
PGETFRAME 			pgf;			        // Pointer to a getframe object
BITMAPINFOHEADER 	bmih;	        	// Header information for drawdibdraw decoding
int mpf;				        		// Will hold rough milliseconds per frame

//---------------------------------------------------------------------------
float			CAVIFile::m_fFPS			= 0.0f;

unsigned int 	CAVIFile::m_uiNumFrames 	= 0;
float			CAVIFile::m_fLength			= 0.0f;

unsigned int 	CAVIFile::m_uiCurFrame		= 0;
float			CAVIFile::m_fTime			= 0.0f;
void* 			CAVIFile::m_pImgData		= NULL;
unsigned int	CAVIFile::m_uiWidth			= 0;
unsigned int	CAVIFile::m_uiHeight		= 0;
//-----------------------------------------------------------------------------
void CAVIFile::FlipImage()
{
    unsigned int uiHHeight = m_uiHeight>>1;
    unsigned int uiSize    = m_uiWidth*4;
    void* pLine    = malloc(uiSize);

    for (unsigned int i=0;i<uiHHeight;i++)
    {
        unsigned char* pSLine = (unsigned char*)m_pImgData + i*uiSize;
        unsigned char* pDLine = (unsigned char*)m_pImgData + (m_uiHeight-i-1)*uiSize;

        memcpy(pLine,pSLine,uiSize);
        memcpy(pSLine,pDLine,uiSize);
        memcpy(pDLine,pLine,uiSize);
    }

    free(pLine);
}
//-----------------------------------------------------------------------------
void CAVIFile::SwapRBChannels()
{
    unsigned int uiSize    = m_uiWidth*4;
    unsigned int uiStep = 32 /8;

    for (unsigned int i=0;i<m_uiHeight;i++)
    {
        unsigned char* pLine = (unsigned char*)m_pImgData + i*uiSize;

        for (unsigned int j=0;j<uiSize;j+=uiStep)
        {
            char* pPixel = (char*)pLine + j;
            char aux = pPixel[0];
            pPixel[0] = pPixel[2];
            pPixel[2] = aux;
            pPixel[3] = 0xff;
        }
    }
}
//---------------------------------------------------------------------------
bool CAVIFile::bOpen(const char* _szFilename)
{
	AVIFileInit();		// Opens the AVIFile library

	// Opens the AVI stream
	if (AVIStreamOpenFromFile(&pavi, _szFilename, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
	{
		// An error occurred opening the stream
		MessageBox (HWND_DESKTOP, "Failed To Open The AVI Stream", "Error", MB_OK | MB_ICONEXCLAMATION);
		return(false);
	}

	AVIStreamInfo(pavi, &psi, sizeof(psi));				// Reads information about the stream into psi
	m_uiWidth  = psi.rcFrame.right-psi.rcFrame.left;   	// Width is right side of frame minus left
	m_uiHeight = psi.rcFrame.bottom-psi.rcFrame.top;	// Height is bottom of frame minus top

	// Create the texture where will dump frame image data
	m_pImgData = malloc(m_uiWidth*m_uiHeight*4);

	// Retrieve total number of video frames
	m_uiNumFrames = AVIStreamLength(pavi);										// The last frame of the stream

	// Calculate frames per second
	m_fFPS = (float)m_uiNumFrames / ((float)AVIStreamSampleToTime(pavi,m_uiNumFrames) / 1000.0f);

	// Length in seconds
	m_fLength = (float)m_uiNumFrames / m_fFPS;

	m_uiCurFrame 		= 0;
	m_fTime      		= 0.0f;

	bmih.biSize 		= sizeof (BITMAPINFOHEADER);		// Size of the bitmapinfoheader
	bmih.biPlanes 		= 1;								// Bitplanes
	bmih.biBitCount		= 32;								// Bits format we want (24 bit, 3 bytes)
	bmih.biWidth 		= m_uiWidth;
	bmih.biHeight 		= m_uiHeight;			            // Height we want (256 Pixels)
	bmih.biCompression  = BI_RGB;				            // Requested mode = RGB

	pgf = AVIStreamGetFrameOpen(pavi, &bmih);				// Create the PGETFRAME	using our request mode
	if (pgf==NULL)
	{
		// An error occurred opening the frame
		MessageBox (HWND_DESKTOP, "Failed To Open The AVI Frame", "Error", MB_OK | MB_ICONEXCLAMATION);
		return(false);
	}

	return(true);
}
//---------------------------------------------------------------------------
void CAVIFile::GotoFrame(unsigned int _uiFrame)
{
	LPBITMAPINFOHEADER lpbi;													// Holds the bitmap header information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, _uiFrame);	        	// Grab data from the AVI stream
	if (lpbi == NULL) return;

	void* pdata =(char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);	// Pointer to data returned by AVIStreamGetFrame

	// Convert data to requested bitmap format
	memcpy(m_pImgData,pdata,m_uiWidth*m_uiHeight*4);

	FlipImage();
	SwapRBChannels();

	//
	m_uiCurFrame = _uiFrame;
	m_fTime		 = (float)m_uiCurFrame / m_fFPS;
}
//---------------------------------------------------------------------------
bool CAVIFile::Close()
{
	AVIStreamGetFrameClose(pgf);	// Deallocates the GetFrame resources
	AVIStreamRelease(pavi);			// Release the stream
	AVIFileExit();					// Release the file

	free(m_pImgData);
    return(true);
}
//---------------------------------------------------------------------------
