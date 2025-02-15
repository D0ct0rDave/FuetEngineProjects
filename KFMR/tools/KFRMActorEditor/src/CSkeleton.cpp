//---------------------------------------------------------------------------
//
// Name:        CSkeleton.h
// Author:      David Márquez de la Cruz
// Created:     7/10/2010
// Description: CSkeleton class declaration
//
//---------------------------------------------------------------------------
#include "CSkeleton.h"
//---------------------------------------------------------------------------
const char* BONE_NAMES[MAX_BONES] = {
    "head",
    "neck",
    "lshoulder",
    "rshoulder",
    "lelbow",
    "relbow",
    "lhand",
    "rhand",

    "lthand",
    "rthand",

    "hip",
    "lhip",
    "rhip",
    "lknee",
    "rknee",
    "lfoot",
    "rfoot",
    "ltfoot",
    "rtfoot",
    
    "PIVOT",
};
//---------------------------------------------------------------------------
const char* SKINNED_MESHES[MAX_SKINNED_MESHES] =
{
    "m_rarm",
    "m_larm",
    "m_rleg",
    "m_lleg",
    // "m_hip"
};
//---------------------------------------------------------------------------
void CSkeleton::CreateBone(uint _uiBoneIdx,int _iParent,EBoneType _eBoneType,CFESkelAnimMesh* _poMesh)
{
    // First node action
    m_oBone[_uiBoneIdx].m_poNodeAction  = new CFESkelAnimNodeAction();
    m_oBone[_uiBoneIdx].m_poNodeAction->m_oPosFunc.InsertKeyFrame(CFEVect2(0,0),0,KFLF_LERP);
    m_oBone[_uiBoneIdx].m_poNodeAction->m_rAngleFunc.InsertKeyFrame(0,0.0,KFLF_LERP);
    m_oBone[_uiBoneIdx].m_poNodeAction->m_oScaleFunc.InsertKeyFrame(CFEVect2(1.0,1.0),0.0,KFLF_LERP);
    m_oBone[_uiBoneIdx].m_poNodeAction->m_oColorFunc.InsertKeyFrame(CFEColor(1,1,1,1),0.0,KFLF_LERP);
    m_oBone[_uiBoneIdx].m_poNodeAction->SetNodeActionIdx(-1);
    m_oBone[_uiBoneIdx].m_poNodeAction->SetNodeIdx(-1);

    m_oBone[_uiBoneIdx].m_eBoneType     = _eBoneType;
    m_oBone[_uiBoneIdx].m_iParent       = _iParent;
    
    if (_iParent != -1)
        m_oBone[_iParent].m_iChild = _uiBoneIdx;    

    switch (_eBoneType)
    {
        case BT_SPRITE:
        {
            m_oBone[_uiBoneIdx].m_poSpr = new CFESkelAnimSprite( BONE_NAMES[_uiBoneIdx] );
            m_oBone[_uiBoneIdx].m_poSpr->SetAngle(0.0);
            m_oBone[_uiBoneIdx].m_poSpr->SetScale(CFEVect2(1,1));
            m_oBone[_uiBoneIdx].m_poSpr->SetPos(CFEVect2(0,0));
            m_oBone[_uiBoneIdx].m_poSpr->SetColor(CFEColor(1,1,1,1));

            CFESkelAnimBone* poBone = new CFESkelAnimBone( CFEString("B_") + CFEString(BONE_NAMES[_uiBoneIdx]) );
            poBone->SetAttachedNode( m_oBone[_uiBoneIdx].m_poSpr );

            if (_poMesh != NULL)
            {
                m_oBone[_uiBoneIdx].m_poMesh = _poMesh;
               _poMesh->AttachBone( poBone);
            }

            m_oBone[_uiBoneIdx].m_poNode = poBone;
            m_oBone[_uiBoneIdx].m_poNode->SetAngle(0.0);
            m_oBone[_uiBoneIdx].m_poNode->SetScale(CFEVect2(1,1));
            m_oBone[_uiBoneIdx].m_poNode->SetPos(CFEVect2(0,0));
            m_oBone[_uiBoneIdx].m_poNode->SetColor(CFEColor(1,1,1,1));
        }
        break;

        case BT_BONE:
        case BT_CONTROL:
        {
            // Meshes are shared among bones.
            CFESkelAnimBone* poBone = new CFESkelAnimBone( CFEString("B_") + CFEString(BONE_NAMES[_uiBoneIdx]) );
            
            if (_poMesh != NULL)
            {
                m_oBone[_uiBoneIdx].m_poMesh = _poMesh;
               _poMesh->AttachBone( poBone);
            }

            m_oBone[_uiBoneIdx].m_poNode = poBone;
            m_oBone[_uiBoneIdx].m_poNode->SetAngle(0.0);
            m_oBone[_uiBoneIdx].m_poNode->SetScale(CFEVect2(1,1));
            m_oBone[_uiBoneIdx].m_poNode->SetPos(CFEVect2(0,0));
            m_oBone[_uiBoneIdx].m_poNode->SetColor(CFEColor(1,1,1,1));
        }
        break;
    }
}
//---------------------------------------------------------------------------
CSkeleton::CSkeleton()
{
    m_poSkel = new CFESkelAnimGroup("Skeleton");
    m_poSkel->SetAngle(0.0);
    m_poSkel->SetPos(CFEVect2(0,0));
    m_poSkel->SetScale(CFEVect2(1,1));
    m_poSkel->SetColor(CFEColor(1,1,1,1));

    /// Pivot bone.
    CreateBone(BONE_PIVOT,-1,BT_CONTROL);
    m_poPivot = m_oBone[BONE_PIVOT].m_poNodeAction;

    // Rest of the bones.
    CreateBone(BONE_HEAD,BONE_NECK,BT_CONTROL);
    CreateBone(BONE_NECK,BONE_HIP,BT_SPRITE,NULL);
    CreateBone(BONE_HIP,-1,BT_SPRITE,NULL);

    CFESkelAnimMesh* poRAMesh = new CFESkelAnimMesh(SKINNED_MESHES[0]);
    CreateBone(BONE_RSHOULDER,-1,BT_BONE,poRAMesh);
    CreateBone(BONE_RELBOW,BONE_RSHOULDER,BT_BONE,poRAMesh);
    CreateBone(BONE_RHAND,BONE_RELBOW,BT_SPRITE,poRAMesh);
    CreateBone(BONE_RTHAND,BONE_RHAND,BT_CONTROL,NULL);

    CFESkelAnimMesh* poLAMesh = new CFESkelAnimMesh(SKINNED_MESHES[1]);
    CreateBone(BONE_LSHOULDER,-1,BT_BONE,poLAMesh);
    CreateBone(BONE_LELBOW,BONE_LSHOULDER,BT_BONE,poLAMesh);
    CreateBone(BONE_LHAND,BONE_LELBOW,BT_SPRITE,poLAMesh);
    CreateBone(BONE_LTHAND,BONE_LHAND,BT_CONTROL,NULL);

    CFESkelAnimMesh* poLLMesh = new CFESkelAnimMesh(SKINNED_MESHES[3]);    
    // poLLMesh->AttachBone( (CFESkelAnimBone*)m_oBone[BONE_HIP].m_poNode );
    CreateBone(BONE_LHIP,-1,BT_BONE,poLLMesh);
    CreateBone(BONE_LKNEE,BONE_LHIP,BT_BONE,poLLMesh);
    CreateBone(BONE_LFOOT,BONE_LKNEE,BT_SPRITE,poLLMesh);
    CreateBone(BONE_LTFOOT,BONE_LFOOT,BT_CONTROL,NULL);

    CFESkelAnimMesh* poRLMesh = new CFESkelAnimMesh(SKINNED_MESHES[2]);
    // poRLMesh->AttachBone( (CFESkelAnimBone*)m_oBone[BONE_HIP].m_poNode );
    CreateBone(BONE_RHIP,-1,BT_BONE,poRLMesh);
    CreateBone(BONE_RKNEE,BONE_RHIP,BT_BONE,poRLMesh);
    CreateBone(BONE_RFOOT,BONE_RKNEE,BT_SPRITE,poRLMesh);
    CreateBone(BONE_RTFOOT,BONE_RFOOT,BT_CONTROL,NULL);

    // add sprite nodes to the group
    
    // Order is important!
    // first left part
    m_poSkel->uiAddNode( m_oBone[BONE_LHIP].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_LKNEE].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_LFOOT].m_poNode);
    m_poSkel->uiAddNode(poLLMesh);
    m_poSkel->uiAddNode( m_oBone[BONE_LFOOT].m_poNode);

    m_poSkel->uiAddNode( m_oBone[BONE_LSHOULDER].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_LELBOW].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_LHAND].m_poNode); // render bone and sprite
    m_poSkel->uiAddNode( poLAMesh );
    m_poSkel->uiAddNode( m_oBone[BONE_LHAND].m_poNode); // render bone and sprite

    // then body and head
    m_poSkel->uiAddNode( m_oBone[BONE_HIP].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_NECK].m_poNode);

    // continue to right part
    m_poSkel->uiAddNode( m_oBone[BONE_RHIP].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_RKNEE].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_RFOOT].m_poNode);
    m_poSkel->uiAddNode( poRLMesh );
    m_poSkel->uiAddNode( m_oBone[BONE_RFOOT].m_poNode);

    m_poSkel->uiAddNode( m_oBone[BONE_RSHOULDER].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_RELBOW].m_poNode);
    m_poSkel->uiAddNode( m_oBone[BONE_RHAND].m_poNode);
    m_poSkel->uiAddNode( poRAMesh );
    m_poSkel->uiAddNode( m_oBone[BONE_RHAND].m_poNode);
    
    // create skel anim object.
    char szName[64];
    sprintf(szName,"actor - 0x%d",(uint)this);

    m_poAnim = new CFESkelAnim(szName);
    m_poAnim->SetAnimNode(m_poSkel);
    CFESkelAnimLoader::BuildActionNodeTable(m_poAnim);

    CFESkelAnimActionSet* poActionSet = new CFESkelAnimActionSet;
    m_poAnim->SetActionSet(poActionSet);
    
    CFESkelAnimAction* poAction = new CFESkelAnimAction("action");

    // Pivot affects root
    // m_poPivot->SetNodeIdx(0);
    // poAction->uiAddNodeAction(m_poPivot);

    for (uint i=0;i<MAX_BONES;i++)
    {
        /// find the node of this bone inside the animation node table.
        for (uint j=0;j<m_poAnim->m_oNodeTab.size();j++)
        {
            if (m_poAnim->m_oNodeTab[j] == m_oBone[i].m_poNode)
            {
                // now set the index in the animation node table in the node action of the bone:
                // This means: This node action affects the node with j index inside the animation node table.
                m_oBone[i].m_poNodeAction->SetNodeIdx(j);

                // add the node action to the action object.
                poAction->uiAddNodeAction(m_oBone[i].m_poNodeAction);
                break;
            }
        }
    }

    // Add action to action set.
    poActionSet->uiAddAction(poAction);
    
    CFESkelAnimMgr::Register(m_poAnim,szName);
    m_hInst = CFESkelAnimInstMgr::hGetInstance(szName);
    CFESkelAnimInstMgr::Enable(m_hInst);
}
//----------------------------------------------------------------------------
void CSkeleton::OrientBone(uint _iBoneIdx, uint _iOrientToBoneIdx, float _fTime)
{
    // modify also linked bone (orientation and scale)        
    CFEVect2 oBonePos   = m_oBone[_iBoneIdx].m_poNodeAction->m_oPosFunc.oGetValue(_fTime);        
    CFEVect2 oOrientToPos = m_oBone[_iOrientToBoneIdx].m_poNodeAction->m_oPosFunc.oGetValue(_fTime);        
    CFEVect2 oD = oOrientToPos - oBonePos;

    // orientation
    oD.Normalize();
    float fAngle = atan2(oD.x,-oD.y) + _2PIr_;
    m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.InsertKeyFrame(fAngle,_fTime,KFLF_LERP);
    
    for (uint i=0;i<m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.uiGetNumKeyFrames();i++)
    {
        CFEKeyFrame* poKF = m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.poGetKeyFrame(i);
        if (CFEMath::bBetween(poKF->rGetKeyTime()-EPSILON,poKF->rGetKeyTime()+EPSILON,_fTime))
        {
            if (i == 0) return;
            
            // get previous keyframe
            poKF = m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.poGetKeyFrame(i-1);
            FEReal rLastAngle = m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.oGetValue(poKF->rGetKeyTime());

            // Avoid strange spins
            if (fabs(rLastAngle - fAngle) > _PIr_)
            {
                if (fAngle > rLastAngle)
                    fAngle -= _2PIr_;
                else
                    fAngle += _2PIr_;

                m_oBone[_iBoneIdx].m_poNodeAction->m_rAngleFunc.InsertKeyFrame(fAngle,_fTime,KFLF_LERP);
            }
            return;
        }
    }    
}//----------------------------------------------------------------------------
void CSkeleton::MoveBoneTo(uint _iBoneIdx,float _fTime,const CFEVect2& _oNewPos)
{
    // setup new bone position
    m_oBone[_iBoneIdx].m_poNodeAction->m_oPosFunc.InsertKeyFrame(_oNewPos,_fTime,KFLF_LERP);

    // if the bone is linked to its parent.
    if (m_oBone[_iBoneIdx].m_iParent != -1)
    {
        // modify also linked bone (orientation and scale)
        OrientBone(m_oBone[_iBoneIdx].m_iParent,_iBoneIdx,_fTime);
    }

    // if the bone has a child.
    if (m_oBone[_iBoneIdx].m_iChild != -1)
    {
        // modify also linked bone (orientation and scale)
        OrientBone(_iBoneIdx,m_oBone[_iBoneIdx].m_iChild,_fTime);
    }
}
//----------------------------------------------------------------------------
void CSkeleton::RescaleSkeleton(float _fHScale,float _fVScale)
{
    for (uint b=0;b<MAX_BONES;b++)
        m_oBone[b].m_poNode->SetScale( CFEVect2(_fHScale,_fVScale) );
}
//---------------------------------------------------------------------------
