// ----------------------------------------------------------------------------
/*! \class CFESkelAnimLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

#include "system/CFESystem.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// BEGIN: HELPERS
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef enum TSkelAnimNodeType
{
	SANT_NONE,
	
	SANT_SPRITE,
	SANT_GROUP,
	
	SANT_NUM
};
//-----------------------------------------------------------------------------
class CFESkelAnimNodeActionTimeGetter : public CFESkelAnimNodeVisitor
{
	public:

		float fGetTime(CFESkelAnimNode* _poNode,unsigned int _uiAction)
		{
			m_uiAction	= _uiAction;
			m_fMaxTime	= -_1r;
			m_fTime		= _0r;

			_poNode->Accept(this);
			return(m_fMaxTime);
		}

		/*
		virtual void Visit(CFESkelAnimSprite* _poNode)
		{
			CFESprite* poSprite = CFESpriteInstMgr::poGetSprite( _poNode->hGetSprite() );

			if ((poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_LOOP) && (poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_PINGPONG))
				m_fTime = poSprite->poGetAction(m_uiAction)->m_rActionTime;
		}
        */

		virtual void Visit(CFESkelAnimGroup* _poNode)
		{
			for (uint i=0;i<_poNode->uiNumObjs();i++)
			{
				if (m_fTime != -_1r)
					if (m_fTime > m_fMaxTime)
						m_fMaxTime = m_fTime;
			}
		}

		virtual void Visit(CFESkelAnimSpriteModel* _poNode)
		{
			CFESprite* poSprite = CFESpriteMgr::poLoad( _poNode->sGetSprite() );
			if ((poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_LOOP) && (poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_PINGPONG))
				m_fTime = poSprite->poGetAction(m_uiAction)->m_rActionTime;
		}

	protected:

		float			m_fTime;
		float			m_fMaxTime;
		unsigned int	m_uiAction;
};
//-----------------------------------------------------------------------------
class CFESkelAnimNodeTypeGetter : public CFESkelAnimNodeVisitor
{
	public:

		TSkelAnimNodeType eGetType(CFESkelAnimNode* _poNode)
		{
			m_eType = SANT_NONE;
			_poNode->Accept(this);
			return(m_eType);
		}

		virtual void Visit(CFESkelAnimGroup* _poNode)
		{
			m_eType = SANT_GROUP;
		}

		virtual void Visit(CFESkelAnimSpriteModel* _poNode)
		{
			m_eType = SANT_SPRITE;
		}
	
	protected:
		
		TSkelAnimNodeType m_eType;
		
};

