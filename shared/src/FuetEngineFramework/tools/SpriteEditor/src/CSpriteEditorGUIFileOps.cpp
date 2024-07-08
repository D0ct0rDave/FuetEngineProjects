//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CSpriteEditorGUIFileOps.h"
// #include "CSpriteEditorGUIFuncs.h"
#include "CAppGlobals.h"
#include "CLibConfigFileWriter.h"
#include <vector>
#include <freeimage.h>
#include "edutils.h"
//---------------------------------------------------------------------------
const FEReal DEF_ANIM_TIME = 1.0f;

const FEReal DEF_POS_X = 0.0f;
const FEReal DEF_POS_Y = 0.0f;
const FEReal DEF_PIVOT_X = 0.5f;
const FEReal DEF_PIVOT_Y = 0.5f;
const FEReal DEF_SCALE_X = 1.0f;
const FEReal DEF_SCALE_Y = 1.0f;
const FEReal DEF_ANGLE = 0.0f;
const FEReal DEF_DEPTH = 0.0f;

const FEReal DEF_WIDTH = 1.0f;
const FEReal DEF_HEIGHT = 1.0f;

const FEReal DEF_IU = 0.0f;
const FEReal DEF_IV = 0.0f;
const FEReal DEF_FU = 1.0f;
const FEReal DEF_FV = 1.0f;

const FEReal DEF_TIME  = 0.0f;
const uint DEF_KEYFRAMES = 1;
const uint DEF_NUMACTIONS = 0;
const bool DEF_VISIBILITY = true;

const char* DEF_LERP = "LINEAR";
const char* DEF_LOOP = "ONESHOT";
const char* DEF_WRAP = "CLAMP";
const char* DEF_BLENDMODE = "ALPHA";
const char* DEF_FILTER = "LINEAR";

