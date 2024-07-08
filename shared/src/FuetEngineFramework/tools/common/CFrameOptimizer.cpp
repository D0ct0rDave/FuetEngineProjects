// Standard Gen Includes

#include <windows.h>
#include <math.h>
#include <direct.h>
// #include <io.h>
#include <vector>
#include <string>
#include <math.h>

#include "CFrameOptimizer.h"
#include <FuetEngine.h>
// ----------------------------------------------------------------------------
void CFrame::Init(const fipImage& _oImg,unsigned int _uiMask,unsigned int _uiBorderPixels)
{
    m_oRect = oComputeMinRect(_oImg,_uiMask);
    
    m_oRect.m_oIni.x -=_uiBorderPixels;
    m_oRect.m_oIni.y -=_uiBorderPixels;
    m_oRect.m_oEnd.x +=_uiBorderPixels;
    m_oRect.m_oEnd.y +=_uiBorderPixels;

    // Valid Rect(Empty image) ?
    if (m_oRect.m_oIni.x >= m_oRect.m_oEnd.x)
    {
        m_oFrame.setSize(FIT_BITMAP,1,1,32);
        return;
    }

    /*
	m_oRect.m_oIni.x -= 1.0;
    m_oRect.m_oIni.y -= 1.0;
    m_oRect.m_oEnd.x += 1.0;
    m_oRect.m_oEnd.y += 1.0;
	*/

    if (m_oRect.m_oIni.x<0) m_oRect.m_oIni.x = 0;
    if (m_oRect.m_oIni.y<0) m_oRect.m_oIni.y = 0;
	
	m_oFrame = _oImg; // .setSize(FIT_BITMAP,m_oRect.m_oEnd.x+1,m_oRect.m_oEnd.y+1,32);
    _oImg.copySubImage(m_oFrame,m_oRect.m_oIni.x,m_oRect.m_oIni.y,m_oRect.m_oEnd.x+1,m_oRect.m_oEnd.y+1);
}
// ----------------------------------------------------------------------------
void CFrame::Create(unsigned int _uiWidth,unsigned int _uiHeight,unsigned int _uiColor)
{
    m_oRect.m_oIni.x = 0.0;
    m_oRect.m_oIni.y = 0.0;
    m_oRect.m_oEnd.x = _uiWidth;
    m_oRect.m_oEnd.y = _uiHeight;

    m_oFrame.setSize(FIT_BITMAP,_uiWidth,_uiHeight,32);

	RGBQUAD oRGBColor;
	*((unsigned int*)&oRGBColor) = _uiColor;

	unsigned char aux = oRGBColor.rgbBlue;
	oRGBColor.rgbBlue = oRGBColor.rgbRed;
	oRGBColor.rgbRed  = aux;

    for (int j=0;j<_uiHeight;j++)
		for (int i=0;i<_uiWidth;i++)
			m_oFrame.setPixelColor(i,j,&oRGBColor);
}
// ----------------------------------------------------------------------------
static float fColorDistance(const CFEColor& oA,const CFEColor& oB)
{
	float fRD = (float)(oA.r - oB.r);
	float fGD = (float)(oA.g - oB.g);
	float fBD = (float)(oA.b - oB.b);

	return ( sqrtf(fRD*fRD + fGD*fGD + fBD*fBD) );
}
// ----------------------------------------------------------------------------
// oComputeMinRect : Find out min and max non-transparent margins
// x min = find x min not transparent
// x max = find x max not transparent
// y min = find y min not transparent
// y max = find y max not transparent
// ----------------------------------------------------------------------------
CFERect CFrame::oComputeMinRect(const fipImage& _oImg,unsigned int _uiMask)
{
	// fipImage oImg;
	// _oImg.getChannel(oImg,FICC_ALPHA);

	int iIX = _oImg.getWidth();
	int iFX = 0;
	int iIY = _oImg.getHeight();
	int iFY = 0;
    bool bValid = false;
    
	for (int j=0;j<_oImg.getHeight();j++)
		for (int i=0;i<_oImg.getWidth();i++)
		{
		    RGBQUAD oRGBColor;
            _oImg.getPixelColor(i, _oImg.getHeight() - j -1, &oRGBColor);
            
		    if ( (*(unsigned int *)&oRGBColor & _uiMask) != 0 )
		    {
				if (i < iIX) iIX = i;
				if (i > iFX) iFX = i;
				if (j < iIY) iIY = j;
				if (j > iFY) iFY = j;

				bValid = true;
            }
		}

    // Safe frame
    /*
    if (iIX > 0) iIX--;
    if (iIY > 0) iIY--;
    if (iFX < (_oImg.getWidth() -1)) iFX++;
    if (iFY < (_oImg.getHeight()-1)) iFY++;
    */
    
    // Y Origin is at the bottom (!?!)
    if (bValid)
    {
        CFERect oR;
        oR.m_oIni.x = iIX;
        oR.m_oIni.y = iIY;
        oR.m_oEnd.x = iFX;
        oR.m_oEnd.y = iFY;
   	    return(oR);
    }
    else
    {
        CFERect oR;
        oR.m_oIni.x = 0;
        oR.m_oIni.y = 0;
        oR.m_oEnd.x = 0;
        oR.m_oEnd.y = 0;
   	    return(oR);
    }
}
// ----------------------------------------------------------------------------
CFrameOptimizer::CFrameOptimizer()
{

}
// ----------------------------------------------------------------------------
bool CFrameOptimizer::bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS)
{
    if ((_poFrame->getWidth() == 0) || (_poFrame->getHeight() == 0)) return(true);

	for (uint dy=0;dy<=_poOFS->m_oFrameSeq.getHeight()-_poFrame->getHeight();dy++)
		for (uint dx=0;dx<=_poOFS->m_oFrameSeq.getWidth()-_poFrame->getWidth();dx++)
		{
			CFERect oSRect;
			oSRect.m_oIni.x = dx;
			oSRect.m_oIni.y = dy;
			oSRect.m_oEnd.x = dx + _poFrame->getWidth();
			oSRect.m_oEnd.y = dy + _poFrame->getHeight();

			// look if source rect overlaps with in the list
			bool bOverlap = false;
			for (uint r=0;(r<_poOFS->m_oRects.size()) && (! bOverlap);r++)
			{
				CFERect oDRect = _poOFS->m_oRects[r];				
				bOverlap = CFEMath::bOverlap(oSRect,oDRect);

				/*
				bOverlap =  // overlap code check
								
								!(
								// separating axis test

									// over x
								   (oSRect.m_oIni.x > oDRect.m_oEnd.x)
								|| (oSRect.m_oEnd.x < oDRect.m_oIni.x)

									// over y
								|| (oSRect.m_oIni.y > oDRect.m_oEnd.y)
								|| (oSRect.m_oEnd.y < oDRect.m_oIni.y)
								)
						
							||

							// inclusion code check (dst inside the src)
							(
								   (oSRect.m_oIni.x <= oDRect.m_oIni.x) && (oSRect.m_oEnd.x >= oDRect.m_oEnd.x)
								&& (oSRect.m_oIni.y <= oDRect.m_oIni.y) && (oSRect.m_oEnd.y >= oDRect.m_oEnd.y)
						    )
							||
							(
								   (oSRect.m_oIni.x >= oDRect.m_oIni.x) && (oSRect.m_oEnd.x <= oDRect.m_oEnd.x)
								&& (oSRect.m_oIni.y >= oDRect.m_oIni.y) && (oSRect.m_oEnd.y <= oDRect.m_oEnd.y)
						    );
				*/
			}

			if (! bOverlap)
			{
				// we've found a hole: put the frame there.
				_poOFS->m_oFrameSeq.pasteSubImage(*_poFrame,dx,dy);
				/*
				_poOFS->m_oFrameSeq.save("C:\\frameseq.tga");
				_poFrame->save("C:\\frame.tga");
				*/
				_poOFS->m_oRects.push_back(oSRect);		
				return(true);
			}
		}

    return(false);
}
// ----------------------------------------------------------------------------
COptiFrameSequence* CFrameOptimizer::poProcess(const CFrameSequence& _oFS)
{
	if (_oFS.m_oFrames.size() == 0) return(NULL);

    float fMinArea = 0.0;

    float fMinNeededWidth  = -1e6f;
    float fMinNeededHeight = -1e6f;

    uint i;
    for (i=0;i<_oFS.m_oFrames.size();i++)
    {
        float fWidth  = _oFS.m_oFrames[i]->oGetRect().m_oEnd.x - _oFS.m_oFrames[i]->oGetRect().m_oIni.x; 
        float fHeight = _oFS.m_oFrames[i]->oGetRect().m_oEnd.y - _oFS.m_oFrames[i]->oGetRect().m_oIni.y;

        fMinArea += (fWidth * fHeight);

        if (fHeight > fMinNeededHeight) fMinNeededHeight = fHeight; 
        if (fWidth > fMinNeededWidth) fMinNeededWidth  = fWidth;
    } 
    
    // Area calculator for power of 2 textures with ratio of 1x1 or 2x1. Specific for DS.
    const uint MAX_POW2 = 11; 
    uint uiWidth = 1;
    uint uiHeight = 1;

    for (uint h=0;h<MAX_POW2;h++)
    {
        uint uiH = 1 << h;

        if ( ((uiH*uiH) >= fMinArea) && (uiH >= fMinNeededHeight) && (uiH >= fMinNeededWidth))
        {
            uiWidth  = uiH;
            uiHeight = uiH;
            break;
        }

        if ( ((uiH*uiH*2) >= fMinArea) && (uiH >= fMinNeededHeight) && ((2*uiH) >= fMinNeededWidth))
        {
            uiWidth  = uiH*2;
            uiHeight = uiH;    
            break;
        }
    }
    
    printf("--- Dimensions: %d,%d\n",uiWidth,uiHeight);
	
	COptiFrameSequence* poOFS = new COptiFrameSequence;

	poOFS->m_sSequenceName = _oFS.m_sSequenceName;
	poOFS->m_oFrameSeq.setSize(FIT_BITMAP,uiWidth,uiHeight,32);
	ReinitProcess();

		bool bCorrect;
		
		do 
		{
			bCorrect = true;

			for (i=0;((i<_oFS.m_oFrames.size()) && (bCorrect));i++)
			{
				if (bInsertFrame((fipImage*)&_oFS.m_oFrames[i]->oGetImage(),poOFS) == false)
				{
					bCorrect = false;
				}
				else
					printf("Frame: %d ok\n",i);
			}

			if (! bCorrect)
			{
				// Resize and start again. 
				// Instead, we can resize everything without interrupting the process. 
				// This way, we achieve the same results, and has been faster to code...
				printf("Resizing and restarting compactation...\n");
				
				// if (uiWidth > uiHeight) uiHeight*= 2; else uiWidth*=2;
				// Give preference to increase height instead of width
				if (uiHeight > uiWidth) uiWidth*= 2; else uiHeight*=2;

				poOFS->m_oFrameSeq.setSize(FIT_BITMAP,uiWidth,uiHeight,32);
				poOFS->m_oRects.clear();

				ReinitProcess();
			}

		}while(! bCorrect);

    return(poOFS);
}
// ----------------------------------------------------------------------------
