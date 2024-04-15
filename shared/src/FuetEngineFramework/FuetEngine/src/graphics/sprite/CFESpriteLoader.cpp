// ----------------------------------------------------------------------------
/*! \class CFESpriteLoader
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "CFESpriteLoader.h"
// #include "System/FMem.h"
#include "support/file/CFEFile.h"
#include "support/config/CFEConfigFile.h"
#include "support/graphics/CFEMaterialMgr.h"
#include "support/misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
CFESprite* CFESpriteLoader::poBuildBasicSprite(FEHandler _hMat,const CFEString& _sSpriteName)
{
    CFESprite* poSprite = new CFESprite;

    poSprite->SetName(_sSpriteName);

    CFESpriteAction oAction;
    oAction.m_ePlayMode = SFSPM_ONESHOT;
	oAction.m_rActionTime = _0r;
	oAction.m_rRandStartTime = _0r;
    oAction.m_eBlendMode = BM_ALPHA;

    CFESpriteFrame oFrame;
    oFrame.m_oPivot     = CFEVect2(_0r,_0r);
    oFrame.m_oUV.m_oIni = CFEVect2(_0r,_0r);
    oFrame.m_oUV.m_oEnd = CFEVect2(_1r,_1r);
    oFrame.m_rBlendTime = _0r;
    oFrame.m_rDelay     = _0r;
    oFrame.m_rStartTime = _0r;
    oFrame.m_rFrameTime = _0r;
	oFrame.m_oPivot.x	= _05r;
	oFrame.m_oPivot.y	= _05r;
	oFrame.m_bScaleXUEqually = false;
	oFrame.m_bScaleYVEqually = false;
	oFrame.m_bUWorldCoords = false;
	oFrame.m_bVWorldCoords = false;

    uint uiTX = 0;
    uint uiTY = 0;
    CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,CFEString("DiffuseMap.Width") ,(FEPointer)&uiTX);
    CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,CFEString("DiffuseMap.Height"),(FEPointer)&uiTY);
    oFrame.m_oSize.x    = uiTX;
    oFrame.m_oSize.y    = uiTY;
    oFrame.m_hMaterial  = _hMat;

    oAction.m_oSeq.push_back(oFrame);
    poSprite->m_oActions.push_back(oAction);

    return( poSprite );
}
// ----------------------------------------------------------------------------
static ESFSPlayMode eGetPlayMode(const CFEString& _sString)
{
    if (_sString |= "ONESHOT")
        return(SFSPM_ONESHOT);

    else if (_sString |= "PINGPONGSTOP")
        return(SFSPM_PINGPONGSTOP);

    else if (_sString |= "LOOP")
        return(SFSPM_LOOP);

    else if (_sString |= "PINGPONG")
        return(SFSPM_PINGPONG);

    return(SFSPM_NONE);
}
// ----------------------------------------------------------------------------
static EFEBlendMode eGetBlendMode(const CFEString& _sString)
{
    if (_sString |= "ALPHA")
        return(BM_ALPHA);

    else if (_sString |= "ALPHAADD")
        return(BM_ALPHAADD);

    else if (_sString |= "ADD")
        return(BM_ADD);

    else if (_sString |= "MULT")
        return(BM_MULT);

    else if (_sString |= "COPY")
        return(BM_COPY);

    else if (_sString |= "FOG")
        return(BM_FOG);

    else if (_sString |= "FOGADD")
        return(BM_FOGADD);

    else if (_sString |= "MAGICMARKER")
        return(BM_MAGICMARKER);

    else if (_sString |= "LIGHTMARKER")
        return(BM_LIGHTMARKER);

    else if (_sString |= "LIGHTSABER")
        return(BM_LIGHTSABER);

    else if (_sString |= "REVEAL")
        return(BM_REVEAL);

    else if (_sString |= "LUMISHADE_REVEAL")
        return(BM_LUMISHADE_REVEAL);

    return(BM_NONE);
}
// ----------------------------------------------------------------------------
static uint uiGetWrapMode(const CFEString& _sString)
{
    if (_sString |= "REPEAT")
        return(1);

    else if (_sString |= "MIRROR")
        return(2);

    return(0);
}
// ----------------------------------------------------------------------------
static uint uiGetFilter(const CFEString& _sString)
{
    if (_sString |= "NEAREST")
        return(0);

    else if (_sString |= "LINEAR")
        return(1);

    return(1);
}
// ----------------------------------------------------------------------------
CFESprite* CFESpriteLoader::poLoad(const CFEString& _sFilename,FEBool _bLoadMaterials)
{
	if (_sFilename == "") return(NULL);

    CFEString sFilename = _sFilename + ".spr";
    CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

    CFEConfigFile oConfig(sFilename);
    if (! oConfig.bInitialized() )
    {
		if (_bLoadMaterials == false) return(NULL);

        // We haven't found the sprite definition file. Let's see i we can find
        // a material and then build a basic sprite with it.

		/// Retrieves the filename portion of a full qualified filename.
		CFEString sMaterial = sWorkingDir + CFEString("/") + CFEStringUtils::sGetFilename(_sFilename);
        FEHandler hMat = (_bLoadMaterials)?(FEHandler)CFEMaterialMgr::I()->poLoad(sMaterial):NULL;

        if (hMat)
            return( poBuildBasicSprite(hMat,sMaterial) );
        else
            return(NULL);
    }

    CFESprite* poSprite = new CFESprite;

    // Retrieve sprite name
    poSprite->SetName( oConfig.sGetString("Sprite.Name","nonamed") );

    // Number of actions
    uint uiNumActions = oConfig.iGetInteger("Sprite.NumActions",0);

    for (uint a=0;a<uiNumActions;a++)
    {
        CFESpriteAction oAction;
        oAction.m_rActionTime = _0r;

        CFEString sAction = CFEString("Sprite.Action") + CFEString((int)a);
        CFEString sVar;

        // Action name
        sVar =  sAction + ".Name";
        oAction.SetName( oConfig.sGetString(sVar,"nonamed") );

		// Action blend mode
		sVar =  sAction + ".BlendMode";
        oAction.m_eBlendMode = eGetBlendMode(oConfig.sGetString(sVar,"ALPHA") );

		// Action play mode
        sVar =  sAction + ".PlayMode";
        oAction.m_ePlayMode = eGetPlayMode(oConfig.sGetString(sVar,"ONESHOT"));

        sVar =  sAction + ".RandomStartTime";
        oAction.m_rRandStartTime = oConfig.rGetReal(sVar,_0r);

        sVar = sAction + ".Frames";

        CFEString sFVar = sVar + ".Type";

        CFEString sType = oConfig.sGetString(sFVar,"Regular");

        if (sType == "Regular")
        {
            sFVar  = sVar + ".Regular";
            CFEString sFRVar;

            sFRVar = sFVar + ".XFrames";
            uint uiXFrames = oConfig.iGetInteger(sFRVar,1);

            sFRVar = sFVar + ".YFrames";
            uint uiYFrames = oConfig.iGetInteger(sFRVar,1);

            sFRVar = sFVar + ".Material";
            CFEString sMaterial = sWorkingDir + CFEString("/") + oConfig.sGetString(sFRVar,"");
            FEHandler hMat = (_bLoadMaterials)?(FEHandler)CFEMaterialMgr::I()->poLoad(sMaterial):NULL;

            // Filter
            sFRVar = sFVar + ".Filter";
            CFEString sFilter = oConfig.sGetString(sFRVar,"linear");
			if (hMat!=NULL) CFEMaterialMgr::I()->bSetMaterialProperty(hMat,"DiffuseMap.Filter",(FEPointer)uiGetFilter(sFilter));

			sFRVar =  sFVar + ".FPS";
			FEReal rFPS = oConfig.rGetReal(sFRVar,_1r);

            sFRVar = sFVar + ".Pivot.x";
            FEReal rPivotX = oConfig.rGetReal(sFRVar,_05r);

			sFRVar = sFVar + ".Pivot.y";
			FEReal rPivotY = oConfig.rGetReal(sFRVar,_05r);

			sFRVar = sFVar + ".Size.Width";
			FEReal rFWidth = oConfig.rGetReal(sFRVar,_1r);

			sFRVar = sFVar + ".Size.Height";
			FEReal rFHeight = oConfig.rGetReal(sFRVar,_1r);


            // Create the frame sequence
            CFESpriteFrame oFrame;

            FEReal rXStep = _1r / (FEReal)uiXFrames;
            FEReal rYStep = _1r / (FEReal)uiYFrames;

            for (uint j=0;j<uiYFrames;j++)
            {
                for (uint i=0;i<uiXFrames;i++)
                {
                    oFrame.m_oPivot         = CFEVect2(rPivotX,rPivotY);
                    oFrame.m_oUV.m_oIni     = CFEVect2((FEReal)i*rXStep,(FEReal)j*rYStep);
                    oFrame.m_oUV.m_oEnd     = CFEVect2((FEReal)(i+1)*rXStep,(FEReal)(j+1)*rYStep);
                    oFrame.m_hMaterial      = hMat;
					oFrame.m_o1OverDims		= CFEVect2(0,0);
					oFrame.m_oSize.x        = rFWidth;
					oFrame.m_oSize.y        = rFHeight;
					oFrame.m_rDelay         = _0r;
                    oFrame.m_rBlendTime     = _1r / rFPS;
                    oFrame.m_rFrameTime     = oFrame.m_rDelay + oFrame.m_rBlendTime;
                    oFrame.m_rStartTime     = oAction.m_rActionTime;
					oFrame.m_bScaleXUEqually = false;
					oFrame.m_bScaleYVEqually = false;
					oFrame.m_bUWorldCoords	= false;
					oFrame.m_bVWorldCoords	= false;

                    oAction.m_rActionTime  += oFrame.m_rFrameTime;

                    oAction.m_oSeq.push_back(oFrame);
				}
            }
        }

   else if (sType == "Sequence")
        {
            sFVar  = sVar + ".Sequence";
            CFEString sFRVar;

            // Number of frames in the sequence
            sFRVar = sFVar + ".Frames";
            uint uiFrames = oConfig.iGetInteger(sFRVar,1);

            // Material filename root
            sFRVar = sFVar + ".Material";
            CFEString sMaterial = sWorkingDir + CFEString("/") + oConfig.sGetString(sFRVar,"");

            // Pivot
            sFRVar = sFVar + ".Pivot.x";
            FEReal rPivotX = oConfig.rGetReal(sFRVar,_05r);

            sFRVar = sFVar + ".Pivot.y";
            FEReal rPivotY = oConfig.rGetReal(sFRVar,_05r);

            // Create the frame sequence
            CFESpriteFrame oFrame;

            for (uint i=0;i<uiFrames;i++)
            {
                CFEString sFrameTex = sMaterial + CFEString((int)i);

                oFrame.m_oPivot     = CFEVect2(rPivotX,rPivotY);
                oFrame.m_oUV.m_oIni = CFEVect2(0,0);
                oFrame.m_oUV.m_oEnd = CFEVect2(1,1);
                oFrame.m_hMaterial  = (_bLoadMaterials)?(FEHandler)CFEMaterialMgr::I()->poLoad(sFrameTex):NULL;
				oFrame.m_o1OverDims = CFEVect2(0,0);
                oFrame.m_rStartTime = oAction.m_rActionTime;
				oFrame.m_bScaleXUEqually = false;
				oFrame.m_bScaleYVEqually = false;
				oFrame.m_bUWorldCoords = false;
				oFrame.m_bVWorldCoords = false;

                oAction.m_rActionTime  += oFrame.m_rFrameTime;

                oAction.m_oSeq.push_back(oFrame);
            }
        }

   else if (sType == "Complete")
        {
            // Read every frame one by one
            sFVar  = sVar + ".Complete";
            CFEString sFCVar;

            // Number of frames in the sequence
            sFCVar = sFVar + ".NumFrames";
            uint uiFrames = oConfig.iGetInteger(sFCVar,1);

            // Create the frame sequence
            CFESpriteFrame oFrame;

            for (uint i=0;i<uiFrames;i++)
            {
                sFCVar = sFVar + ".Frame" + CFEString((int)i);
                CFEString sFrameVar;

                // Material filename
                sFrameVar = sFCVar + ".Material";
                CFEString sMaterial = sWorkingDir + CFEString("/") + oConfig.sGetString(sFrameVar,"");
				FEHandler hMat = (_bLoadMaterials)?(FEHandler)CFEMaterialMgr::I()->poLoad(sMaterial):NULL;

				// Wrap mode
                sFrameVar = sFCVar + ".SWrapMode";
                CFEString sSWrapMode = oConfig.sGetString(sFrameVar,"clamp");
				CFEMaterialMgr::I()->bSetMaterialProperty(hMat,"DiffuseMap.SWrapMode",(FEPointer)uiGetWrapMode(sSWrapMode));

                sFrameVar = sFCVar + ".TWrapMode";
                CFEString sTWrapMode = oConfig.sGetString(sFrameVar,"clamp");
				CFEMaterialMgr::I()->bSetMaterialProperty(hMat,"DiffuseMap.TWrapMode",(FEPointer)uiGetWrapMode(sTWrapMode));

				// Filter
                sFrameVar = sFCVar + ".Filter";
                CFEString sFilter = oConfig.sGetString(sFrameVar,"linear");
				CFEMaterialMgr::I()->bSetMaterialProperty(hMat,"DiffuseMap.Filter",(FEPointer)uiGetFilter(sFilter));

				// Read delay time
                sFrameVar = sFCVar + ".DelayTime";
                FEReal rDelayTime = oConfig.rGetReal(sFrameVar,_0r);

				// Read blend time
                sFrameVar = sFCVar + ".BlendTime";
                FEReal rBlendTime = oConfig.rGetReal(sFrameVar,_0r);

                // Pivot
                sFrameVar = sFCVar + ".Pivot.x";
                FEReal rPivotX = oConfig.rGetReal(sFrameVar,_05r);

                sFrameVar = sFCVar + ".Pivot.y";
                FEReal rPivotY = oConfig.rGetReal(sFrameVar,_05r);

                // Size
			    sFrameVar = sFCVar + ".Size.Width";
			    FEReal rFWidth = oConfig.rGetReal(sFrameVar,_1r);

			    sFrameVar = sFCVar + ".Size.Height";
			    FEReal rFHeight = oConfig.rGetReal(sFrameVar,_1r);

                // Tex coords
                sFrameVar = sFCVar + ".TexCoords.IU";
                FEReal rIU = oConfig.rGetReal(sFrameVar,_0r);

                sFrameVar = sFCVar + ".TexCoords.IV";
                FEReal rIV = oConfig.rGetReal(sFrameVar,_0r);

                sFrameVar = sFCVar + ".TexCoords.FU";
                FEReal rFU = oConfig.rGetReal(sFrameVar,_1r);

                sFrameVar = sFCVar + ".TexCoords.FV";
                FEReal rFV = oConfig.rGetReal(sFrameVar,_1r);

				sFrameVar = sFCVar + ".TexCoords.UWorldCoords";
                FEBool bUWorldCoords = oConfig.bGetBool(sFrameVar,false);

				sFrameVar = sFCVar + ".TexCoords.VWorldCoords";
                FEBool bVWorldCoords = oConfig.bGetBool(sFrameVar,false);

				sFrameVar = sFCVar + ".TexCoords.ScaleXUEqually";
				FEBool bScaleXUEqually = oConfig.bGetBool(sFrameVar,false);

				sFrameVar = sFCVar + ".TexCoords.ScaleYVEqually";
				FEBool bScaleYVEqually = oConfig.bGetBool(sFrameVar,false);
				
				FEReal r1OverW = _0r;
				FEReal r1OverH = _0r;
				if (bUWorldCoords)
				{
					uint uiTX = 0;
					CFEMaterialMgr::I()->bGetMaterialProperty(hMat,CFEString("DiffuseMap.Width") ,(FEPointer)&uiTX);
					if (uiTX == 0) uiTX = 1;
					r1OverW = _1r / (FEReal)uiTX;
				}

				if (bVWorldCoords)
				{
					uint uiTY = 0;
					CFEMaterialMgr::I()->bGetMaterialProperty(hMat,CFEString("DiffuseMap.Height"),(FEPointer)&uiTY);
					if (uiTY == 0) uiTY = 1;
					r1OverH = _1r / (FEReal)uiTY;
				}

                oFrame.m_hMaterial  = hMat;

				oFrame.m_o1OverDims = CFEVect2(r1OverW,r1OverH);
                oFrame.m_oPivot     = CFEVect2(rPivotX,rPivotY);
                oFrame.m_oUV.m_oIni = CFEVect2(rIU,rIV);
                oFrame.m_oUV.m_oEnd = CFEVect2(rFU,rFV);
                oFrame.m_oSize      = CFEVect2(rFWidth,rFHeight);
                oFrame.m_rDelay     = rDelayTime;
                oFrame.m_rBlendTime = rBlendTime;
                oFrame.m_rFrameTime = rBlendTime + rDelayTime;
                oFrame.m_rStartTime = oAction.m_rActionTime;
				oFrame.m_bScaleXUEqually = bScaleXUEqually;
				oFrame.m_bScaleYVEqually = bScaleYVEqually;
				oFrame.m_bUWorldCoords = bUWorldCoords;
				oFrame.m_bVWorldCoords = bVWorldCoords;
				oAction.m_rActionTime+= oFrame.m_rFrameTime;

                oAction.m_oSeq.push_back(oFrame);
            }
        }

        poSprite->m_oActions.push_back(oAction);
    }

    return( poSprite );
}
// ----------------------------------------------------------------------------
