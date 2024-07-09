//---------------------------------------------------------------------------
//
// Name:        CSkeleton.h
// Author:      David Márquez de la Cruz
// Created:     7/10/2010
// Description: CSkeleton class declaration
//
// BJointAction always affects the bone in the CBone structure were both are found.
// PJointAction sometimes affect the parent of the bone and sometimes affects the m_poParentJoint object, depending on AffectParent value.
//---------------------------------------------------------------------------
#include "CSkeleton.h"
#include "CSkeletonUtils.h"
#include "CActorEditorGUIFileOps.h"
//---------------------------------------------------------------------------
uint CSkeleton::m_uiEditMode = 0;
//-----------------------------------------------------------------------------
class CMeshModelResolver : public CFESkelAnimNodeVisitor
{
    public:
		
		static void Resolve(CSkeleton* _poSkeleton)
		{
			CMeshModelResolver oR;
			oR.m_poSkeleton = _poSkeleton;

			_poSkeleton->m_oBone[0].m_poBone->Accept(&oR);
		}

		CMeshModelResolver() : m_poNewNode(NULL), m_poSkeleton(NULL)
		{
		}

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
		{
			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				CFESkelAnimNode* poNode = _poObj->poGetChild(i);
				
				if (poNode != NULL)
				{
					poNode->Accept(this);
				
					if (m_poNewNode != NULL)
					{
						delete poNode;
						_poObj->SetChild(i,m_poNewNode);	
						m_poNewNode = NULL;
					}
				}
			}
		}

        virtual void Visit(CFESkelAnimMeshModel* _poObj)
        {
			CFESkelAnimMesh* poMesh = new CFESkelAnimMesh(_poObj->sGetName());
			
			// Copy common properties
			poMesh->SetPos	( _poObj->oGetPos() );
			poMesh->SetAngle( _poObj->rGetAngle() );

			#ifdef USE_SKELANIM_SCALE_COLOR
			poMesh->SetScale( _poObj->oGetScale() );
			poMesh->SetColor( _poObj->oGetColor() );
			#endif

			poMesh->Show	( _poObj->bIsVisible() );
			poMesh->SetDepth( _poObj->rGetDepth() );
	
			// Setup bone info
			for (uint i=0;i<_poObj->uiGetNumAttachedBones();i++)
			{
				CFEString sBoneName = _poObj->sGetAttachedBone(i);
				CFESkelAnimBone* poBone = NULL;
				
				for (uint b=0;b<m_poSkeleton->m_oBone.size();b++)
				{
					if (m_poSkeleton->m_oBone[b].m_poBone->sGetName() == sBoneName)
					{
						poBone = m_poSkeleton->m_oBone[b].m_poBone;
						break;
					}
				}

				// Attach always, NULL or not.
				poMesh->AttachBone(poBone);
			}

			// Sprite info
			FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance( _poObj->sGetSprite() );
			if (hInst != NULL)
			{
				CFESpriteInstMgr::I()->ManageRender(hInst,false);
				poMesh->SetSprite( hInst );
				poMesh->SetAction( _poObj->uiGetSpriteAction() );
			}

			m_poNewNode = poMesh;
        }        
	
	public:

		CFESkelAnimNode*	m_poNewNode;
		CSkeleton*			m_poSkeleton;
};

