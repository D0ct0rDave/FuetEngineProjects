// ----------------------------------------------------------------------------
/*! \class CFESkelAnimLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESprite.h"
#include "support/misc/CFEStringUtils.h"

#include "CFESkelAnimLoader.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimSpriteModel.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
#include "CFESkelAnimMeshModel.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimNodeTableBuilder.h"
#include "CFESkelAnimUtils.h"

#include "core/CFECore.h"
#include "types/CFEKFBFuncUtils.h"


//-----------------------------------------------------------------------------
CFEString CFESkelAnimLoader::m_sWorkingDir;
//-----------------------------------------------------------------------------
CFESkelAnim* CFESkelAnimLoader::poLoad(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".ska";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);

	if (! oConfig.bInitialized() )
	{
	    CFECore::Log::Print("WARNING: Unable to load skeletal anim %s\n",_sFilename.szString());
	    return(NULL);
    }
    
    CFEString sAnimName = oConfig.sGetString("SkelAnim.Name","nonamed-skel-anim");

	// Read the skeleton animation node
	CFESkelAnimNode* poAnimNode = poLoadNode("SkelAnim.AnimNode",oConfig);
	
	if (poAnimNode == NULL)
	{
		// error
		CFECore::Log::Print("WARNING: Unable to load skeletal node in anim file %s\n",_sFilename.szString());
		return(NULL);
	}	

	// Create the object    
    CFESkelAnim* poSkelAnim = new CFESkelAnim(sAnimName);
    poSkelAnim->SetAnimNode(poAnimNode);
    
	BuildActionNodeTable(poSkelAnim);

	CFESkelAnimActionSet* poActionSet = poLoadActionSet("SkelAnim.ActionSet",oConfig,poSkelAnim);
	if (poActionSet != NULL)
		poSkelAnim->SetActionSet(poActionSet);
    else
        CFECore::Log::Print("WARNING: Unable to load action set in anim file %s\n",_sFilename.szString());

	return(poSkelAnim);
}
//-----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimLoader::poLoadNode(const CFEString& _sFilename)
{
	CFEString sFilename = _sFilename + ".ske";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);

	if (! oConfig.bInitialized() )
	{
	    CFECore::Log::Print("WARNING: Unable to skeleton %s\n",_sFilename.szString());
	    return(NULL);
	}
    
	// Read the skeleton animation node
	return( poLoadNode("SkelNode",oConfig) );
}
//-----------------------------------------------------------------------------
CFESkelAnimAction* CFESkelAnimLoader::poLoadAction(const CFEString& _sFilename, CFESkelAnim* _poAnim)
{
	CFEString sFilename = _sFilename + ".ska";
	m_sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oConfig(sFilename);

	if (! oConfig.bInitialized() )
	{	    
	    CFECore::Log::Print("WARNING: Unable to load action %s\n",_sFilename.szString());
	    return(NULL);
    }

	// Read the skeleton animation node
	return( poLoadAction("SkelAction",oConfig, _poAnim) );
}
//-----------------------------------------------------------------------------
// Load common properties of skeleton animation nodes
//-----------------------------------------------------------------------------
void CFESkelAnimLoader::LoadNode(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile,CFESkelAnimNode* _poNode)
{
    // position
    FEReal rX = _oConfigFile.rGetReal(_sPrefix + ".Position.x",_0r);
    FEReal rY = _oConfigFile.rGetReal(_sPrefix + ".Position.y",_0r);
    _poNode->SetPos( CFEVect2(rX,rY) );

    // angle
    FEReal rA = _oConfigFile.rGetReal(_sPrefix + ".Angle",_0r);
    _poNode->SetAngle( rA );
    
	#ifdef USE_SKELANIM_SCALE_COLOR
	// scale
    FEReal rSX = _oConfigFile.rGetReal(_sPrefix + ".Scale.x",_1r);
    FEReal rSY = _oConfigFile.rGetReal(_sPrefix + ".Scale.y",_1r);
    _poNode->SetScale( CFEVect2(rSX,rSY) );
    
    // color
    FEReal rCR = _oConfigFile.rGetReal(_sPrefix + ".Color.r",_1r);
    FEReal rCG = _oConfigFile.rGetReal(_sPrefix + ".Color.g",_1r);
    FEReal rCB = _oConfigFile.rGetReal(_sPrefix + ".Color.b",_1r);
    FEReal rCA = _oConfigFile.rGetReal(_sPrefix + ".Color.a",_1r);
	_poNode->SetColor( CFEColor(rCR,rCG,rCB,rCA) );
	#endif

    // Depth
    FEReal rDepth = _oConfigFile.rGetReal(_sPrefix + ".Depth",_0r);
    _poNode->SetDepth( rDepth );

    // visibility
    FEBool bVisible = _oConfigFile.bGetBool(_sPrefix + ".Visible",true);
    _poNode->Show( bVisible );
}
//-----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimLoader::poLoadNode(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile)
{
    CFEString sNodeType = _oConfigFile.sGetString(_sPrefix + ".Type","none");
    CFEString sNodeName = _oConfigFile.sGetString(_sPrefix + ".Name","nonamed");

	if (sNodeType |= "sprite")
    {
        CFESkelAnimSpriteModel* poSprite = new CFESkelAnimSpriteModel(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poSprite);
			
        CFEString sSprite = _oConfigFile.sGetString(_sPrefix + ".Sprite","");

        if (sSprite != "")
			sSprite = m_sWorkingDir + CFEString("/") + sSprite;

		poSprite->SetSprite( sSprite );

		uint uiSpriteAction = _oConfigFile.iGetInteger(_sPrefix + ".SpriteAction",0);
		poSprite->SetSpriteAction( uiSpriteAction );

        return(poSprite);
    }
	/*
	else if (sNodeType |= "spriteinst")
    {
        CFESkelAnimSprite* poSprite = new CFESkelAnimSprite(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poSprite);

        CFEString sSprite = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Sprite","");
        FEHandler hInstance = CFESpriteInstMgr::hGetInstance(sSprite);
        CFESpriteInstMgr::ManageRender(hInstance, false);
		poSprite->SetSprite( hInstance );

        return(poSprite);
    }
	*/
    else if (sNodeType |= "group")
    {
        CFESkelAnimGroup* poGroup = new CFESkelAnimGroup(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poGroup);

        uint uiNumChildren = _oConfigFile.iGetInteger(_sPrefix + ".NumChildren",0);

        for (uint i=0;i<uiNumChildren;i++)
        {
            CFEString sNode = _sPrefix + ".Child" + CFEString((int)i);
            CFESkelAnimNode* poNode = poLoadNode(sNode,_oConfigFile/*,_poAction*/);

            poGroup->uiAddChild(poNode);
        }

        return(poGroup);
    }

	else if (sNodeType |= "bone")
    {
        CFESkelAnimBone* poBone = new CFESkelAnimBone(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poBone);

		// bone position
		FEReal rX = _oConfigFile.rGetReal(_sPrefix + ".BonePos.x",_0r);
		FEReal rY = _oConfigFile.rGetReal(_sPrefix + ".BonePos.y",_0r);
		poBone->SetBonePos( CFEVect2(rX,rY) );

		// read bone children
        uint uiNumChildren = _oConfigFile.iGetInteger(_sPrefix + ".NumChildren",0);

        for (uint i=0;i<uiNumChildren;i++)
        {
            CFEString sNode = _sPrefix + ".Child" + CFEString((int)i);
            CFESkelAnimNode* poNode = poLoadNode(sNode,_oConfigFile/*,_poAction*/);

            poBone->uiAddChild(poNode);
        }

        return(poBone);
    }

    else if (sNodeType |= "mesh")
    {
        CFESkelAnimMeshModel* poMesh = new CFESkelAnimMeshModel(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poMesh);
        
        CFEString sSprite = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Sprite","");
        poMesh->SetSprite(sSprite);
        
		uint uiSpriteAction = _oConfigFile.iGetInteger(_sPrefix + ".SpriteAction",0);
		poMesh->SetSpriteAction( uiSpriteAction );

        uint uiNumAttachedBones = _oConfigFile.iGetInteger(_sPrefix + ".NumAttachedBones",0);

        for (uint i=0;i<uiNumAttachedBones;i++)
            poMesh->AttachBone( _oConfigFile.sGetString(_sPrefix + ".AttachedBone" + CFEString((int)i) + CFEString(".Name"),"nonamed-bone") );

        return(poMesh);
    }
    
    CFECore::Log::Print("WARNING: Unknown node type %s\n",sNodeType.szString());
    return(NULL);
}
//-----------------------------------------------------------------------------
CFESkelAnimActionSet* CFESkelAnimLoader::poLoadActionSet(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim)
{
	CFESkelAnimActionSet* poActionSet = new CFESkelAnimActionSet;

    // Number of actions.
    uint uiNumActions = _oConfigFile.iGetInteger(_sPrefix + ".NumActions",0);

    // Read actions.
    for (uint j=0;j<uiNumActions;j++)
    {
        CFEString sVar = _sPrefix + CFEString(".Action") + CFEString((int)j);

		// Load action
        CFESkelAnimAction* poAction = poLoadAction(sVar,_oConfigFile,_poAnim);
	
        // Add the new action.
        if (poAction != NULL)
			poActionSet->uiAddAction(poAction);
        else
            CFECore::Log::Print("WARNING: Unable to load action %d\n",j);
    }

    return(poActionSet);
}
//-----------------------------------------------------------------------------
CFESkelAnimAction* CFESkelAnimLoader::poLoadAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim)
{
    CFEString sName = _oConfigFile.sGetString(_sPrefix + ".Name","nonamed action");
    CFESkelAnimAction* poAction = new CFESkelAnimAction(sName);

    // Number of node actions
    uint uiNumNodeActions = _oConfigFile.iGetInteger(_sPrefix + ".NumNodeActions",0);
    for (uint i=0;i<uiNumNodeActions;i++)
    {
        CFEString sPrefix = _sPrefix + CFEString(".NodeAction") + CFEString((int)i);
        CFESkelAnimNodeAction* poNodeAction = poLoadNodeAction(sPrefix,_oConfigFile,_poAnim);

        if (poNodeAction != NULL)
			poAction->uiAddNodeAction(poNodeAction);
        else
            CFECore::Log::Print("WARNING: Unable to load node action %d\n",i);
    }

	poAction->SetPlayMode(CFESkelAnimUtils::eGetActionPlayMode( _oConfigFile.sGetString(_sPrefix + ".PlayMode","loop") ) );
	poAction->SetActionTime(CFESkelAnimUtils::rGetActionTime(_poAnim, poAction) );
	poAction->SetMaxActionTime(CFESkelAnimUtils::rGetMaxActionTime(_poAnim, poAction) );

    return(poAction);
}
//-----------------------------------------------------------------------------
CFESkelAnimNodeAction* CFESkelAnimLoader::poLoadNodeAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim)
{
    CFEString sNodeName = _oConfigFile.sGetString(_sPrefix + ".NodeName","");
    int iNodeIdx = iFindNode(_poAnim,sNodeName);
    if (iNodeIdx == -1)
    {
		// error
		CFECore::Log::Print("WARNING: Cannot find node %s for node action.\n",sNodeName.szString());
		return(NULL);
    }

    CFESkelAnimNodeAction*	poNodeAction = new CFESkelAnimNodeAction;

	// Associate the node index with the action.
    poNodeAction->SetNodeIdx(iNodeIdx);

	// Is the object associated with this action visible?
	poNodeAction->Show( _oConfigFile.bGetBool(_sPrefix + ".Visible",true) );

	if (poNodeAction->bIsVisible())
	{
		uint					i;
		CFEString				sVar;
		uint					uiKeyFrames;
		CFEString				sWrapMode;
		EFEKFBFuncWrapMode		eWrapMode;
		FEReal					rTime;

		// Pos
		sVar = _sPrefix + ".PosFunc";
		sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");
		eWrapMode = CFEKFBFuncUtils::eGetWrapMode(sWrapMode);
		poNodeAction->m_oPosFunc.SetWrapMode( eWrapMode );

		rTime = _0r;
		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
		if (uiKeyFrames>0)
		{
			for (i=0;i<uiKeyFrames;i++)
			{
				CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);

				CFEVect2 oPos;
				oPos.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
				oPos.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
				rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
				CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

				poNodeAction->m_oPosFunc.InsertKeyFrame(oPos,rTime,CFEKFBFuncUtils::eGetLerpFunc(sLerpFunc) );
			}
		}

		// Angle
		sVar = _sPrefix + ".AngleFunc";
		sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");
		eWrapMode = CFEKFBFuncUtils::eGetWrapMode(sWrapMode);
		poNodeAction->m_rAngleFunc.SetWrapMode( eWrapMode );

		rTime = _0r;
		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
		if (uiKeyFrames>0)
		{
    		for (i=0;i<uiKeyFrames;i++)
			{
				CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);

				FEReal rAngle = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
				rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
				CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

				poNodeAction->m_rAngleFunc.InsertKeyFrame(rAngle,rTime,CFEKFBFuncUtils::eGetLerpFunc(sLerpFunc) );
			}
		}
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		// Scale
		sVar = _sPrefix + ".ScaleFunc";
		sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");    
		eWrapMode = CFEKFBFuncUtils::eGetWrapMode(sWrapMode);
		poNodeAction->m_oScaleFunc.SetWrapMode( eWrapMode );

		rTime = _0r;
		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);    
		if (uiKeyFrames>0)
		{
			for (i=0;i<uiKeyFrames;i++)
			{
				CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);

				CFEVect2 oScale;
				oScale.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
				oScale.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
				rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
				CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

				poNodeAction->m_oScaleFunc.InsertKeyFrame(oScale,rTime,CFEKFBFuncUtils::eGetLerpFunc(sLerpFunc) );
			}
		}

		// Color
		sVar = _sPrefix + ".ColorFunc";
		sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");    
		eWrapMode = CFEKFBFuncUtils::eGetWrapMode(sWrapMode);
		poNodeAction->m_oColorFunc.SetWrapMode( eWrapMode );

		rTime = _0r;
		uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
		if (uiKeyFrames>0)
		{
			for (i=0;i<uiKeyFrames;i++)
			{
				CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString((int)i);

				CFEColor oColor;
				oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
				oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
				oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
				oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

				rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
				CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

				poNodeAction->m_oColorFunc.InsertKeyFrame(oColor,rTime,CFEKFBFuncUtils::eGetLerpFunc(sLerpFunc) );
			}
		}
		#endif

		// Retrieve the action to be played on this node (only for sprites)
		CFESkelAnimNodeTypeGetter oTypeGetter;
		EFESkelAnimNodeType eType = oTypeGetter.eGetType(_poAnim->m_oNodeTab[iNodeIdx]);

		if ((eType == SANT_SPRITE) || (eType == SANT_MESH) || (eType == SANT_SPRITE_MODEL) || (eType == SANT_MESH_MODEL))
		{
			int iNodeAction = _oConfigFile.iGetInteger(_sPrefix + ".NodeAction",-1);
			poNodeAction->SetNodeActionIdx(iNodeAction);
		}
		else
			poNodeAction->SetNodeActionIdx(-1);
	}

    return(poNodeAction);
}
//-----------------------------------------------------------------------------
int CFESkelAnimLoader::iFindNode(CFESkelAnim* _poAnim,const CFEString& _sName)
{
	for (uint i=0;i<_poAnim->m_oNodeTab.size();i++)
		if (_poAnim->m_oNodeTab[i]->sGetName() |= _sName)
			return(i);

	return(-1);
}
//-----------------------------------------------------------------------------
void CFESkelAnimLoader::BuildActionNodeTable(CFESkelAnim* _poAnim)
{
	// Traverse all the nodes of the animation and build a table with them.
	CFESkelAnimNodeTableBuilder::BuildTable(&_poAnim->m_oNodeTab,_poAnim->poGetAnimNode());
}
//-----------------------------------------------------------------------------