const char* DEF_FELERP	= "LINEAR";
const char* DEF_FEWRAP	= "LOOP";
const FEBool DEF_XYUVSCALE   = false;
const FEBool DEF_WORLDCOORDS = false;
const int	DEF_ACTION = -1;
//---------------------------------------------------------------------------
bool bLoadSprite(const CFEString& _sFilename)
{
//     if ( bLoadAnimation(CFEString(globals.m_sInputFilename.char_str())) )
    globals.m_poSprite = CFESpriteMgr::I()->poLoad(_sFilename);

    if (globals.m_poSprite != NULL)
    {
        globals.m_hCurMaterial = globals.m_poSprite->poGetAction(0)->m_oSeq[0].m_hMaterial;        
        globals.m_bRelaunchImage = true;
        globals.m_iSelectedAction= 0;
        globals.m_iSelectedFrame = 0;

        if (globals.m_hCurSprInst != NULL)
            CFESpriteInstMgr::I()->ReleaseInstance(globals.m_hCurSprInst);

        globals.m_hCurSprInst = CFESpriteInstMgr::I()->hGetInstance( globals.m_poSprite );
        CFESpriteInstMgr::I()->Enable(globals.m_hCurSprInst);
        CFESpriteInstMgr::I()->SetPos(globals.m_hCurSprInst,CFEVect2::ZERO());
    }
    
    return(globals.m_poSprite != NULL);
}
// ------------------------------------------------------------------------------------------------------
// saving 
// ------------------------------------------------------------------------------------------------------
CFEString sGetLoopMode(ESFSPlayMode _ePlayMode)
{
    switch(_ePlayMode)
    {
        case SFSPM_ONESHOT:
	    return("ONESHOT");
	    break;
    	
	    case SFSPM_PINGPONGSTOP:
	    return("PINGPONGSTOP");
	    break;

	    case SFSPM_LOOP:
	    return("LOOP");
	    break;

	    case SFSPM_PINGPONG:
	    return("PINGPONG");
	    break;
	}

	return(DEF_LOOP);
}
// ------------------------------------------------------------------------------------------------------
CFEString sGetWrapMode(int _iWrapMode)
{
    switch(_iWrapMode)
    {
        case 0:
	    return("CLAMP");
	    break;
    	
	    case 1:
	    return("REPEAT");
	    break;

	    case 2:
	    return("MIRROR");
	    break;
	}

	return(DEF_WRAP);
}
// ------------------------------------------------------------------------------------------------------
CFEString sGetFilter(int _iFilter)
{
    switch(_iFilter)
    {
        case 0:
	    return("NEAREST");
	    break;
    	
	    case 1:
	    return("LINEAR");
	    break;
	}

	return(DEF_FILTER);
}
// ------------------------------------------------------------------------------------------------------
CFEString sGetBlendMode(EFEBlendMode _eBlendMode)
{
    switch(_eBlendMode)
    {
        case BM_COPY:
	    return("COPY");
	    break;
        case BM_ALPHA:
	    return("ALPHA");
	    break;
        case BM_ALPHAADD:
	    return("ALPHAADD");
	    break;
        case BM_ADD:
	    return("ADD");
	    break;
        case BM_MULT:
	    return("MULT");
	    break;
        case BM_FOG:
	    return("FOG");
	    break;
        case BM_FOGADD:
	    return("FOGADD");
	    break;
        case BM_MAGICMARKER:
	    return("MAGICMARKER");
	    break;
        case BM_LIGHTMARKER:
	    return("LIGHTMARKER");
	    break;
        case BM_LIGHTSABER:
	    return("LIGHTSABER");
	    break;
        case BM_REVEAL:
	    return("REVEAL");
	    break;
        case BM_LUMISHADE_REVEAL:
	    return("LUMISHADE_REVEAL");
	    break;	    
	}

	return(DEF_BLENDMODE);
}
// ------------------------------------------------------------------------------------------------------
bool bSaveSprite(CFESprite* _poSprite,const CFEString& _sFilename)
{
    CFEString sWorkingDir = CFEStringUtils::sGetPath( _sFilename );
	
	char szWorkDirFullFilename[1024];
	GetFullPathName(sWorkingDir.szString(),1024,szWorkDirFullFilename, NULL);

	// make backup of file
	if (bFileExists(_sFilename))	
		MakeBackupOfFile( wxString(_sFilename.szString()) );

	CLibConfigFileWriter oFW;
    if (! oFW.bCreate(std::string( _sFilename.szString()) ))
    {
		return(false);
    }
    
    CFEString sSpriteName = CFEStringUtils::sRemoveExtension(CFEStringUtils::sGetFilename(_sFilename));

    oFW.OpenSection("Sprite");
        
        oFW.AddVar("Name",sSpriteName.szString(),"");
        oFW.AddVar("BlendMode","ALPHA","ALPHA");
        oFW.AddVar("NumActions",(int)_poSprite->m_oActions.size(),(int)DEF_NUMACTIONS);
    
        for (uint uiAction=0;uiAction<_poSprite->m_oActions.size();uiAction++)
        {
            CFESpriteAction* poAction = &_poSprite->m_oActions[uiAction];

            oFW.OpenSection("Action",uiAction);
                
                oFW.AddVar("Name",poAction->sGetName().szString(),"");
                oFW.AddVar("BlendMode",sGetBlendMode(poAction->m_eBlendMode).szString(),DEF_BLENDMODE);
                oFW.AddVar("PlayMode",sGetLoopMode(poAction->m_ePlayMode).szString(),DEF_LOOP);
                if (poAction->m_rRandStartTime>0)
                    oFW.AddVar("RandomStartTime",(float)poAction->m_rRandStartTime,0,2);

                oFW.OpenSection("Frames");
                    oFW.AddVar("Type","Complete","");
                    oFW.OpenSection("Complete");

                        oFW.AddVar("NumFrames",(int)poAction->m_oSeq.size(),(int)DEF_KEYFRAMES);

                        for (uint uiFrame=0;uiFrame<poAction->m_oSeq.size();uiFrame++)
                        {
                            CFESpriteFrame* poFrame = &poAction->m_oSeq[uiFrame];
                            
                            oFW.OpenSection("Frame",uiFrame);

								CFEString sMaterial = CFEMaterialMgr::I()->sGetResourceName((FEHandler*)poFrame->m_hMaterial);

								char szMaterialFullFilename[1024];
								GetFullPathName( sMaterial.szString(),1024,szMaterialFullFilename, NULL);

                                sMaterial = sRelativizePath(CFEString(szMaterialFullFilename),CFEString(szWorkDirFullFilename));
                                oFW.AddVar("Material",sMaterial.szString(),"");

                                int iWrapMode;
                                CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.SWrapMode",(FEPointer)&iWrapMode);
								oFW.AddVar("SWrapMode",sGetWrapMode(iWrapMode).szString(),DEF_WRAP);

                                CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.TWrapMode",(FEPointer)&iWrapMode);
								oFW.AddVar("TWrapMode",sGetWrapMode(iWrapMode).szString(),DEF_WRAP);

                                int iFilter;
                                CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Filter",(FEPointer)&iFilter);
								oFW.AddVar("Filter",sGetFilter(iFilter).szString(),DEF_FILTER);

                                oFW.AddVar("DelayTime",poFrame->m_rDelay,DEF_TIME);
                                oFW.AddVar("BlendTime",poFrame->m_rBlendTime,DEF_TIME);

                                oFW.OpenSection("Pivot");
                                    oFW.AddVar("x",poFrame->m_oPivot.x,DEF_PIVOT_X);
                                    oFW.AddVar("y",poFrame->m_oPivot.y,DEF_PIVOT_Y);
                                oFW.CloseSection(); // Pivot

                                oFW.OpenSection("Size");
                                    oFW.AddVar("Width",poFrame->m_oSize.x,DEF_WIDTH,1);
                                    oFW.AddVar("Height",poFrame->m_oSize.y,DEF_HEIGHT,1);
                                oFW.CloseSection(); // Size

                                oFW.OpenSection("TexCoords");
                                    oFW.AddVar("IU",poFrame->m_oUV.m_oIni.x,DEF_IU,8);
                                    oFW.AddVar("IV",poFrame->m_oUV.m_oIni.y,DEF_IV,8);
                                    oFW.AddVar("FU",poFrame->m_oUV.m_oEnd.x,DEF_FU,8);
                                    oFW.AddVar("FV",poFrame->m_oUV.m_oEnd.y,DEF_FV,8);
									oFW.AddVar("ScaleXUEqually",poFrame->m_bScaleXUEqually,DEF_XYUVSCALE);
									oFW.AddVar("ScaleYVEqually",poFrame->m_bScaleYVEqually,DEF_XYUVSCALE);
									oFW.AddVar("UWorldCoords",poFrame->m_bUWorldCoords,DEF_WORLDCOORDS);
									oFW.AddVar("VWorldCoords",poFrame->m_bVWorldCoords,DEF_WORLDCOORDS);
									oFW.CloseSection(); // TexCoords

                            oFW.CloseSection(); // Frame
                        }
    
                    oFW.CloseSection();// Complete
                oFW.CloseSection(); // Frames
            oFW.CloseSection(); // Action        
        }

    oFW.CloseSection();    // Sprite section
    oFW.Write();

    return(true);
};
//---------------------------------------------------------------------------
void AddAnimProperty(FIBITMAP* dst,char* _szProperty,uint _uiID,FREE_IMAGE_MDTYPE _iType,uint _uiCount,uint _uiLen,void* _pValue)
{
	FITAG *tag = FreeImage_CreateTag();

	FreeImage_SetTagKey(tag,_szProperty);
	FreeImage_SetTagID(tag,_uiID);
	FreeImage_SetTagType(tag,_iType);
	FreeImage_SetTagCount(tag,_uiCount);
	FreeImage_SetTagLength(tag,_uiLen);
	FreeImage_SetTagValue(tag,_pValue);
	FreeImage_SetMetadata(FIMD_ANIMATION,dst,_szProperty,tag);
	
	// delete tag???
}
//-----------------------------------------------------------------------------
fipImage* poGetFrame(CFESpriteFrame* _poFrame)
{
	CFEString sMaterial = CFEMaterialMgr::I()->sGetResourceName((FEHandler*)_poFrame->m_hMaterial) + ".png";

	// load material ... 
	fipImage* poImg = new fipImage();	
	if (poImg->load(sMaterial.szString()))
	{
		poImg->convertTo32Bits();

		uint uiWidth  = poImg->getWidth();
		uint uiHeight = poImg->getHeight();

		int iIX = uiWidth*_poFrame->m_oUV.m_oIni.x-1;
		int iIY = uiHeight*(_poFrame->m_oUV.m_oIni.y)-1;
		int iFX = uiWidth*_poFrame->m_oUV.m_oEnd.x+1;
		int iFY = uiHeight*(_poFrame->m_oUV.m_oEnd.y)+1;
		
		if (iIX<0) iIX = 0;
		if (iIY<0) iIY = 0;
		if (iFX>=uiWidth) iFX = uiWidth-1;
		if (iFY>=uiHeight) iFY = uiHeight-1;

		poImg->crop(iIX,iIY,iFX,iFY);

		// Process transparency
		RGBQUAD oBackColor = {0xff,0x00,0xff,0x00};
		bool bIsTransparent = false;
		for (uint j=0;j<poImg->getHeight();j++)
			for (uint i=0;i<poImg->getWidth();i++)
			{
				RGBQUAD oRGBColor;
				poImg->getPixelColor(i,j,&oRGBColor);
			
				// is transparent this pixel?
				if ( (*(unsigned int *)&oRGBColor & 0xff000000) == 0 )
				{
					oRGBColor.rgbRed	= 255;
					oRGBColor.rgbGreen	= 0;
					oRGBColor.rgbBlue	= 255;

					poImg->setPixelColor(i,j,&oBackColor);
					bIsTransparent = true;
				}
			}

		// color-quantize 24bpp (results in a 8bpp bitmap to set transparency) 
		poImg->convertTo24Bits();
		poImg->colorQuantize(FIQ_WUQUANT);

		if (bIsTransparent)
		{
			// get palette and find bright magenta
			uint uiTransparentIdx = 0;
			RGBQUAD *Palette = poImg->getPalette();
			if (Palette != NULL)
			{
				BYTE Transparency[256];
				for (uint i = 0; i < 256; i++)
				{
					Transparency[i] = 0xFF;

					if ((Palette[i].rgbGreen == 0x00) && (Palette[i].rgbBlue >= 0xFE) && (Palette[i].rgbRed >= 0xFE))
					{
						Transparency[i] = 0x00;
						uiTransparentIdx = i;
					}
				}

				poImg->setTransparencyTable(Transparency, 256);
				poImg->setFileBkColor(&Palette[uiTransparentIdx]);
			}
		}
		else
			poImg->setFileBkColor(&oBackColor);

		// Get src page count
		unsigned long ulFrameTime = (unsigned long)(1000.0f* _poFrame->m_rFrameTime);
		AddAnimProperty(*poImg,"FrameTime"    ,0x1005,FIDT_LONG,1,4,(void*)&ulFrameTime);

/*
		static int iCount2 = 0;
		char szString2[1024];
		sprintf(szString2,"c:/frame%db.png",iCount2++);
		poImg->save(szString2);
*/

		return(poImg);
	}
	else
	{
		delete poImg;
		return(NULL);
	}
}
// ----------------------------------------------------------------------------
static void GenerateSpriteGeometry(CFESpriteFrame* _poFrame,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,CFEVect2* _poVX)
{
    FEReal rCosA = CFEMath::rCos(_rAngle);
    FEReal rSinA = CFEMath::rSin(_rAngle);

    CFEVect2 oPivot = (CFEVect2(_05r,_05r) - _poFrame->m_oPivot);

    _poVX[0]   = CFEVect2(-_05r,-_05r);
    _poVX[2]   = CFEVect2( _05r, _05r);
    _poVX[1].x = _poVX[2].x;
    _poVX[1].y = _poVX[0].y;
    _poVX[3].x = _poVX[0].x;
    _poVX[3].y = _poVX[2].y;

    for (uint i=0;i<4;i++)
    {
        // 1
        _poVX[i] += oPivot;

        // 2
        _poVX[i] *= _oScale;

		_poVX[i].x *= _poFrame->m_oSize.x;
		_poVX[i].y *= _poFrame->m_oSize.y;
		
		// 4
        CFEVect2 oPos = _poVX[i];
        _poVX[i].x = oPos.x*rCosA - oPos.y*rSinA;
        _poVX[i].y = oPos.x*rSinA + oPos.y*rCosA;

        // 5
        _poVX[i] += _oPos;
    }
}
//---------------------------------------------------------------------------
bool bSaveSpriteAsAnimatedGIF(CFESprite* _poSprite,const CFEString& _sFilename)
{
	// Open dst file (creation, use memory cache)
	FIMULTIBITMAP *dst = FreeImage_OpenMultiBitmap(FIF_GIF, _sFilename.szString(), TRUE, FALSE, FALSE);
	if (dst == NULL) return(false);
	
	class CPage
	{
		public:
			
			fipImage*		m_poImg;
			CFESpriteFrame* m_poFrame;
			CFEVect2		m_oVX[4];
	};
	
	std::vector<CPage> oFrames;
	
	FEReal rMinX = 1e6f;
	FEReal rMinY = 1e6f;
	FEReal rMaxX = -1e6f;
	FEReal rMaxY = -1e6f;

	CFESpriteAction* poAction =_poSprite->poGetAction(globals.m_iSelectedAction); 
	for (uint i=0;i<poAction->m_oSeq.size();i++)
	{
		CPage oPage;

		oPage.m_poFrame = &poAction->m_oSeq[i];
		oPage.m_poImg   = poGetFrame(oPage.m_poFrame);
		GenerateSpriteGeometry(oPage.m_poFrame,CFEVect2::ZERO(),CFEVect2::ONE(),_0r,oPage.m_oVX);

		for (uint j=0;j<4;j++)
		{
			if (oPage.m_oVX[j].x < rMinX) rMinX = oPage.m_oVX[j].x;
			if (oPage.m_oVX[j].y < rMinY) rMinY = oPage.m_oVX[j].y;
			if (oPage.m_oVX[j].x > rMaxX) rMaxX = oPage.m_oVX[j].x;
			if (oPage.m_oVX[j].y > rMaxY) rMaxY = oPage.m_oVX[j].y;
		}

		oFrames.push_back(oPage);
	}

	unsigned short uiCanvasWidth  = (rMaxX - rMinX) + 1;
	unsigned short uiCanvasHeight = (rMaxY - rMinY) + 1;

	// Get src page count
	for(int page = 0; page < oFrames.size(); page++)
	{
		// Load the bitmap at position 'page'
		FIBITMAP* poImg = *oFrames[page].m_poImg;
		CFESpriteFrame* poFrame = oFrames[page].m_poFrame;

		if (poImg != NULL)
		{
			if (page == 0)
			{
				AddAnimProperty(poImg,"LogicalWidth" ,0x0001,FIDT_SHORT,1,2,(void*)&uiCanvasWidth);
				AddAnimProperty(poImg,"LogicalHeight",0x0002,FIDT_SHORT,1,2,(void*)&uiCanvasHeight);

				unsigned long uiLoops = 0;
				if (poAction->eGetPlayMode() != SFSPM_LOOP)
					uiLoops = 1;

				AddAnimProperty(poImg,"Loop"         ,0x0004,FIDT_LONG ,1,4,(void*)&uiLoops);
			}

			short usWidth = oFrames[page].m_poImg->getWidth();
			short usHeight= oFrames[page].m_poImg->getHeight();
			short usLeft = oFrames[page].m_oVX[0].x - rMinX;
			short usTop  = oFrames[page].m_oVX[0].y - rMinY;
			if (usLeft < 0) usLeft = 0;
			if (usTop < 0) usTop = 0;
			
			BYTE bDisposalMethod = 2;
			AddAnimProperty(poImg,"DisposalMethod" ,0x1006,FIDT_BYTE,1,1,(void*)&bDisposalMethod);
			AddAnimProperty(poImg,"FrameLeft",0x1001,FIDT_SHORT,1,2,(void*)&usLeft);
			AddAnimProperty(poImg,"FrameTop" ,0x1002,FIDT_SHORT,1,2,(void*)&usTop);

			FreeImage_AppendPage(dst, poImg);
		}
	}

	FreeImage_CloseMultiBitmap(dst);
	return(true);
}
//---------------------------------------------------------------------------
