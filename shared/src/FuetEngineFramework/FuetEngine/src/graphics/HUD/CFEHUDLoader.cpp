// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "support/misc/CFEStringUtils.h"
#include "graphics/font/CFEFontMgr.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
#include "graphics/particlesys/CFEParticleSysInstMgr.h"

#include "types/CFEKFBFuncUtils.h"
#include "types/CFEKFBEventFunc.h"

#include "CFEHUDLoader.h"
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"
#include "CFEHUDPSys.h"
#include "CFEHUD.h"
#include "CFEHUDAction.h"
#include "CFEHUDElemLocator.h"
#include "CFEHUDActionTime.h"
//-----------------------------------------------------------------------------
const CFEString DEFAULT_WRAP_MODE = CFEString("finalvalue");
const CFEString DEFAULT_LERP_FUNC = CFEString("linear");
//-----------------------------------------------------------------------------
CFEString CFEHUDLoader::m_sWorkingDir;
uint	CFEHUDLoader::m_uiFileVersion = 1;
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDLoader::poLoad(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".hud";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);

	if (! oConfig.bInitialized() ) return(NULL);

    CFEHUD* poHUD = new CFEHUD;

	// FileVersion
	m_uiFileVersion = oConfig.iGetInteger("HUD.FileVersion",1);

    // Number of elements.
    uint uiNumElements = oConfig.iGetInteger("HUD.NumElements",0);

    // Read elements.
    for (uint j=0;j<uiNumElements;j++)
    {
        CFEString sVar = CFEString("HUD.Element") + CFEString((int)j);
        CFEHUDElement* poElem = poLoadElement(sVar,oConfig);

        // Add the new element.
        poHUD->uiAddElement(poElem);
    }

	return(poHUD);
}
//-----------------------------------------------------------------------------
/// Loads a HUD element from disk
CFEHUDElement* CFEHUDLoader::poLoadElement(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".hel";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);
	if (! oConfig.bInitialized() ) return(NULL);

    return( poLoadElement("HUDElement",oConfig) );
}
//-----------------------------------------------------------------------------
/// Loads a HUD object from disk
CFEHUDObject* CFEHUDLoader::poLoadObject(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".hob";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);
	if (! oConfig.bInitialized() ) return(NULL);

    return( poLoadObject("HUDObject",oConfig) );
}
//-----------------------------------------------------------------------------
void CFEHUDLoader::LoadElementActions(const CFEString& _sFilename,CFEHUDElement* _poElem)
{
	CFEString sFilename = _sFilename + ".hea";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);
	if (! oConfig.bInitialized() ) return;

	LoadElementActions("HUDElementActions",oConfig,_poElem);
}
//-----------------------------------------------------------------------------
CFEHUDElement* CFEHUDLoader::poLoadElement(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile)
{
	// Element name
	CFEString sElemName = _oConfigFile.sGetString(_sPrefix + ".Name","nonamed-element");
	CFEHUDElement* poElem = new CFEHUDElement(sElemName);

	// Number of layers
	uint uiNumLayers = _oConfigFile.iGetInteger(_sPrefix + ".NumLayers",0);

	uint i;
	for (i=0;i<uiNumLayers;i++)
	{
		CFEString sPrefix = _sPrefix + ".Layer" + CFEString((int)i);
		CFEHUDObject* poObj = poLoadObject(sPrefix,_oConfigFile);

		poElem->uiAddLayer(poObj);
	}

	// Number of actions
	LoadElementActions(_sPrefix,_oConfigFile,poElem);

	return(poElem);
}
//-----------------------------------------------------------------------------
void CFEHUDLoader::LoadCommonObjectProperties(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile,CFEHUDObject* _poObj)
{
    // position
    FEReal rX = _oConfigFile.rGetReal(_sPrefix + ".Position.x",_0r);
    FEReal rY = _oConfigFile.rGetReal(_sPrefix + ".Position.y",_0r);
    _poObj->SetIniPos( CFEVect2(rX,rY) );

    // depth
    FEReal rD = _oConfigFile.rGetReal(_sPrefix + ".Depth",_0r);
    _poObj->SetIniDepth( rD );

    // scale
    FEReal rSX = _oConfigFile.rGetReal(_sPrefix + ".Scale.x",_1r);
    FEReal rSY = _oConfigFile.rGetReal(_sPrefix + ".Scale.y",_1r);
    _poObj->SetIniScale( CFEVect2(rSX,rSY) );

    // angle
    FEReal rA = _oConfigFile.rGetReal(_sPrefix + ".Angle",_0r);
    _poObj->SetIniAngle( rA );

    // color
    FEReal rCR = _oConfigFile.rGetReal(_sPrefix + ".Color.r",_1r);
    FEReal rCG = _oConfigFile.rGetReal(_sPrefix + ".Color.g",_1r);
    FEReal rCB = _oConfigFile.rGetReal(_sPrefix + ".Color.b",_1r);
    FEReal rCA = _oConfigFile.rGetReal(_sPrefix + ".Color.a",_1r);

    _poObj->SetIniColor( CFEColor(rCR,rCG,rCB,rCA) );

    // visibility
    FEBool bVisible = _oConfigFile.bGetBool(_sPrefix + ".Visible",true);
    _poObj->ShowObj( bVisible );

    // action
    int iAction = _oConfigFile.iGetInteger(_sPrefix + ".Action",-1);
    _poObj->SetIniAction( iAction );
    
    // TAG
    CFEString sTAG = _oConfigFile.sGetString(_sPrefix + ".TAG",CFEString::sNULL());
    _poObj->SetTAG( sTAG );
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
CFEHUDObject* CFEHUDLoader::poLoadObject(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile)
{
    CFEString sElemType = _oConfigFile.sGetString(_sPrefix + ".Type","none");
    CFEString sElemName = _oConfigFile.sGetString(_sPrefix + ".Name","nonamed");

    if (sElemType |= "label")
    {
        CFEHUDLabel* poLabel = new CFEHUDLabel(sElemName);
        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poLabel);

        CFEString sText = _oConfigFile.sGetString(_sPrefix + ".Text","Label");
        poLabel->SetText( sText );
		
		CFEString sFont = _oConfigFile.sGetString(_sPrefix + ".Font",CFEString::sNULL());
		if (sFont != NULL)
        {
			CFEString sFontFile = m_sWorkingDir + CFEString("/") + sFont;
			CFEFont* poFont = CFEFontMgr::I()->poLoad( sFontFile );
			poLabel->SetFont( poFont );
		}

        CFEString sHAlign = _oConfigFile.sGetString(_sPrefix + ".HAlignment","left");
        poLabel->SetHAlignment( eGetHAlignFromString(sHAlign) );

        CFEString sVAlign = _oConfigFile.sGetString(_sPrefix + ".VAlignment","center");
        poLabel->SetVAlignment( eGetVAlignFromString(sVAlign) );

		FEReal rTracking, rInterlining, rPointSize,rAdjustmentWidth;
		rTracking	 = _oConfigFile.rGetReal(_sPrefix + ".Tracking",_0r);
		rInterlining = _oConfigFile.rGetReal(_sPrefix + ".Interlining",_0r);
		rPointSize   = _oConfigFile.rGetReal(_sPrefix + ".PointSize",_1r);
		rAdjustmentWidth = _oConfigFile.rGetReal(_sPrefix + ".AdjustmentWidth",-_1r);

        poLabel->SetTracking( rTracking );
		poLabel->SetInterlining( rInterlining );
        poLabel->SetPointSize( rPointSize );
        poLabel->SetAdjustmentWidth( rAdjustmentWidth );

		return(poLabel);
    }

    else if (sElemType |= "icon")
    {
        CFEHUDIcon* poIcon = new CFEHUDIcon(sElemName);
        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poIcon);

		CFEString sSprite = _oConfigFile.sGetString(_sPrefix + ".Sprite",CFEString::sNULL());
		if (sSprite != "")
		{
			CFEString sSpriteFile = m_sWorkingDir + CFEString("/") + sSprite;
			FEHandler hSpriteInst = CFESpriteInstMgr::I()->hGetInstance(sSpriteFile);

			if (hSpriteInst != NULL)
			{
				poIcon->SetIcon(hSpriteInst);

				CFESpriteInstMgr::I()->SetAction(hSpriteInst,poIcon->iGetAction());
				CFESpriteInstMgr::I()->Enable(hSpriteInst);
				CFESpriteInstMgr::I()->ManageRender(hSpriteInst,false);
			}
		}

        return(poIcon);
    }

    else if (sElemType |= "rect")
    {
        CFEHUDRect* poRect = new CFEHUDRect(sElemName);
        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poRect);

		// Load width / height / pivot
		FEReal rWidth  = _oConfigFile.rGetReal(_sPrefix + ".Width", _0r);
		FEReal rHeight = _oConfigFile.rGetReal(_sPrefix + ".Height",_0r);
		FEReal rPivotX = _oConfigFile.rGetReal(_sPrefix + ".PivotX",_05r);
		FEReal rPivotY = _oConfigFile.rGetReal(_sPrefix + ".PivotY",_05r);

		poRect->SetWidth(rWidth);
		poRect->SetHeight(rHeight);
		poRect->SetPivot( CFEVect2(rPivotX,rPivotY) );

		// Load corner colors
		for (uint i=0;i<4;i++)
		{
		    CFEString sCorner = CFEString(".Corner") + CFEString((int)i);
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

    else if (sElemType |= "shape")
    {
        CFEHUDShape* poShape = new CFEHUDShape(sElemName);
        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poShape);

		CFEString sMesh = _oConfigFile.sGetString(_sPrefix + ".Mesh",CFEString::sNULL());
		if (sMesh != "")
		{
			CFEString sMeshFile = m_sWorkingDir + CFEString("/") + sMesh;
			FEHandler hMeshInst = CFEMeshInstMgr::I()->hGetInstance(sMeshFile);

			if (hMeshInst != NULL)
			{
				poShape->SetMesh(hMeshInst);

				CFEMeshInstMgr::I()->SetAction(hMeshInst,poShape->iGetAction());
				CFEMeshInstMgr::I()->Enable(hMeshInst);
				CFEMeshInstMgr::I()->ManageRender(hMeshInst,false);
			}
		}

        return(poShape);
    }

    else if (sElemType |= "psys")
    {
        CFEHUDPSys* poPSys = new CFEHUDPSys(sElemName);
        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poPSys);

		CFEString sPSys = _oConfigFile.sGetString(_sPrefix + ".PSys",CFEString::sNULL());
		if (sPSys != "")
		{
			CFEString sPSysFile = m_sWorkingDir + CFEString("/") + sPSys;
			FEHandler hPSysInst = CFEParticleSysInstMgr::I()->hGetInstance(sPSysFile);

			if (hPSysInst != NULL)
			{
				poPSys->SetPSys(hPSysInst);
			
				CFEParticleSysInstMgr::I()->Enable(hPSysInst);
				CFEParticleSysInstMgr::I()->ManageRender(hPSysInst,false);
			}
		}
        return(poPSys);
    }

    else if (sElemType |= "group")
    {
		uint uiNumObjects = _oConfigFile.iGetInteger(_sPrefix + ".NumObjects",0);
        CFEHUDGroup* poGroup = new CFEHUDGroup(sElemName,uiNumObjects);

        LoadCommonObjectProperties(_sPrefix,_oConfigFile,poGroup);

		FEReal rDepthFact = _oConfigFile.rGetReal(_sPrefix + ".DepthFact",_01r);
        poGroup->SetDepthFact(rDepthFact);

        for (uint i=0;i<uiNumObjects;i++)
        {
            CFEString sObject = _sPrefix + ".Object" + CFEString((int)i);
            CFEHUDObject* poObj = poLoadObject(sObject,_oConfigFile);

            poGroup->uiAddObject(poObj);
		}

        return(poGroup);
    }

    return(NULL);
}
//-----------------------------------------------------------------------------
void CFEHUDLoader::LoadElementActions(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile,CFEHUDElement* _poElem)
{
	uint uiNumActions = _oConfigFile.iGetInteger(_sPrefix + ".NumElemActions",0);

	for (uint i=0;i<uiNumActions;i++)
	{
		CFEString sPrefix = _sPrefix + ".Action" + CFEString((int)i);
		CFEHUDElementAction* poObj = poLoadAction(sPrefix,_oConfigFile,_poElem);

		_poElem->uiAddAction(poObj);
	}
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDLoader::poLoadAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem)
{
    CFEString sName = _oConfigFile.sGetString(_sPrefix + ".Name","noname action elem");
    CFEHUDElementAction* poElemAction = new CFEHUDElementAction(sName);

    uint uiNumObjActions = _oConfigFile.iGetInteger(_sPrefix + ".NumObjActions",0);
	for (uint i=0;i<uiNumObjActions;i++)
	{
	    CFEString sPrefix = CFEString(_sPrefix) + CFEString(".ObjAction") + CFEString((int)i);
	    CFEHUDObjectAction* poObjAction =  poLoadObjAction(sPrefix,_oConfigFile,_poElem);

		if (poObjAction != NULL)
			poElemAction->uiAddAction(poObjAction);
    }

	poElemAction->SetActionTime( CFEHUDActionTime::rGetActionTime(poElemAction) );
	poElemAction->SetMaxActionTime( CFEHUDActionTime::rGetMaxActionTime(poElemAction) );

    return(poElemAction);
}
//-----------------------------------------------------------------------------
// - DMC WARNING <18-Feb-2012>: Changed default number of keyframes from 0 to 1 
// when reading key framed func data. Look for for side effects!
// - DMC NOTE <05-Oct-2013>: Optimization avoiding the use of string constructors
// and other string operation highly decreased the loading time on N3DS.
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDLoader::poLoadObjAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem)
{
    CFEString sHUDObject = _oConfigFile.sGetString(_sPrefix + ".HUDObject",CFEString::sNULL());    
    if (sHUDObject == "")
		return(NULL);
		
	CFEHUDElemLocator oLocator;
	CFEHUDObject* poObj = oLocator.poLocateHUDObject(_poElem,sHUDObject);
    if (poObj== NULL)
		return(NULL);

    CFEHUDObjectAction* poObjAction = new CFEHUDObjectAction;
    poObjAction->SetHUDObject(poObj);

	/// Retrieve object action (for sprites and meshes)
	uint        i;
    CFEString   sVar;
    uint        uiKeyFrames;
    CFEString   sWrapMode;

	/// To keep compatibility loading previous HUD files.
    if (m_uiFileVersion > 1)
    {
		// Load X func
		sVar = _sPrefix + ".XFunc";
		
		if (_oConfigFile.bExists(sVar + ".WrapMode"))
			poObjAction->m_rXFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
		else
			poObjAction->m_rXFunc.SetWrapMode( KFBFWM_FINALVALUE );

		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
		if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

		sVar += ".KeyFrame";
		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString((int)i);

			FEReal rX = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
			FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

			if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
				poObjAction->m_rXFunc.InsertKeyFrame(rX,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
			else
				poObjAction->m_rXFunc.InsertKeyFrame(rX,rTime,KFLF_LERP);			
		}
	    
		// Load Y func
		sVar = _sPrefix + ".YFunc";
		if (_oConfigFile.bExists(sVar + ".WrapMode"))
			poObjAction->m_rYFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
		else
			poObjAction->m_rYFunc.SetWrapMode( KFBFWM_FINALVALUE );

		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
		if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

		sVar += ".KeyFrame";
		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString((int)i);

			FEReal rY = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
			FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

			if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
				poObjAction->m_rYFunc.InsertKeyFrame(rY,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
			else
				poObjAction->m_rYFunc.InsertKeyFrame(rY,rTime,KFLF_LERP);
		}
	}
	else
	{
		// Load Pos Func (for HUD files previous to 4-Oct-2012)
		sVar = _sPrefix + ".PosFunc";

		if (_oConfigFile.bExists(sVar + ".WrapMode"))
		{
			EFEKFBFuncWrapMode eWM = CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE));
			poObjAction->m_rXFunc.SetWrapMode(eWM);
			poObjAction->m_rYFunc.SetWrapMode(eWM);
		}
		else
		{
			poObjAction->m_rXFunc.SetWrapMode( KFBFWM_FINALVALUE );
			poObjAction->m_rYFunc.SetWrapMode( KFBFWM_FINALVALUE );
		}

		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
		if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);
			FEReal rX = _oConfigFile.rGetReal(sIVar + ".x",_0r);
			FEReal rY = _oConfigFile.rGetReal(sIVar + ".y",_0r);
			FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

			if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
			{
				EFEKFLerpFunc eLerpFunc = CFEKFBFuncUtils::eGetLerpFunc( _oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC) );
				poObjAction->m_rXFunc.InsertKeyFrame(rX,rTime,eLerpFunc);
				poObjAction->m_rYFunc.InsertKeyFrame(rY,rTime,eLerpFunc);
			}
			else
			{
				poObjAction->m_rXFunc.InsertKeyFrame(rX,rTime,KFLF_LERP);
				poObjAction->m_rYFunc.InsertKeyFrame(rY,rTime,KFLF_LERP);
			}
		}
	}

    // Scale
    sVar = _sPrefix + ".ScaleFunc";
	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_oScaleFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_oScaleFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

	sVar += ".KeyFrame";
    for (i=0;i<uiKeyFrames;i++)
    {
		CFEString sIVar = sVar + CFEString((int)i);

        CFEVect2 oScale;
        oScale.x = _oConfigFile.rGetReal(sIVar + ".x",_1r);
        oScale.y = _oConfigFile.rGetReal(sIVar + ".y",_1r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

		if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
			poObjAction->m_oScaleFunc.InsertKeyFrame(oScale,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
		else
			poObjAction->m_oScaleFunc.InsertKeyFrame(oScale,rTime,KFLF_LERP);
	}

    // Angle
    sVar = _sPrefix + ".AngleFunc";
	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_rAngleFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_rAngleFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

	sVar += ".KeyFrame";
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString((int)i);

        FEReal rAngle = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
        
        if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
			poObjAction->m_rAngleFunc.InsertKeyFrame(rAngle,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
		else
			poObjAction->m_rAngleFunc.InsertKeyFrame(rAngle,rTime,KFLF_LERP);
	}     


    // Depth
    sVar = _sPrefix + ".DepthFunc";
   	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_rDepthFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_rDepthFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values
	
	sVar += ".KeyFrame";
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString((int)i);

        FEReal rDepth = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

		if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
			poObjAction->m_rDepthFunc.InsertKeyFrame(rDepth,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
		else
			poObjAction->m_rDepthFunc.InsertKeyFrame(rDepth,rTime,KFLF_LERP);
    }

    // Color
    sVar = _sPrefix + ".ColorFunc";

   	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_oColorFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_oColorFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values
	
	sVar += ".KeyFrame";
    for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString((int)i);

        CFEColor oColor;
        oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
        oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
        oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
        oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

		if (_oConfigFile.bExists(sIVar + ".LerpFunc"))
			poObjAction->m_oColorFunc.InsertKeyFrame(oColor,rTime,CFEKFBFuncUtils::eGetLerpFunc(_oConfigFile.sGetString(sIVar + ".LerpFunc",DEFAULT_LERP_FUNC)) );
		else
			poObjAction->m_oColorFunc.InsertKeyFrame(oColor,rTime,KFLF_LERP);
    }

    // Visibility
    sVar = _sPrefix + ".VisFunc";
   	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_bVisFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_bVisFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

	sVar += ".KeyFrame";
    for (i=0;i<uiKeyFrames;i++)    
    {
        CFEString sIVar = sVar + CFEString((int)i);

        FEBool bVisibility = _oConfigFile.bGetBool(sIVar + ".Value",true);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

		// visibility functions should have constant interpolation
        poObjAction->m_bVisFunc.InsertKeyFrame(bVisibility,rTime,KFLF_CONSTANT);
    }
    
    // Actions
    sVar = _sPrefix + ".ActionFunc";
   	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_iActionFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_iActionFunc.SetWrapMode( KFBFWM_FINALVALUE );
    
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values

    sVar += ".KeyFrame";
	for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString((int)i);

        int iAction = _oConfigFile.iGetInteger(sIVar + ".Value",-1);
        FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

		// Action functions should have constant interpolation
        poObjAction->m_iActionFunc.InsertKeyFrame(iAction,rTime,KFLF_CONSTANT);
    }

    // Events
    sVar = _sPrefix + ".EventFunc";
   	if (_oConfigFile.bExists(sVar + ".WrapMode"))
		poObjAction->m_oEventFunc.SetWrapMode( CFEKFBFuncUtils::eGetWrapMode(_oConfigFile.sGetString(sVar + ".WrapMode",DEFAULT_WRAP_MODE)) );
	else
		poObjAction->m_oEventFunc.SetWrapMode( KFBFWM_FINALVALUE );

    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",1);
    if (uiKeyFrames == 0) uiKeyFrames = 1;	// will use default values
    
    sVar += ".KeyFrame";
	for (i=0;i<uiKeyFrames;i++)
    {
        CFEString sIVar = sVar + CFEString((int)i);
		FEReal rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);

        CFEEvent oEvent;
        oEvent.m_sEventName  = _oConfigFile.sGetString(sIVar + ".EventName", CFEString::sNULL());
        oEvent.m_sEventValue = _oConfigFile.sGetString(sIVar + ".EventValue",CFEString::sNULL());
        // oEvent.SetEventTime(rTime);

        poObjAction->m_oEventFunc.InsertKeyFrame(oEvent,rTime,KFLF_NONE);
    }

    return(poObjAction);
}
//-----------------------------------------------------------------------------
