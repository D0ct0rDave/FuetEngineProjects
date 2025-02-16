// ----------------------------------------------------------------------------
/*! \class CFESpriteLoader
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

    poSprite->m_eBlendMode = BM_ALPHA;
    poSprite->m_sName      = _sSpriteName;

    CFESpriteAction oAction;
    oAction.m_ePlayMode = SFSPM_ONESHOT;
	oAction.m_rActionTime = _0r;
	oAction.m_rRandStartTime = _0r;

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

    uint uiTX = 0;
    uint uiTY = 0;
    CFEMaterialMgr::bGetMaterialProperty(_hMat,CFEString("DiffuseMap.Width") ,(FEPointer)&uiTX);
    CFEMaterialMgr::bGetMaterialProperty(_hMat,CFEString("DiffuseMap.Height"),(FEPointer)&uiTY);
    oFrame.m_oSize.x    = uiTX;
    oFrame.m_oSize.y    = uiTY;
    oFrame.m_hMaterial  = _hMat;

    oAction.m_oSeq.push_back(oFrame);
    poSprite->m_oActions.push_back(oAction);

    return( poSprite );
}
// ----------------------------------------------------------------------------
ESFSPlayMode eGetPlayMode(const CFEString& _sString)
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
EFEBlendMode eGetBlendMode(const CFEString& _sString)
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

    return(BM_NONE);
}
// ----------------------------------------------------------------------------
CFESprite* CFESpriteLoader::poLoad(const CFEString& _sFilename)
{
    CFEString sFilename = _sFilename + ".spr";
    CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

    CFEConfigFile oConfig(sFilename);
    if (! oConfig.bInitialized() )
    {
        // We haven't found the sprite definition file. Let's see i we can find
        // a material and then build a basic sprite with it.
        FEHandler hMat = CFEMaterialMgr::hLoad(_sFilename);

        if (hMat)
            return( poBuildBasicSprite(hMat,_sFilename) );
        else
            return(NULL);
    }

    CFESprite* poSprite = new CFESprite;

    // Retrieve sprite name
    poSprite->m_sName = oConfig.sGetString("Sprite.Name","nonamed");

    // Retrieve sprite blend mode
    poSprite->m_eBlendMode = eGetBlendMode(oConfig.sGetString("Sprite.BlendMode","ALPHA") );

    // Number of actions
    uint uiNumActions = oConfig.iGetInteger("Sprite.NumActions",0);

    for (uint j=0;j<uiNumActions;j++)
    {
        CFESpriteAction oAction;
        oAction.m_rActionTime = _0r;

        CFEString sAction = CFEString("Sprite.Action") + CFEString(j);
        CFEString sVar;

        // Action play mode
        sVar =  sAction + ".Name";
        oAction.m_sName = oConfig.sGetString(sVar,"nonamed");
        
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
            FEHandler hTex = (FEHandler)CFEMaterialMgr::poLoad(sMaterial);

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
                    oFrame.m_hMaterial      = hTex;
					oFrame.m_oSize.x        = rFWidth;
					oFrame.m_oSize.y        = rFHeight;
					oFrame.m_rDelay         = _0r;
                    oFrame.m_rBlendTime     = _1r / rFPS;
                    oFrame.m_rFrameTime     = oFrame.m_rDelay + oFrame.m_rBlendTime;
                    oFrame.m_rStartTime     = oAction.m_rActionTime;

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
                CFEString sFrameTex = sMaterial + CFEString(i);

                oFrame.m_oPivot     = CFEVect2(rPivotX,rPivotY);
                oFrame.m_oUV.m_oIni = CFEVect2(0,0);
                oFrame.m_oUV.m_oEnd = CFEVect2(1,1);
                oFrame.m_hMaterial  = CFEMaterialMgr::hLoad(sFrameTex);
                oFrame.m_rStartTime = oAction.m_rActionTime;
                
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
                sFCVar = sFVar + ".Frame" + CFEString(i);
                CFEString sFrameVar;

                // Material filename
                sFrameVar = sFCVar + ".Material";
                CFEString sMaterial = sWorkingDir + CFEString("/") + oConfig.sGetString(sFrameVar,"");                
                FEHandler hTex = (FEHandler)CFEMaterialMgr::poLoad(sMaterial);

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

                oFrame.m_hMaterial  = hTex;
                oFrame.m_oPivot     = CFEVect2(rPivotX,rPivotY);
                oFrame.m_oUV.m_oIni = CFEVect2(rIU,rIV);
                oFrame.m_oUV.m_oEnd = CFEVect2(rFU,rFV);
                oFrame.m_oSize      = CFEVect2(rFWidth,rFHeight);
                oFrame.m_rDelay     = rDelayTime;
                oFrame.m_rBlendTime = rBlendTime;
                oFrame.m_rFrameTime = rBlendTime + rDelayTime;
                oFrame.m_rStartTime = oAction.m_rActionTime;

                oAction.m_rActionTime  += oFrame.m_rFrameTime;

                oAction.m_oSeq.push_back(oFrame);
            }
        }

        poSprite->m_oActions.push_back(oAction);
    }

    return( poSprite );
}
// ----------------------------------------------------------------------------