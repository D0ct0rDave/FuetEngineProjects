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
class CBone
{   
    public:

        CBone() :   m_poPJointAction(NULL),
					m_poBJointAction(NULL),
					
					m_poBoneAction(NULL),
                    
                    m_poBone(NULL),
                    m_poParentJoint(NULL),
                    // m_poBoneJoint(NULL),
                    // m_poChildJoint(NULL),
                    m_poLeafChild(NULL),
                    
					m_sParentName(""),
					m_bAffectParent(true),
                   
					m_iParent(-1)
        {
        }

    public:
		/// Retrieves the name of this bone.
		const CFEString& sGetName() { return(m_poBone->sGetName()); };
		
		/// Retrieves the child of this bone.
		CFESkelAnimNode* poGetChild() { return(m_poBone->poGetChild(0)); };
      
        /// Retrieves whether this bone has editable information or not.
        bool bEditable() { return( m_poBone->uiNumChildren() > 0); };

        /// Retrieves whether this bone has editable information or not.
        bool bVisible() { return( m_poBone->bIsVisible()); };

        // These attributes are filled during skeleton creation
        CFESkelAnimNodeAction*  m_poPJointAction;	// Parent Join action data for angle animation)
        CFESkelAnimNodeAction*  m_poBJointAction;	// Bone action data (for position animation) (affects attached node)
		CFESkelAnimNodeAction*  m_poBoneAction;

        CFESkelAnimBone*        m_poBone;			// can be a group or a bone.
		CFESkelAnimGroup*		m_poParentJoint;
		// CFESkelAnimGroup*		m_poBoneJoint;
		// CFESkelAnimGroup*		m_poChildJoint;		// Children bones of this node.
		CFESkelAnimNode*		m_poLeafChild;		// Children Leaf of this bone.

		CFEString				m_sParentName;
		bool					m_bAffectParent;

		// These information is not saved in ske/ska animation files so we need in the editor these.
        int								m_iParent;      // Parent bone of this bone.
        CFEArray<int>					m_iChild;		// List of children bones of this bone.
};
//---------------------------------------------------------------------------
class CSkeleton
{
    public:

		/// Constructors for the class
		CSkeleton(CSkeleton *_poSkeleton);
        CSkeleton(const CFEString& _sFilename);
        CSkeleton(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);

        /// Destructor of the class
		~CSkeleton();

        void RescaleSkeleton(float _fHScale,float _fVScale);

        CFEVect2 oGetAbsBonePos(uint _uiBoneIdx,float _fTime,uint _uiEditMode = m_uiEditMode);
		
		void OrientBone(uint _iBoneIdx, float _fAngle, float _fTime);

		// 0 skeleton
		// 1 Bone orientation
		// 2 Bone position
		void MoveBoneTo(uint _uiBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle,uint _uiEditMode = m_uiEditMode);
		
		void SetEditMode(uint _uiEditMode)
		{	
			m_uiEditMode = _uiEditMode;
		}

		void SynchronizeSprites(FEReal _rSpeedFact);

	public:

		CFEArray<CBone>			m_oBone;

		bool					m_bAnimPivot;
        CFESkelAnimNodeAction*  m_poPivot;
        uint					m_uiPivotIdx;
        CFESkelAnim*            m_poAnim;
        FEHandler               m_hInst;
		static uint				m_uiEditMode;
       
	protected:
		
		void MoveSkeletonBoneTo(uint _uiBoneIdx,const CFEVect2& _oNewPos);
        void OrientBoneFromTo(uint _uiBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle);
        void MoveBoneFromTo(uint _uiBoneIdx,float _fTime,const CFEVect2& _oNewPos,const CFEVect2& _oOldPos,const CFEVect2& _oOldParentPos,float _fOldAngle);

        uint uiCreateBone(const CFEString& _sBoneName,const CFEString& _sParentName,CFESkelAnimNode* _poChild,bool _bAffectParent);
        CFESkelAnimNode* poReadNode(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);        
        void ReadSkeletonDefinition(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);
        void Init(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);
        void BuildStructure();

        // Resolve parents once the skeleton has been initialized from file.
        void ResolveParents();
		void ResolveMeshModels();
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
