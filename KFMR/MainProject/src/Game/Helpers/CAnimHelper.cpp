//-----------------------------------------------------------------------------
#include "FuetEngineExt.h"
#include "Game/CGameGlobals.h"
#include "CAnimHelper.h"
//-----------------------------------------------------------------------------
CFESkelAnimActionSet*       CAnimHelper::m_poGlobalActionSet = NULL;
CFESkelAnim*                CAnimHelper::m_poGlobalAnim = NULL;
CAnimHelper::TActionData    CAnimHelper::m_oActionDataTable[ACTION_NUM_ACTIONS];
//-----------------------------------------------------------------------------
void CAnimHelper::Init(ECharacterAction _eActions[])
{
    // load skeleton.
    CFESkelAnimNode* poSkeleton = CFESkelAnimLoader::poLoadNode("data/actors/actor/actor");

    // create the head group
	CFESkelAnimNode* poHeadGroup = CFESkelAnimLoader::poLoadNode("data/actors/actor/face");
	CAnimHelper::SubstNode(poSkeleton,"B_neck","neck",poHeadGroup);

    // create skel anim object.
    m_poGlobalAnim = new CFESkelAnim("Actor");
    m_poGlobalAnim->SetAnimNode(poSkeleton);
    CFESkelAnimLoader::BuildActionNodeTable(m_poGlobalAnim);

    // build action set
    CreateActionSet(_eActions);
 
    m_poGlobalAnim->SetActionSet(m_poGlobalActionSet);
    CFESkelAnimMgr::Register(m_poGlobalAnim,"actor");    
}
//-----------------------------------------------------------------------------
void CAnimHelper::Finish()
{
    memset(m_oActionDataTable,0,sizeof(m_oActionDataTable));

    delete m_poGlobalActionSet;
    m_poGlobalActionSet= NULL;

    delete m_poGlobalAnim;
    m_poGlobalAnim = NULL;
}
//-----------------------------------------------------------------------------
void CAnimHelper::Play(FEHandler _hActorInst,ECharacterAction _eAction)
{
    if (m_oActionDataTable[_eAction].m_uiNumAnims > 0)
    {
        uint uiAnimIdx = CFEMath::iRand(0,m_oActionDataTable[_eAction].m_uiNumAnims - 1 );
        CFESkelAnimInstMgr::SetAction(_hActorInst, m_oActionDataTable[_eAction].m_uiAnimIdxs[uiAnimIdx] );
    }
}
///-----------------------------------------------------------------------------
CFESkelAnimSprite* CAnimHelper::poSubstNodeSprite(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sSprite,uint _uiAction)
{
	CFESkelAnimSprite* poNode = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(CFESkelAnimInstMgr::poGetInstancedNode(_hAnimInst),_sNodeName);
	if (poNode != NULL)
	{
		FEHandler hInst = CFESpriteInstMgr::hGetInstance(_sSprite);
		if (hInst != NULL)
		{
			CFESpriteInstMgr::ManageRender(hInst,false);
			CFESpriteInstMgr::Enable(hInst);

			poNode->SetSprite( hInst );
			poNode->SetAction(_uiAction);
		}
	}

	return(poNode);
}
///-----------------------------------------------------------------------------
CFESkelAnimMesh* CAnimHelper::poSubstNodeMesh(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sMaterial)
{
	CFESkelAnimMesh* poNode = (CFESkelAnimMesh*)CFESkelAnimNodeLocator::poLocateNode(CFESkelAnimInstMgr::poGetInstancedNode(_hAnimInst),_sNodeName);
	if (poNode != NULL)
	{
		FEHandler hMaterial = CFEMaterialMgr::hLoad(_sMaterial);
		
		/*
		if (hMaterial != NULL)
		    poNode->SetMaterial(hMaterial);
		*/
	}

	return(poNode);
}
///-----------------------------------------------------------------------------
void CAnimHelper::SubstNode(FEHandler _hAnimInst,const CFEString& _sParentNodeName,const CFEString& _sNodeName,CFESkelAnimNode* _poNode)
{
    SubstNode(CFESkelAnimInstMgr::poGetInstancedNode(_hAnimInst),_sParentNodeName,_sNodeName,_poNode);
}
///-----------------------------------------------------------------------------
void CAnimHelper::SubstNode(CFESkelAnimNode* _poRoot,const CFEString& _sParentNodeName,const CFEString& _sNodeName,CFESkelAnimNode* _poNode)
{	
	CFESkelAnimBone* poParentNode = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode(_poRoot,_sParentNodeName);

	if (poParentNode != NULL)
	    poParentNode->SetAttachedNode(_poNode);
    else
    {
        goGameGlobals.m_oLogger.Warning("Unable to find %s node",_sParentNodeName);
    }
}
//-----------------------------------------------------------------------------
/// Create an instance of a given actor
FEHandler CAnimHelper::hGetReskinedActorInstance(const CFEString& _sActorSkin) 
{
	// create the head group
    FEHandler hAnimInst = CFESkelAnimInstMgr::hGetInstance("actor");
    
    CFEString sDataPath = CFEString("data/actors/") + _sActorSkin;
    
    // Sprited parts
	CAnimHelper::poSubstNodeSprite(hAnimInst,"head",sDataPath+"/head");
	CAnimHelper::poSubstNodeSprite(hAnimInst,"face",sDataPath+"/face");
	CAnimHelper::poSubstNodeSprite(hAnimInst,"hip",sDataPath+"/body");

	CAnimHelper::poSubstNodeSprite(hAnimInst,"rhand",sDataPath+"/hand_right");
	CAnimHelper::poSubstNodeSprite(hAnimInst,"lhand",sDataPath+"/hand_left");
	CAnimHelper::poSubstNodeSprite(hAnimInst,"rfoot",sDataPath+"/foot_right");
	CAnimHelper::poSubstNodeSprite(hAnimInst,"lfoot",sDataPath+"/foot_left");

    CAnimHelper::poSubstNodeMesh(hAnimInst,"m_lleg",sDataPath+"/leg_left");
    CAnimHelper::poSubstNodeMesh(hAnimInst,"m_rleg",sDataPath+"/leg_right");   
    CAnimHelper::poSubstNodeMesh(hAnimInst,"m_larm",sDataPath+"/arm_left");
    CAnimHelper::poSubstNodeMesh(hAnimInst,"m_rarm",sDataPath+"/arm_right");

	return(hAnimInst);
}
//-----------------------------------------------------------------------------
void CAnimHelper::CreateActor(const CFEString& _sActorName) 
{
    CFEString sActor = CFEString("data/actors/") + _sActorName + CFEString("/actor");
    CFEString sFace = CFEString("data/actors/") + _sActorName + CFEString("/face");

    // load skeleton.
    CFESkelAnimNode* poSkeleton = CFESkelAnimLoader::poLoadNode(sActor);
    if (poSkeleton == NULL)
        goGameGlobals.m_oLogger.Warning("Unable to load %s",sActor);

 	// create the head group
    CFESkelAnimNode* poHeadGroup = CFESkelAnimLoader::poLoadNode(sFace);
    if (poHeadGroup == NULL)
        goGameGlobals.m_oLogger.Warning("Unable to load %s",sFace);

	CAnimHelper::SubstNode(poSkeleton,"B_neck","neck",poHeadGroup);

	// create skel anim object.
    CFESkelAnim* poSkelAnim = new CFESkelAnim(_sActorName);
    poSkelAnim->SetAnimNode(poSkeleton);
    CFESkelAnimLoader::BuildActionNodeTable(poSkelAnim);

    /// create the action set
    poSkelAnim->SetActionSet(m_poGlobalActionSet);

    // Register newly created actor
	CFESkelAnimMgr::Register(poSkelAnim, _sActorName);
}
//-----------------------------------------------------------------------------
void CAnimHelper::CreateActionSet(ECharacterAction _eActions[])
{
    m_poGlobalActionSet = new CFESkelAnimActionSet;
    
    uint i = 0;
    while (_eActions[i] != ACTION_NONE)
    {
        ECharacterAction eAction = _eActions[i];
        uint uiActionIdx = (uint)eAction;

        // setup first the first name.
        CFEString sAction = CFEString("data/anims/") + ACTION_NAMES[ uiActionIdx];
        CFESkelAnimAction* poAnimAction = NULL;        

        // try loading one by one.
        m_oActionDataTable[uiActionIdx].m_uiNumAnims = 0;
        uint j = 0;
        while (poAnimAction = CFESkelAnimLoader::poLoadAction(sAction, m_poGlobalAnim))
        {
            m_oActionDataTable[uiActionIdx].m_uiAnimIdxs[j] = m_poGlobalActionSet->uiAddAction( poAnimAction );
            m_oActionDataTable[uiActionIdx].m_uiNumAnims++;
            j++;

            sAction.Format("data/anims/%s_%d",ACTION_NAMES[ uiActionIdx],j);
        }

        i++;
    }
}
//-----------------------------------------------------------------------------
