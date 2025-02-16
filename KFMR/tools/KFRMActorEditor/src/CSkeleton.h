//---------------------------------------------------------------------------
//
// Name:        CSkeleton.h
// Author:      David Márquez de la Cruz
// Created:     7/10/2010
// Description: CSkeleton class declaration
//
//---------------------------------------------------------------------------
#ifndef CSkeletonH
#define CSkeletonH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
//---------------------------------------------------------------------------
enum 
{
    BONE_HEAD,
    BONE_NECK,
    
    BONE_LSHOULDER,
    BONE_RSHOULDER,
    BONE_LELBOW,
    BONE_RELBOW,
    BONE_LHAND,
    BONE_RHAND,
    BONE_LTHAND,
    BONE_RTHAND,
    
    BONE_HIP,
    BONE_LHIP,
    BONE_RHIP,
    BONE_LKNEE,
    BONE_RKNEE,
    BONE_LFOOT,
    BONE_RFOOT,
    BONE_LTFOOT,
    BONE_RTFOOT,
    
    BONE_PIVOT,    
    MAX_BONES,
};
//---------------------------------------------------------------------------
extern const char* BONE_NAMES[MAX_BONES];

// const uint MAX_SKINNED_MESHES = 5;
const uint MAX_SKINNED_MESHES = 4;
extern const char* SKINNED_MESHES[MAX_SKINNED_MESHES];
//---------------------------------------------------------------------------
typedef enum EBoneType
{
    BT_NONE,
    BT_SPRITE,
    BT_BONE,
    BT_SPRITE_AND_BONE,
    BT_CONTROL
};

const uint NUM_NORMAL_BONES = MAX_BONES-6;
//---------------------------------------------------------------------------
class CBone
{   
    public:

        CBone() :   m_iParent(-1), 
                    m_iChild(-1), 
                    m_oOfs(CFEVect2::oZERO()),
                    m_eBoneType(BT_NONE), 
                    m_sExternalFile(""),
                    m_poNodeAction(NULL),
                    m_poNode(NULL),
                    m_poSpr(NULL),
                    m_poMesh(NULL)
        {
        }

    public:

        // These attributes are filled during skeleton creation
        CFESkelAnimNodeAction*  m_poNodeAction;
        CFESkelAnimNode*        m_poNode;       // can be a group or a bone.
        CFESkelAnimSprite*      m_poSpr;        // if node is a group this is its child.
        CFESkelAnimMesh*        m_poMesh;       // if node is a bone this is attached to it.

        int                     m_iChild;       // Child bone of this bone.
        int                     m_iParent;      // Parent bone of this bone.

        EBoneType               m_eBoneType;    // Type of bone.

        // These are edited attributes.
        CFEVect2                m_oOfs;         // Pivot Offset.
        CFEString               m_sExternalFile;
};
//---------------------------------------------------------------------------
class CSkeleton
{
    public:
        CSkeleton();
        CBone   m_oBone[MAX_BONES];

        CFESkelAnimNodeAction*  m_poPivot;
        CFESkelAnimGroup*       m_poSkel;
        CFESkelAnim*            m_poAnim;
        FEHandler               m_hInst;
        
        void RescaleSkeleton(float _fHScale,float _fVScale);
        void TranslateSkeleton(float _fXOfs,float _fYOfs);

        void MoveBoneTo(uint _iBoneIdx,float _fTime,const CFEVect2& _oNewPos);

    protected:
        void OrientBone(uint _iBoneIdx, uint _iOrientToBoneIdx, float _fTime);
        void CreateBone(uint _uiBoneIdx,int _iParent,EBoneType _eBoneType,CFESkelAnimMesh* _poMesh = NULL);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