//-----------------------------------------------------------------------------
FEReal rGetActionTime(CFESkelAnimAction* _poAction)
{
	uint i;
	FEReal rActionTime = -_1r;
    for (i=0;i<_poAction->uiNumNodeActions();i++)
    {
		CFESkelAnimNodeAction* poNodeAction = _poAction->poGetNodeAction(i);
		EFEKFBFuncWrapMode eWrapMode;
		FEReal rTime;

		eWrapMode = poNodeAction->m_oPosFunc.eGetWrapMode();
		rTime = poNodeAction->m_oPosFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poNodeAction->m_oScaleFunc.eGetWrapMode();
		rTime = poNodeAction->m_oScaleFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poNodeAction->m_oColorFunc.eGetWrapMode();
		rTime = poNodeAction->m_oColorFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

		eWrapMode = poNodeAction->m_rAngleFunc.eGetWrapMode();
		rTime = poNodeAction->m_rAngleFunc.rGetMaxLimit();
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;
    }

    return(rActionTime);
}
//-----------------------------------------------------------------------------
static EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode)
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
static ESAAPlayMode eGetActionPlayMode(const CFEString& _sPlayMode)
{
    if (_sPlayMode |= "Loop")
        return(SAAPM_LOOP);

else if (_sPlayMode |= "PingPong")
        return(SAAPM_PINGPONG);

else if (_sPlayMode |= "FinalValue")
        return(SAAPM_ONESHOT);

	return(SAAPM_ONESHOT);
}
//-----------------------------------------------------------------------------
static EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc)
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
        
	return(KFLF_LERP);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// END: HELPERS
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

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
	    CFESystem::Log::Print("WARNING: Unable to load skeletal anim %s\n",_sFilename.szString());
	    return(NULL);
    }
    
    CFEString sAnimName = oConfig.sGetString("SkelAnim.Name","nonamed-skel-anim");

	// Read the skeleton animation node
	CFESkelAnimNode* poAnimNode = poLoadNode("SkelAnim.AnimNode",oConfig);
	
	if (poAnimNode == NULL)
	{
		// error
		CFESystem::Log::Print("WARNING: Unable to load skeletal node in anim file %s\n",_sFilename.szString());
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
        CFESystem::Log::Print("WARNING: Unable to load action set in anim file %s\n",_sFilename.szString());

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
	    CFESystem::Log::Print("WARNING: Unable to skeleton %s\n",_sFilename.szString());
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
	    CFESystem::Log::Print("WARNING: Unable to load action %s\n",_sFilename.szString());
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

    // scale
    FEReal rSX = _oConfigFile.rGetReal(_sPrefix + ".Scale.x",_1r);
    FEReal rSY = _oConfigFile.rGetReal(_sPrefix + ".Scale.y",_1r);
    _poNode->SetScale( CFEVect2(rSX,rSY) );

    // angle
    FEReal rA = _oConfigFile.rGetReal(_sPrefix + ".Angle",_0r);
    _poNode->SetAngle( rA );
    
    // color
    FEReal rCR = _oConfigFile.rGetReal(_sPrefix + ".Color.r",_1r);
    FEReal rCG = _oConfigFile.rGetReal(_sPrefix + ".Color.g",_1r);
    FEReal rCB = _oConfigFile.rGetReal(_sPrefix + ".Color.b",_1r);
    FEReal rCA = _oConfigFile.rGetReal(_sPrefix + ".Color.a",_1r);

    // Depth
    FEReal rDepth = _oConfigFile.rGetReal(_sPrefix + ".Depth",_0r);
    _poNode->SetDepth( rDepth );

    _poNode->SetColor( CFEColor(rCR,rCG,rCB,rCA) );
    
    // visibility
    bool bVisible = _oConfigFile.bGetBool(_sPrefix + ".Visible",true);
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

        CFEString sSprite = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Sprite","undefined");
		poSprite->SetSprite( sSprite );
        return(poSprite);
    }

	else if (sNodeType |= "spriteinst")
    {
        CFESkelAnimSprite* poSprite = new CFESkelAnimSprite(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poSprite);

        CFEString sSprite = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Sprite","undefined");
        FEHandler hInstance = CFESpriteInstMgr::hGetInstance(sSprite);
        CFESpriteInstMgr::ManageRender(hInstance, false);
		poSprite->SetSprite( hInstance );

        return(poSprite);
    }

    else if (sNodeType |= "group")
    {
        CFESkelAnimGroup* poGroup = new CFESkelAnimGroup(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poGroup);

        uint uiNumNodes = _oConfigFile.iGetInteger(_sPrefix + ".NumNodes",0);

        for (uint i=0;i<uiNumNodes;i++)
        {
            CFEString sNode = _sPrefix + ".Node" + CFEString(i);
            CFESkelAnimNode* poNode = poLoadNode(sNode,_oConfigFile/*,_poAction*/);

            poGroup->uiAddNode(poNode);
        }

        return(poGroup);
    }

	else if (sNodeType |= "bone")
    {
        CFESkelAnimBone* poBone = new CFESkelAnimBone(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poBone);

		CFEString sNodeType = _oConfigFile.sGetString(_sPrefix + ".AttachedNode.Type","none");
		if (sNodeType != "none")
		{
            CFESkelAnimNode* poNode = poLoadNode(_sPrefix + ".AttachedNode",_oConfigFile);
            poBone->SetAttachedNode(poNode);		    
		}

        return(poBone);
    }

    else if (sNodeType |= "mesh")
    {
        CFESkelAnimMeshModel* poMesh = new CFESkelAnimMeshModel(sNodeName);
        LoadNode(_sPrefix,_oConfigFile,poMesh);
        
        CFEString sMaterial = m_sWorkingDir + CFEString("/") + _oConfigFile.sGetString(_sPrefix + ".Material","undefined");
        poMesh->SetMaterial(sMaterial);
        
        uint uiNumAttachedBones = _oConfigFile.iGetInteger(_sPrefix + ".NumAttachedBones",0);

        for (uint i=0;i<uiNumAttachedBones;i++)
            poMesh->AttachBone( _oConfigFile.sGetString(_sPrefix + ".AttachedBone" + CFEString(i) + CFEString(".Name"),"nonamed-bone") );

        return(poMesh);
    }
    
    CFESystem::Log::Print("WARNING: Unknown node type %s\n",sNodeType);
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
        CFEString sVar = _sPrefix + CFEString(".Action") + CFEString(j);

		// Load action
        CFESkelAnimAction* poAction = poLoadAction(sVar,_oConfigFile,_poAnim);
	
        // Add the new action.
        if (poAction != NULL)
			poActionSet->uiAddAction(poAction);
        else
            CFESystem::Log::Print("WARNING: Unable to load action %d\n",j);
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
        CFEString sPrefix = _sPrefix + CFEString(".NodeAction") + CFEString(i);
        CFESkelAnimNodeAction* poNodeAction = poLoadNodeAction(sPrefix,_oConfigFile,_poAnim);

        if (poNodeAction != NULL)
			poAction->uiAddNodeAction(poNodeAction);
        else
            CFESystem::Log::Print("WARNING: Unable to load node action %d\n",i);
    }

	poAction->SetPlayMode( eGetActionPlayMode( _oConfigFile.sGetString(_sPrefix + ".PlayMode","OneShot") ) );
	poAction->SetActionTime( rGetActionTime(poAction) );

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
		CFESystem::Log::Print("WARNING: Cannot find node %s for node action.\n",sNodeName);
		return(NULL);
    }

    CFESkelAnimNodeAction*	poNodeAction = new CFESkelAnimNodeAction;

    uint					i;
    CFEString				sVar;
    uint					uiKeyFrames;
    CFEString				sWrapMode;
	EFEKFBFuncWrapMode		eWrapMode;
	FEReal					rActionTime = _0r;
	FEReal					rTime;

    // Pos
    sVar = _sPrefix + ".PosFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");
	eWrapMode = eGetWrapMode(sWrapMode);
	poNodeAction->m_oPosFunc.SetWrapMode( eWrapMode );

    rTime = _0r;
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);
    if (uiKeyFrames>0)
    {
		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

			CFEVect2 oPos;
			oPos.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
			oPos.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
			rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
			CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

			poNodeAction->m_oPosFunc.InsertKeyFrame(oPos,rTime,eGetLerpFunc(sLerpFunc) );
		}		
	}
	else
		poNodeAction->m_oPosFunc.InsertKeyFrame(CFEVect2(0,0),_0r,KFLF_CONSTANT);

    // Scale
    sVar = _sPrefix + ".ScaleFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");    
	eWrapMode = eGetWrapMode(sWrapMode);
	poNodeAction->m_oScaleFunc.SetWrapMode( eWrapMode );

    rTime = _0r;
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);    
    if (uiKeyFrames>0)
    {
		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

			CFEVect2 oScale;
			oScale.x = _oConfigFile.rGetReal(sIVar + ".x",_0r);
			oScale.y = _oConfigFile.rGetReal(sIVar + ".y",_0r);
			rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
			CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

			poNodeAction->m_oScaleFunc.InsertKeyFrame(oScale,rTime,eGetLerpFunc(sLerpFunc) );
		}
	}
	else
		poNodeAction->m_oScaleFunc.InsertKeyFrame(CFEVect2(1,1),_0r,KFLF_CONSTANT);

    // Angle
    sVar = _sPrefix + ".AngleFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");    
	eWrapMode = eGetWrapMode(sWrapMode);
	poNodeAction->m_rAngleFunc.SetWrapMode( eWrapMode );
    
    rTime = _0r;
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);    
	if (uiKeyFrames>0)
    {
    	for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

			FEReal rAngle = _oConfigFile.rGetReal(sIVar + ".Value",_0r);
			rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
			CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

			poNodeAction->m_rAngleFunc.InsertKeyFrame(rAngle,rTime,eGetLerpFunc(sLerpFunc) );
		}
	}
	else
		poNodeAction->m_rAngleFunc.InsertKeyFrame(_0r,_0r,KFLF_CONSTANT);

    // Color
    sVar = _sPrefix + ".ColorFunc";
    sWrapMode = _oConfigFile.sGetString(sVar + ".WrapMode","finalvalue");    
	eWrapMode = eGetWrapMode(sWrapMode);
	poNodeAction->m_oColorFunc.SetWrapMode( eWrapMode );

    rTime = _0r;
    uiKeyFrames = _oConfigFile.iGetInteger(sVar + ".NumKeyFrames",0);    
	if (uiKeyFrames>0)
    {
		for (i=0;i<uiKeyFrames;i++)
		{
			CFEString sIVar = sVar + CFEString(".KeyFrame") + CFEString(i);

			CFEColor oColor;
			oColor.r = _oConfigFile.rGetReal(sIVar + ".r",_1r);
			oColor.g = _oConfigFile.rGetReal(sIVar + ".g",_1r);
			oColor.b = _oConfigFile.rGetReal(sIVar + ".b",_1r);
			oColor.a = _oConfigFile.rGetReal(sIVar + ".a",_1r);

			rTime = _oConfigFile.rGetReal(sIVar + ".Time",_0r);
			CFEString sLerpFunc = _oConfigFile.sGetString(sIVar + ".LerpFunc","linear");

			poNodeAction->m_oColorFunc.InsertKeyFrame(oColor,rTime,eGetLerpFunc(sLerpFunc) );
		}
	}
	else
		poNodeAction->m_oColorFunc.InsertKeyFrame(CFEColor(1,1,1,1),_0r,KFLF_CONSTANT);

	// Associate the node index with the action.
    poNodeAction->SetNodeIdx(iNodeIdx);

	// Retrieve the action to be played on this node (only for sprites)
	CFESkelAnimNodeTypeGetter oTypeGetter;
	TSkelAnimNodeType eType = oTypeGetter.eGetType(_poAnim->m_oNodeTab[iNodeIdx]);

	if (eType == SANT_SPRITE)
	{
		CFEString sNodeAction = _oConfigFile.sGetString(_sPrefix + ".NodeAction","none");
		CFESkelAnimSpriteModel* poSpr = (CFESkelAnimSpriteModel*)_poAnim->m_oNodeTab[iNodeIdx];

		if ((poSpr != NULL) && (poSpr->sGetSprite()!= ""))
		{
		    CFESprite* poSprite = CFESpriteMgr::poLoad(poSpr->sGetSprite());
			int iNodeActionIdx = poSprite->iGetActionIdx( sNodeAction );
			poNodeAction->SetNodeActionIdx(iNodeActionIdx);
		}
		else
			poNodeAction->SetNodeActionIdx(-1);
	}
	else
		poNodeAction->SetNodeActionIdx(-1);

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
