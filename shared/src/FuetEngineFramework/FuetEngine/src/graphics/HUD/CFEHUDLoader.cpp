// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "support/misc/CFEStringUtils.h"
#include "graphics/font/CFEFontMgr.h"
#include "graphics/sprite/CFESpriteInstMgr.h"

#include "CFEHUDLoader.h"
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUD.h"
#include "CFEHUDAction.h"
#include "CFEHUDElemLocator.h"
//-----------------------------------------------------------------------------
FEReal rGetActionTime(CFEHUDElementAction* _poAction)
{
	FEReal rActionTime = -_1r;
    for (uint i=0;i<_poAction->uiNumActions();i++)
    {
		CFEHUDObjectAction* poObjAction = _poAction->poGetAction(i);
		EFEKFBFuncWrapMode eWrapMode;
		FEReal rTime;

		eWrapMode = poObjAction->m_oPosFunc.eGetWrapMode();
		rTime = poObjAction->m_oPosFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poObjAction->m_oScaleFunc.eGetWrapMode();
		rTime = poObjAction->m_oScaleFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poObjAction->m_oColorFunc.eGetWrapMode();
		rTime = poObjAction->m_oColorFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poObjAction->m_rAngleFunc.eGetWrapMode();
		rTime = poObjAction->m_rAngleFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;
    }

    return(rActionTime);
}
//-----------------------------------------------------------------------------
CFEString CFEHUDLoader::m_sWorkingDir;
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDLoader::poLoad(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".hud";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);
	
	CFEConfigFile oConfig(sFilename);
	
	if (! oConfig.bInitialized() ) return(NULL);
    
    CFEHUD* poHUD = new CFEHUD;

    // Number of elements.
    uint uiNumElements = oConfig.iGetInteger("HUD.NumElements",0);

    // Read elements.
    for (uint j=0;j<uiNumElements;j++)
    {
        uint i;

        CFEString sVar = CFEString("HUD.Element") + CFEString(j);
        
        // Element name        
        CFEString sElemName = oConfig.sGetString(sVar + ".Name","nonamed-element");
    
        CFEHUDElement* poElem = new CFEHUDElement(sElemName);

        // Number of layers
        uint uiNumLayers = oConfig.iGetInteger(sVar + ".NumLayers",0);
        
        for (i=0;i<uiNumLayers;i++)
        {
            CFEString sPrefix = sVar + ".Layer" + CFEString(i);
            CFEHUDObject* poObj = poLoadObject(sPrefix,oConfig,poElem);

            poElem->uiAddLayer(poObj);
        }

        // Number of actions
        uint uiNumActions = oConfig.iGetInteger(sVar + ".NumElemActions",0);
        
        for (i=0;i<uiNumActions;i++)
        {
            CFEString sPrefix = sVar + ".Action" + CFEString(i);
            CFEHUDElementAction* poObj = poLoadAction(sPrefix,oConfig,poElem);

            poElem->uiAddAction(poObj);
        }
        
        // Add the new element.
        poHUD->uiAddElement(poElem);
    }

	return(poHUD);
}
//-----------------------------------------------------------------------------
void CFEHUDLoader::LoadObject(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile,CFEHUDObject* _poObj)
{
    // position
    FEReal rX = _oConfigFile.rGetReal(_sPrefix + ".Position.x",_0r);
    FEReal rY = _oConfigFile.rGetReal(_sPrefix + ".Position.y",_0r);
    _poObj->SetPos( CFEVect2(rX,rY) );
    
    // scale
    FEReal rSX = _oConfigFile.rGetReal(_sPrefix + ".Scale.x",_1r);
    FEReal rSY = _oConfigFile.rGetReal(_sPrefix + ".Scale.y",_1r);
    _poObj->SetScale( CFEVect2(rSX,rSY) );
    
    // angle
    FEReal rA = _oConfigFile.rGetReal(_sPrefix + ".Angle",_0r);
    _poObj->SetAngle( rA );
    
    // color
    FEReal rCR = _oConfigFile.rGetReal(_sPrefix + ".Color.r",_1r);
    FEReal rCG = _oConfigFile.rGetReal(_sPrefix + ".Color.g",_1r);
    FEReal rCB = _oConfigFile.rGetReal(_sPrefix + ".Color.b",_1r);
    FEReal rCA = _oConfigFile.rGetReal(_sPrefix + ".Color.a",_1r);

    _poObj->SetColor( CFEColor(rCR,rCG,rCB,rCA) );
    
    // visibility
    bool bVisible = _oConfigFile.bGetBool(_sPrefix + ".Visible",true);
    _poObj->Show( bVisible );
}
//-----------------------------------------------------------------------------
EFETextHAlignmentMode eGetHAlignFromString(const CFEString& _sAlign)
{
    if (_sAlign |= "left")
        return(THAM_LEFT);

else if (_sAlign |= "center")
    return(THAM_CENTER);
    
else if (_sAlign |= "right")
    return(THAM_RIGHT);
    
    return(THAM_LEFT);
}
//-----------------------------------------------------------------------------
EFETextVAlignmentMode eGetVAlignFromString(const CFEString& _sAlign)
{
    if (_sAlign |= "top")
        return(TVAM_TOP);

else if (_sAlign |= "center")
    return(TVAM_CENTER);
    
else if (_sAlign |= "bottom")
    return(TVAM_BOTTOM);

    return(TVAM_CENTER);
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDLoader::poLoadObject(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem)
{
    CFEString sElemType = _oConfigFile.sGetString(_sPrefix + ".Type","none");
    CFEString sElemName = _oConfigFile.sGetString(_sPrefix + ".Name","nonamed");

    if (sElemType |= "label")
    {
        CFEHUDLabel* poLabel = new CFEHUDLabel(sElemName);
        LoadObject(_sPrefix,_oConfigFile,poLabel);
        
        CFEString sText = _oConfigFile.sGetString(_sPrefix + ".Text","Label");
        poLabel->SetText( sText );

        CFEString sFont = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Font","undefined");
        CFEFont* poFont = CFEFontMgr::poLoad( sFont );
        poLabel->SetFont( poFont );

        CFEString sHAlign = _oConfigFile.sGetString(_sPrefix + ".HAlignment","left");
        poLabel->SetHAlignment( eGetHAlignFromString(sHAlign) );
        
        CFEString sVAlign = _oConfigFile.sGetString(_sPrefix + ".VAlignment","center");
        poLabel->SetVAlignment( eGetVAlignFromString(sVAlign) );
        
        return(poLabel);
    }

    else if (sElemType |= "icon")
    {
        CFEHUDIcon* poIcon = new CFEHUDIcon(sElemName);
        LoadObject(_sPrefix,_oConfigFile,poIcon);

		CFEString sSprite = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Sprite","undefined");

        FEHandler hSpriteInst = CFESpriteInstMgr::hGetInstance(sSprite);
        poIcon->SetIcon(hSpriteInst);
        CFESpriteInstMgr::Enable(hSpriteInst);
        CFESpriteInstMgr::ManageRender(hSpriteInst,false);

        uint uiAction = _oConfigFile.iGetInteger(_sPrefix + ".Action",0);
        poIcon->SetAction( uiAction );

        return(poIcon);
    }

    else if (sElemType |= "rect")
    {
        CFEHUDRect* poRect = new CFEHUDRect(sElemName);
        LoadObject(_sPrefix,_oConfigFile,poRect);
		
		// Load width / height / pivot
		FEReal rWidth  = _oConfigFile.rGetReal(_sPrefix + ".Width", _0r);
		FEReal rHeight = _oConfigFile.rGetReal(_sPrefix + ".Height",_0r);
		FEReal rPivotX = _oConfigFile.rGetReal(_sPrefix + ".PivotX",_0r);
		FEReal rPivotY = _oConfigFile.rGetReal(_sPrefix + ".PivotY",_0r);

		poRect->SetWidth(rWidth);
		poRect->SetHeight(rHeight);
		poRect->SetPivot( CFEVect2(rPivotX,rPivotY) );

		// Load corner colors
		for (uint i=0;i<4;i++)
		{
		    CFEString sCorner = CFEString(".Corner") + CFEString(i);
			CFEString sVar    = _sPrefix + sCorner;

			CFEColor oColor;
			oColor.r = _oConfigFile.rGetReal(sVar + ".r",_1r);
			oColor.g = _oConfigFile.rGetReal(sVar + ".g",_1r);
			oColor.b = _oConfigFile.rGetReal(sVar + ".b",_1r);
			oColor.a = _oConfigFile.rGetReal(sVar + ".a",_1r);					
			
			poRect->SetCornerColor(i,oColor);
		}

        return(poRect);
    }
    
    else if (sElemType |= "group")
    {
        CFEHUDGroup* poGroup = new CFEHUDGroup(sElemName);
        LoadObject(_sPrefix,_oConfigFile,poGroup);

        uint uiNumObjects = _oConfigFile.iGetInteger(_sPrefix + ".NumObjects",0);

        for (uint i=0;i<uiNumObjects;i++)
        {
            CFEString sObject = _sPrefix + ".Object" + CFEString(i);
            CFEHUDObject* poObj = poLoadObject(sObject,_oConfigFile,_poElem);

            poGroup->uiAddObject(poObj);
        }

        return(poGroup);
    }

    return(NULL);
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDLoader::poLoadAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem)
{
    CFEString sName = _oConfigFile.sGetString(_sPrefix + ".Name","noname action elem");
    CFEHUDElementAction* poElemAction = new CFEHUDElementAction(sName);

    uint uiNumObjActions = _oConfigFile.iGetInteger(_sPrefix + ".NumObjActions",0);
	for (uint i=0;i<uiNumObjActions;i++)
	{
	    CFEString sPrefix = CFEString(_sPrefix) + CFEString(".ObjAction") + CFEString(i);
	    CFEHUDObjectAction* poObjAction =  poLoadObjAction(sPrefix,_oConfigFile,_poElem);

	    poElemAction->uiAddAction(poObjAction);
    }

	poElemAction->SetActionTime( rGetActionTime(poElemAction) );
    return(poElemAction);
}
//-----------------------------------------------------------------------------
EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode)
{
    if (_sWrapMode |= "Loop")
        return(KFBFWM_LOOP);

else if (_sWrapMode |= "PingPong")
        return(KFBFWM_PINGPONG);

else if (_sWrapMode |= "InitialValue")
        return(KFBFWM_INITIALVALUE);

else if (_sWrapMode |= "FinalValue")
        return(KFBFWM_FINALVALUE);
    
    return(KFBFWM_FINALVALUE);
}
//-----------------------------------------------------------------------------
EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc)
{
     if (_sLerpFunc |= "constant")
        return(KFLF_CONSTANT);

else if (_sLerpFunc |= "sin")
        return(KFLF_SIN);
else if (_sLerpFunc |= "exp")
        return(KFLF_EXP);

else if (_sLerpFunc |= "random")
        return(KFLF_RAND);

else if (_sLerpFunc |= "linear")
        return(KFLF_LERP);

else if (_sLerpFunc |= "sinsin")
        return(KFLF_SINSIN);

else if (_sLerpFunc |= "explog")
        return(KFLF_EXPLOG);

    return(KFLF_CONSTANT);
}
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDLoader::poLoadObjAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem)
{
    CFEHUDObjectAction* poObjAction = new CFEHUDObjectAction;
    
    CFEString sHUDObject = _oConfigFile.sGetString(_sPrefix + ".HUDObject","");    
    CFEHUDElemLocator oLocator;
    CFEHUDObject* poObj = oLocator.poLocateHUDObject(_poElem,sHUDObject);
    poObjAction->SetHUDObject(poObj);

    uint        i;
    CFEString   sVar;
    uint        uiKeyFrames;
    CFEString   sWrapMode;

    // Load pos func
    sVar = _sPrefix + ".PosFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	poObjAction->m_oPosFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

        CFEVect2 oPos;
        oPos.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
        oPos.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

        poObjAction->m_oPosFunc.InsertKeyFrame(oPos,rTime,eGetLerpFunc(sLerpFunc) );
    }

    // Scale
    sVar = _sPrefix + ".ScaleFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	poObjAction->m_oScaleFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

        CFEVect2 oScale;
        oScale.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
        oScale.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

        poObjAction->m_oScaleFunc.InsertKeyFrame(oScale,rTime,eGetLerpFunc(sLerpFunc) );
    }

    // Angle
    sVar = _sPrefix + ".AngleFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	poObjAction->m_rAngleFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

        FEReal rAngle = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

        poObjAction->m_rAngleFunc.InsertKeyFrame(rAngle,rTime,eGetLerpFunc(sLerpFunc) );
    }
    
    // Color
    sVar = _sPrefix + ".ColorFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");

	poObjAction->m_oColorFunc.SetWrapMode( eGetWrapMode(sWrapMode) );
    
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

        CFEColor oColor;
        oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
        oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
        oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
        oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

        poObjAction->m_oColorFunc.InsertKeyFrame(oColor,rTime,eGetLerpFunc(sLerpFunc) );
    }

    return(poObjAction);
}
//-----------------------------------------------------------------------------