//---------------------------------------------------------------------------
CSkeleton::~CSkeleton()
{
	CFESkelAnimMgr::I()->ReleaseResource(m_poAnim);
    CFESkelAnimInstMgr::I()->ReleaseInstance(m_hInst);
    
    // delete m_poAnim;
}
//---------------------------------------------------------------------------
void CSkeleton::ResolveMeshModels()
{
	CMeshModelResolver::Resolve(this);
}
//---------------------------------------------------------------------------
void CSkeleton::ResolveParents()
{
	// For each bone ...
	for (uint i=0;i<m_oBone.size();i++)
	{
		int iParentIdx = -1;
		
		// find parent...
		if (m_oBone[i].m_sParentName != "")
		{
			for (uint j=0;j<m_oBone.size();j++)
			{
				if (m_oBone[j].m_poBone->sGetName() == m_oBone[i].m_sParentName)
				{
					iParentIdx = j;
					break;
				}
			}
		}
		
		if (iParentIdx != -1)
		{
			m_oBone[i].m_iParent = iParentIdx;
			m_oBone[iParentIdx].m_iChild.push_back(i);

			// add this node to the group and also manage orientation action
			if (m_oBone[i].m_bAffectParent)
			{
				m_oBone[iParentIdx].m_poBone->uiAddChild( m_oBone[i].m_poBone );
				m_oBone[i].m_poPJointAction = m_oBone[iParentIdx].m_poBoneAction;
			}
			else
			{
				m_oBone[i].m_poPJointAction  = new CFESkelAnimNodeAction();
				m_oBone[iParentIdx].m_poBone->uiAddChild( m_oBone[i].m_poParentJoint );
			}
		}
		else
		{
			m_oBone[i].m_poPJointAction  = new CFESkelAnimNodeAction();
		}
		
		m_oBone[i].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(0,0.0,KFLF_LERP);
		m_oBone[i].m_poPJointAction->SetNodeActionIdx(-1);
		m_oBone[i].m_poPJointAction->SetNodeIdx(-1);
	}
}
//---------------------------------------------------------------------------
uint CSkeleton::uiCreateBone(const CFEString& _sBoneName,const CFEString& _sParentName,CFESkelAnimNode* _poChild,bool _bAffectParent)
{
	CBone oBone;
	m_oBone.push_back(oBone);
	uint uiBoneIdx = m_oBone.size()-1;

    // First node actions
    m_oBone[uiBoneIdx].m_poBoneAction    = new CFESkelAnimNodeAction();   
    m_oBone[uiBoneIdx].m_poBJointAction  = m_oBone[uiBoneIdx].m_poBoneAction;

	// setup first frame
	m_oBone[uiBoneIdx].m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),0,KFLF_LERP);
    m_oBone[uiBoneIdx].m_poBJointAction->SetNodeActionIdx(-1);
    m_oBone[uiBoneIdx].m_poBJointAction->SetNodeIdx(-1);

	m_oBone[uiBoneIdx].m_bAffectParent  = _bAffectParent;
    m_oBone[uiBoneIdx].m_iParent		= -1;
	m_oBone[uiBoneIdx].m_sParentName	= _sParentName;
	m_oBone[uiBoneIdx].m_poLeafChild	= _poChild;

	m_oBone[uiBoneIdx].m_poBone			= new CFESkelAnimBone(_sBoneName);

	if (_bAffectParent)
	{
		m_oBone[uiBoneIdx].m_poParentJoint  = NULL;
	}
	else
	{
		m_oBone[uiBoneIdx].m_poParentJoint	= new CFESkelAnimGroup(CFEString("pj_")+_sBoneName);
		m_oBone[uiBoneIdx].m_poParentJoint->uiAddChild(m_oBone[uiBoneIdx].m_poBone);
	}

	if (_poChild != NULL)
		m_oBone[uiBoneIdx].m_poBone->uiAddChild(_poChild);

	return(uiBoneIdx);
}
//---------------------------------------------------------------------------
CFESkelAnimNode* CSkeleton::poReadNode(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFESkelAnimNode* poNode = NULL;

	CFEString sType   = _oCfg.sGetString(_sPrefix+".Type","");
	CFEString sName   = _oCfg.sGetString(_sPrefix+".Name","");

	CFEColor oColor;
	CFEVect2 oPos;
	CFEVect2 oScale;
	FEReal	rAngle;
	FEReal	rDepth;
	bool bVisible;

	// ------------------------------------------------------------------------
	// Read properties
	// ------------------------------------------------------------------------
	rDepth		= _oCfg.rGetReal(_sPrefix+".Depth",DEF_DEPTH);
	rAngle		= _oCfg.rGetReal(_sPrefix+".Angle",DEF_ANGLE);
	oPos.x		= _oCfg.rGetReal(_sPrefix+".Position.x",DEF_POS_X);
	oPos.y		= _oCfg.rGetReal(_sPrefix+".Position.y",DEF_POS_Y);
	
	#ifdef USE_SKELANIM_SCALE_COLOR
	oScale.x	= _oCfg.rGetReal(_sPrefix+".Scale.x",DEF_SCALE_X);
	oScale.y	= _oCfg.rGetReal(_sPrefix+".Scale.y",DEF_SCALE_Y);
	oColor.r	= _oCfg.rGetReal(_sPrefix+".Color.r",_1r);
	oColor.g	= _oCfg.rGetReal(_sPrefix+".Color.g",_1r);
	oColor.b	= _oCfg.rGetReal(_sPrefix+".Color.b",_1r);
	oColor.a	= _oCfg.rGetReal(_sPrefix+".Color.a",_1r);
	#endif

	bVisible	= _oCfg.bGetBool(_sPrefix+".Visible",DEF_VISIBILITY);

	if (sType |= "Group")
	{
		int iNumChilds    = _oCfg.iGetInteger(_sPrefix+".NumChildren",0);
		CFESkelAnimGroup* poGroup = new CFESkelAnimGroup(sName);
		for (uint i=0;i<iNumChilds;i++)
		{
			CFEString sNewPrefix = _sPrefix+CFEString(".Child")+CFEString((int)i);
			CFESkelAnimNode* poChild = poReadNode(_oCfg,sNewPrefix);
		
			poGroup->uiAddChild(poChild);
		}
		
		poNode = poGroup;
	}
	else if (sType |= "Sprite")
	{
		CFESkelAnimSprite* poSprite = new CFESkelAnimSprite(sName);
		CFEString sSpriteName		= _oCfg.sGetString(_sPrefix+".Sprite","");
		int iAction					= _oCfg.iGetInteger(_sPrefix+".SpriteAction",-1);
		
		poSprite->SetSprite(CFESpriteInstMgr::I()->hGetInstance(sSpriteName));
		poSprite->SetAction(iAction);

		poNode = poSprite;
	}
	else if (sType |= "Mesh")
	{
		CFESkelAnimMeshModel* poMesh = new CFESkelAnimMeshModel(sName);
		CFEString sSpriteName	= _oCfg.sGetString(_sPrefix+".Sprite","");
		int iAction					= _oCfg.iGetInteger(_sPrefix+".SpriteAction",-1);
		
		poMesh->SetSprite(sSpriteName);
		poMesh->SetSpriteAction(iAction);
		
		uint uiAttachedBones = _oCfg.iGetInteger(_sPrefix+".NumAttachedBones",0);
		for (uint i=0;i<uiAttachedBones;i++)
		{
			CFEString sBone = _oCfg.sGetString(_sPrefix+".AttachedBone"+CFEString((int)i),"");
			poMesh->AttachBone(sBone);
		}

		poNode = poMesh;
	}
	// ------------------------------------------------------------------------
	// Set properties
	// ------------------------------------------------------------------------
	if (poNode != NULL)
	{
		poNode->SetPos(oPos);
		poNode->SetAngle(rAngle);
		poNode->SetDepth(rDepth);
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		poNode->SetColor(oColor);
		poNode->SetScale(oScale);
		#endif

		poNode->Show(bVisible);
	}

	return(poNode);
}
//---------------------------------------------------------------------------
void CSkeleton::ReadSkeletonDefinition(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	uint uiNumBones = _oCfg.iGetInteger(_sPrefix+"SkeletonDefinition.NumBones",0);
	for (uint i=0;i<uiNumBones;i++)
	{
		CFEString sVar    = CFEString(_sPrefix+"SkeletonDefinition.Bone") + CFEString((int)i);
		CFEString sName   = _oCfg.sGetString(sVar+".Name","");

		if (sName != "")
		{
			bool bAffectParent = _oCfg.bGetBool(sVar+".AffectParent",true);
			CFEString sParent	= _oCfg.sGetString(sVar+".Parent","");

			CFESkelAnimNode* poChild = poReadNode(_oCfg,sVar+".Child");
			unsigned int uiIdx = uiCreateBone(sName, sParent, poChild, bAffectParent);

			CFESkelAnimBone* poBone = m_oBone[uiIdx].m_poBone;
			
			// ------------------------------------------------------------------------
			// Read properties
			// ------------------------------------------------------------------------
			FEReal	rAngle		= _oCfg.rGetReal(sVar+".Angle",DEF_ANGLE);
			FEReal	rDepth		= _oCfg.rGetReal(sVar+".Depth",DEF_DEPTH);

			CFEVect2 oPos;
			oPos.x = _oCfg.rGetReal(sVar+".Position.x",DEF_POS_X);
			oPos.y = _oCfg.rGetReal(sVar+".Position.y",DEF_POS_Y);

			#ifdef USE_SKELANIM_SCALE_COLOR
			CFEVect2 oScale;
			oScale.x = _oCfg.rGetReal(sVar+".Scale.x",DEF_SCALE_X);
			oScale.y = _oCfg.rGetReal(sVar+".Scale.y",DEF_SCALE_Y);

			CFEColor oColor;
			oColor.r = _oCfg.rGetReal(sVar+".Color.r",_1r);
			oColor.g = _oCfg.rGetReal(sVar+".Color.g",_1r);
			oColor.b = _oCfg.rGetReal(sVar+".Color.b",_1r);
			oColor.a = _oCfg.rGetReal(sVar+".Color.a",_1r);
			#endif
			
			bool bVisible = _oCfg.bGetBool(sVar+".Visible",DEF_VISIBILITY);

			// ------------------------------------------------------------------------
			// Set properties
			// ------------------------------------------------------------------------
			poBone->SetBonePos(oPos);
			poBone->SetPos(CFEVect2::ZERO());

			poBone->SetAngle(rAngle);
			poBone->SetDepth(rDepth);
			

			#ifdef USE_SKELANIM_SCALE_COLOR
			poBone->SetColor(oColor);
			poBone->SetScale(oScale);
			#endif
			
			poBone->Show(bVisible);
		}
	}

	ResolveParents();
	ResolveMeshModels();
}
//---------------------------------------------------------------------------
void CSkeleton::BuildStructure()
{
	// get pivot action
	m_poPivot = m_oBone[m_uiPivotIdx].m_poBoneAction;

    // create skel anim object.
    char szName[64];
    sprintf(szName,"actor - 0x%d",(uint)this);

    m_poAnim = new CFESkelAnim(szName);
    m_poAnim->SetAnimNode(m_oBone[m_uiPivotIdx].m_poBone);
    CFESkelAnimLoader::BuildActionNodeTable(m_poAnim);

    CFESkelAnimActionSet* poActionSet = new CFESkelAnimActionSet;
    m_poAnim->SetActionSet(poActionSet);

    CFESkelAnimAction* poAction = new CFESkelAnimAction("action");

    for (uint i=0;i<m_oBone.size();i++)
    {
		// add position action
		for (uint j=0;j<m_poAnim->m_oNodeTab.size();j++)
		{
			if (m_oBone[i].m_poBone == m_poAnim->m_oNodeTab[j])
			{
				// now set the index in the animation node table in the node action of the bone:
				// This means: This node action affects the node with j index inside the animation node table.
				m_oBone[i].m_poBJointAction->SetNodeIdx(j);

				// add the node action to the action object.
				poAction->uiAddNodeAction(m_oBone[i].m_poBJointAction);
				break;
			}
		}

		// add angle action
		for (uint j=0;j<m_poAnim->m_oNodeTab.size();j++)
		{
			if (m_oBone[i].m_poParentJoint != NULL)
			{
				if (m_oBone[i].m_poParentJoint == m_poAnim->m_oNodeTab[j])
				{
					// now set the index in the animation node table in the node action of the bone:
					// This means: This node action affects the node with j index inside the animation node table.
					m_oBone[i].m_poPJointAction->SetNodeIdx(j);

					// add the node action to the action object.
					poAction->uiAddNodeAction(m_oBone[i].m_poPJointAction);
					break;
				}
			}
			//
			// else
			//{
			//      this action should already have been inserted in the "add position action" loop.
			//}
		}
	}

    // Add action to action set.
    poActionSet->uiAddAction(new CFESkelAnimAction("skeleton_action"));
    poActionSet->uiAddAction(poAction);

    CFESkelAnimMgr::I()->Register(m_poAnim,szName);
    m_hInst = CFESkelAnimInstMgr::I()->hGetInstance(szName);
    CFESkelAnimInstMgr::I()->Enable(m_hInst);
}
//---------------------------------------------------------------------------
void CSkeleton::Init(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	/// intialize with default values.
	m_oBone.clear();
	m_bAnimPivot = false;
	m_poPivot = NULL;
	m_uiPivotIdx = 0;
	m_poAnim = NULL;
    m_hInst = NULL;

    /// Pivot bone should be created here...
	ReadSkeletonDefinition(_oCfg,_sPrefix);
	if (m_oBone.size() == 0) return;

	// find pivot
	int iPivotIdx = -1;
	for (uint i=0;i<m_oBone.size();i++)
	{
		if (m_oBone[i].sGetName() |= "PIVOT")
		{
			iPivotIdx = i;
			break;
		}
	}
	
	if (iPivotIdx == -1)
	{	
		iPivotIdx = uiCreateBone("PIVOT","",m_oBone[0].m_poBone,true);
	}

	m_uiPivotIdx = iPivotIdx;
	BuildStructure();
}
//---------------------------------------------------------------------------
CSkeleton::CSkeleton(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	Init(_oCfg,_sPrefix);
}
//----------------------------------------------------------------------------
CSkeleton::CSkeleton(const CFEString& _sFilename) :

	m_bAnimPivot(false)
	,m_poPivot(NULL)
	,m_uiPivotIdx(0)
	,m_poAnim(NULL)
	,m_hInst(NULL)
{
    CFEConfigFile oCfg(_sFilename);
	if (oCfg.bInitialized())
	{
		Init(oCfg,"");
	}
	else
	{
		// ?????
	}
}
//---------------------------------------------------------------------------
CSkeleton::CSkeleton(CSkeleton * _poSkeleton)
{
	m_uiPivotIdx = _poSkeleton->m_uiPivotIdx;

	// Create structure
	for (uint i=0;i<_poSkeleton->m_oBone.size();i++)
	{
		CFESkelAnimNode* poChild = CFESkelAnimNodeInstancer::poCreateInstance( _poSkeleton->m_oBone[i].m_poLeafChild );
		unsigned int uiIdx = uiCreateBone(	_poSkeleton->m_oBone[i].sGetName(),
											_poSkeleton->m_oBone[i].m_sParentName, 
											poChild, 
											_poSkeleton->m_oBone[i].m_bAffectParent);
	}

	ResolveParents();
	BuildStructure();

	// Build animation data.
	for (uint i=0;i<_poSkeleton->m_oBone.size();i++)
	{
		m_oBone[i].m_poBJointAction->m_oPosFunc.SetWrapMode( _poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.eGetWrapMode() );
		for (uint k=0;k<_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<CFEVect2>* poKF = _poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.poGetKeyFrameData(k);
			m_oBone[i].m_poBJointAction->m_oPosFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}

		m_oBone[i].m_poPJointAction->m_rAngleFunc.SetWrapMode( _poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.eGetWrapMode() );
		for (uint k=0;k<_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();k++)
		{
			CFETypedKeyFrame<FEReal>* poKF = _poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.poGetKeyFrameData(k);
			m_oBone[i].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(poKF->m_oKey,poKF->rGetKeyTime(),poKF->eGetLerpFunc());
		}
	}

	// Copy data from original to destination.
	CloneNodes(m_oBone[m_uiPivotIdx].m_poBone,_poSkeleton->m_oBone[m_uiPivotIdx].m_poBone);
	CloneNodes(CFESkelAnimInstMgr::I()->poGetInstancedNode(m_hInst),CFESkelAnimInstMgr::I()->poGetInstancedNode(_poSkeleton->m_hInst));
}
//----------------------------------------------------------------------------
void CSkeleton::OrientBone(uint _iBoneIdx, float _fAngle, float _fTime)
{
    // modify also linked bone (orientation and scale)
    /*
    CFEVect2 oBonePos		= CAbsPosGetter::oGetPos(m_oBone[_iBoneIdx].sGetName(),_fTime,m_hInst);
    CFEVect2 oOrientToPos	= CAbsPosGetter::oGetPos(m_oBone[_iOrientToBoneIdx].sGetName(),_fTime,m_hInst);
    CFEVect2 oD = oOrientToPos - oBonePos;

    // orientation
    EFEKFLerpFunc eLerpFunc = KFLF_LERP;
	CFEKeyFrame* poKF = m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.poGetKeyFrameAtTime(_fTime);
	if (poKF != NULL) eLerpFunc = poKF->eGetLerpFunc();

    float fAngle = atan2(oD.y,oD.x);
    fAngle += (_2PIr + _3PI2r);
    m_oBone[_iBoneIdx].m_poBone->SetAngle(fAngle);

	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(m_hInst);
	CFESkelAnim*     poAnim = CFESkelAnimInstMgr::I()->poGetAnim(m_hInst);

    CFESkelAnimBone* poBone = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode( poNode, m_oBone[_iBoneIdx].sGetName() );
    if (poBone != NULL)
		poBone->SetAngle(fAngle);
	*/

    // first of all insert it
    EFEKFLerpFunc eLerpFunc = KFLF_LERP;
	CFETypedKeyFrame<float>* poKF = m_oBone[_iBoneIdx].m_poPJointAction->m_rAngleFunc.poGetKeyFrameDataAtTime(_fTime);
	if (poKF != NULL) eLerpFunc = poKF->eGetLerpFunc();

	m_oBone[_iBoneIdx].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(_fAngle,_fTime,eLerpFunc);

	// then fix it (in case...) : May require fix propagation hence the full loop
    for (uint i=0;i<m_oBone[_iBoneIdx].m_poPJointAction->m_rAngleFunc.uiGetNumKeyFrames();i++)
    {
        poKF = m_oBone[_iBoneIdx].m_poPJointAction->m_rAngleFunc.poGetKeyFrameData(i);
        FEReal fAngle = poKF->m_oKey;

		if ( i == 0)
		{
		}
		else
		{
			CFETypedKeyFrame<float>* poPrevKF = m_oBone[_iBoneIdx].m_poPJointAction->m_rAngleFunc.poGetKeyFrameData(i-1);
			FEReal rPrevAngle = poPrevKF->m_oKey;

			// Avoid strange spins
			if (CFEMath::rAbs(rPrevAngle - fAngle) > _PIr)
			{
				FEReal rA = CFEMath::rAbs(rPrevAngle - (fAngle + _2PIr));
				FEReal rB = CFEMath::rAbs(rPrevAngle - (fAngle - _2PIr));

				if (rA < rB)
					fAngle = rA;
				else
					fAngle = rB;
			}

			poKF->m_oKey = fAngle;
		}
    }

    CFESkelAnimInstMgr::I()->Update(m_hInst,_0r);
}
//----------------------------------------------------------------------------
// Positions are only used to handle the skeleton. Only the initial position of the bone
// is affected. Bones have no actions attached to them but it child node has the action attached.
//----------------------------------------------------------------------------
void CSkeleton::MoveBoneTo(uint _uiBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle,uint _uiEditMode)
{
	// pivot only allowed to animate its position
	if ((_uiEditMode == 1) && (_uiBoneIdx == m_uiPivotIdx)) _uiEditMode = 2;

	// update instance just in case.
	CFESkelAnimInstMgr::I()->Update(m_hInst,_0r);

	switch (_uiEditMode)
	{
		// skeleton
		case 0:
		MoveSkeletonBoneTo(_uiBoneIdx,_oNewPos);
		break;

		// bone orientation
		case 1:
		OrientBoneFromTo(_uiBoneIdx,_fTime,_oNewPos,_oOldPos,_oOldParentPos,_fOldAngle);
		break;

		// bone position
		case 2:
		MoveBoneFromTo(_uiBoneIdx,_fTime,_oNewPos,_oOldPos,_oOldParentPos,_fOldAngle);
		break;
	}

	// Recalc action times
	CFESkelAnimAction* poAction = m_poAnim->poGetAnimActionSet()->poGetAction(0);
	poAction->SetActionTime( CFESkelAnimUtils::rGetActionTime(m_poAnim, poAction));
	poAction->SetMaxActionTime( CFESkelAnimUtils::rGetMaxActionTime(m_poAnim, poAction));
	
	// Setup current action (Why should we do this here!?!?!?)
	CFESkelAnimInstMgr::I()->SetAction( m_hInst, m_uiEditMode );	// WARNING: this puts the animation instance to the 0 sec pose
	CFESkelAnimInstMgr::I()->SetTime(m_hInst,_fTime);
}
//---------------------------------------------------------------------------
void CSkeleton::MoveSkeletonBoneTo(uint _uiBoneIdx,const CFEVect2& _oNewPos)
{
    // setup new bone position
    // CFEVect2 oOldPos   = CLocalPosGetter::oGetPos( CAbsPosGetter::oGetPos(m_oBone[_uiBoneIdx].m_poBoneJoint->sGetName(),_fTime,m_hInst),m_oBone[_uiBoneIdx].m_poBoneJoint->sGetName(),_fTime,m_hInst);
    CFEVect2 oLocalPos = CBoneLocalPosGetter::oGetPos(_oNewPos,m_oBone[m_uiPivotIdx].m_poBone,m_oBone[_uiBoneIdx].m_poBone->sGetName());
    m_oBone[_uiBoneIdx].m_poBone->SetBonePos(oLocalPos);
    
    // modify also the instanced bone.
	CFESkelAnimBone* poBone = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::I()->poGetInstancedNode(m_hInst), m_oBone[_uiBoneIdx].m_poBone->sGetName());
	if (poBone != NULL)
		poBone->SetBonePos( oLocalPos );
}
//---------------------------------------------------------------------------
void CSkeleton::MoveBoneFromTo(uint _uiBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle)
{
	EFEKFLerpFunc eLerpFunc = KFLF_LERP;
	CFEKeyFrame* poKF = m_oBone[_uiBoneIdx].m_poBJointAction->m_oPosFunc.poGetKeyFrameAtTime(_fTime);
	if (poKF != NULL) eLerpFunc = poKF->eGetLerpFunc();

    // setup new bone position
	if (m_oBone[_uiBoneIdx].m_iParent != -1)
	{
		// we want a new position for this bone. then we should use a position relative to its parent, not to itself.
		CFEVect2 oLocalPos = CLocalPosGetter::oGetPos(_oNewPos,m_oBone[ m_oBone[_uiBoneIdx].m_iParent ].m_poBone->sGetName(),_fTime,m_hInst);

		/*
		As the bone has an offset always (from the initial hierarchy), we have to substract it from the local position.
		This offset is a displacement from the bone reference system. It's a offset local to the parent bone ref sys, and it's always constant, independently of the 
		parent bone orientation.

					4								  4/
					3|····x							 3/
					2|    ·							2/  ·
					1|    ·						   1/	   ·   
				    0o-----  offset = (5,3) ->    0o			 x    offset = (5,3)
					 012345						   1--		·	
													 23--  ·
													    45-
		
		Then, it is correct to substract this offset to a coordinate local to the parent ref sys.
		*/
		
		oLocalPos -= m_oBone[_uiBoneIdx].m_poBone->oGetBonePos();
		
		/*
		CFESystem::Print("(%.3f,%.3f) <- (%.3f,%.3f)\n",oLocalPos.x,oLocalPos.y,_oNewPos.x,_oNewPos.y);
		static CFEVect2 oOldNewPos;
		static CFEVect2 oOldLocalPosFromStack;
		CFEVect2 oOldLocalPosFromPosFunc = m_oBone[_uiBoneIdx].m_poBJointAction->m_oPosFunc.oGetValue(_fTime);
		CFEVect2 oOldLocalPosFromOldNewPos = CLocalPosGetter::oGetPos(oOldNewPos,m_oBone[ m_oBone[_uiBoneIdx].m_iParent ].m_poBone->sGetName(),_fTime,m_hInst);

		float fLocalDist = oOldLocalPosFromPosFunc.rDistance( oLocalPos );
		float fAbsDist   = oOldNewPos.rDistance( _oNewPos );

		if ( CFEMath::rAbs(fLocalDist-fAbsDist) > 1.0f)
		{
			int a = 0;
		}
		
		oOldLocalPosFromStack = oLocalPos;
		oOldNewPos = _oNewPos;
		*/

		m_oBone[_uiBoneIdx].m_poBJointAction->m_oPosFunc.InsertKeyFrame(oLocalPos,_fTime,eLerpFunc);
	}
	else
	{
		CFEVect2 oLocalPos = _oNewPos - m_oBone[_uiBoneIdx].m_poBone->oGetBonePos();

		// Bone has no parent, so world position is exactly the local position (excluding the initial bone offset)
		m_oBone[_uiBoneIdx].m_poBJointAction->m_oPosFunc.InsertKeyFrame(oLocalPos,_fTime,eLerpFunc);
	}
}
//----------------------------------------------------------------------------
void CSkeleton::OrientBoneFromTo(uint _iBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle)
{
	// miramos si el hueso tiene padre.
	if (m_oBone[_iBoneIdx].m_iParent == -1)
	{
		MoveBoneFromTo(_iBoneIdx,_fTime,_oNewPos,_oOldPos,_oOldParentPos,_fOldAngle);
	}
	else
	{
		CFEVect2 oParentPos = _oOldParentPos;
		CFEVect2 oOldDir = _oOldPos - oParentPos;
		CFEVect2 oNewDir = _oNewPos - oParentPos;

		oOldDir.Normalize();
		oNewDir.Normalize();

		float fDot   = CFEMath::rClamp(-_1r,_1r,oOldDir.rDot(oNewDir));
		float fAngle = acosf(fDot);
		
		// get sign of rotation: Cross product (Z axis value)
		// z = _oV1.x * _oV2.y - _oV1.y * _oV2.x;
		float fSign = oOldDir.x*oNewDir.y - oOldDir.y*oNewDir.x;

		if (fSign<_0r)
			fAngle *= -1.0f;

		OrientBone(_iBoneIdx,_fOldAngle+fAngle,_fTime);
	}
}
//----------------------------------------------------------------------------
void CSkeleton::RescaleSkeleton(float _fHScale,float _fVScale)
{
	// update instance just in case.
	CFESkelAnimInstMgr::I()->Update(m_hInst,_0r);

	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(m_hInst);
	CFEVect2 oScale(_fHScale,_fVScale);

    for (uint b=0;b<m_oBone.size();b++)
	{
		CFEString sBoneName = m_oBone[b].m_poBone->sGetName();
		CFESkelAnimBone* poBone = (CFESkelAnimBone*)CFESkelAnimNodeLocator::poLocateNode( poNode, sBoneName);

		CFEVect2 oPos = m_oBone[b].m_poBone->oGetPos() * oScale;
		m_oBone[b].m_poBone->SetPos( oPos );

		if (poBone != NULL)
			poBone->SetPos( oPos );

		// m_oBone[b].m_poBone->SetScale( CFEVect2(_fHScale,_fVScale) );
		// CAbsPosGetter::oGetPos(sBoneName,
	    // CFEVect2 oLocalPos = CLocalPosGetter::oGetPos(_oNewPos,m_oBone[_iBoneIdx].m_poBone->sGetName(),_fTime,m_hInst);
		// m_oBone[_iBoneIdx].m_poBone->SetPos(oLocalPos);
	}
}
//---------------------------------------------------------------------------
CFEVect2 CSkeleton::oGetAbsBonePos(uint _uiBoneIdx,float _fTime,uint _uiEditMode)
{
	// update instance just in case.
	CFESkelAnimInstMgr::I()->Update(m_hInst,_0r);

	switch (_uiEditMode)
	{
		// skeleton
		case 0:
		return(CBoneAbsPosGetter::oGetPos(m_oBone[m_uiPivotIdx].m_poBone,m_oBone[_uiBoneIdx].sGetName()));
		break;

		// bone orientation
		case 1:
		case 2:
		return(CAbsPosGetter::oGetPos(m_oBone[_uiBoneIdx].sGetName(),_fTime,m_hInst));
		break;
	}

	return(CFEVect2::ZERO());
}
//-----------------------------------------------------------------------------
class CSkelAnimSpriteSynchronizer: public CFESkelAnimNodeVisitor
{
	public:
		static void Sync(CFESkelAnimNode* _poRoot,FEReal _rTimeFact)
		{
			CSkelAnimSpriteSynchronizer oObj(_rTimeFact);
			_poRoot->Accept(&oObj);
		}

		CSkelAnimSpriteSynchronizer(FEReal _rTimeFact)
		{
			m_rTimeFact		= _rTimeFact;
		}

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			CFESpriteInstMgr::I()->SetSpeedMult( _poObj->hGetSprite(), m_rTimeFact);				
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			CFESpriteInstMgr::I()->SetSpeedMult( _poObj->hGetSprite(), m_rTimeFact);
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
            for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

	protected:

		FEReal	m_rTimeFact;
};
//---------------------------------------------------------------------------
void CSkeleton::SynchronizeSprites(FEReal _rSpeedFact)
{
	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(m_hInst);
	CSkelAnimSpriteSynchronizer::Sync(poNode,_rSpeedFact);
}
//---------------------------------------------------------------------------
