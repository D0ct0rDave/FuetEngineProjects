// ----------------------------------------------------------------------------
/*! \class CFESkelAnim
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimH
#define CFESkelAnimH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEKFBFunc.h"
#include "types/FEKFBFLerpFuncs.h"
#include "types/CFENamedObject.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNodeAction
{
	public:

		CFESkelAnimNodeAction() : m_bVisible(true), m_uiNodeIdx(0), m_iNodeActionIdx(-1)
		{
		};

		/// Sets up the configuration of a given node according to the given time.
		void SetupNode(FEReal _fTime,CFESkelAnimNode* _poNode)
		{
			// Computes all the values for all the properties of the node.
			if (bIsVisible())
			{
				_poNode->Show();

				if (m_rAngleFunc.uiGetNumKeyFrames()>0) _poNode->SetAngle( m_rAngleFunc.oGetValue(_fTime) ); else _poNode->SetAngle(_0r);
				if (m_oPosFunc.uiGetNumKeyFrames()>0)   _poNode->SetPos( m_oPosFunc.oGetValue(_fTime) );	 else _poNode->SetPos( CFEVect2::ZERO() );
				if (m_oScaleFunc.uiGetNumKeyFrames()>0) _poNode->SetScale( m_oScaleFunc.oGetValue(_fTime) ); else _poNode->SetScale( CFEVect2::ONE() );
				if (m_oColorFunc.uiGetNumKeyFrames()>0) _poNode->SetColor( m_oColorFunc.oGetValue(_fTime) ); else _poNode->SetColor ( CFEColor::WHITE() );
			}
			else
				_poNode->Hide();
		}

		/// Sets the node index this action will affect.
		void SetNodeIdx(uint _uiNodeIdx)
		{
			m_uiNodeIdx = _uiNodeIdx;
		}

		/// Retrieves the node index this action is affecting.
		uint uiGetNodeIdx()
		{
			return (m_uiNodeIdx);
		}

		/// Sets the node action to be played when this node animation is active.
		void SetNodeActionIdx(int _iNodeActionIdx)
		{
			m_iNodeActionIdx = _iNodeActionIdx;
		}

		/// Retrieves the node action to be played when this node animation is active.
		int iGetNodeActionIdx()
		{
			return (m_iNodeActionIdx);
		}

		/// Makes the object associated to the action visible.
		void Show(bool _bShow = true)
		{
			m_bVisible = _bShow;
		}

		/// Makes the object associated to the action invisible.
		void Hide()
		{
			m_bVisible = false;
		}

		/// Retrieves whether the object associated to the action is visible or not.
		bool bIsVisible()
		{
			return(m_bVisible);
		}

	public:

		CFEKFBFunc<CFEVect2>	m_oPosFunc;
		CFEKFBFunc<CFEVect2>	m_oScaleFunc;
		CFEKFBFunc<CFEColor>	m_oColorFunc;
		CFEKFBFunc<FEReal>		m_rAngleFunc;
		bool					m_bVisible;
		uint					m_uiNodeIdx; 

		// Only valid for sprites
		int						m_iNodeActionIdx;
};
// ----------------------------------------------------------------------------
typedef enum ESAAPlayMode {

    SAAPM_NONE,

    SAAPM_ONESHOT,
    SAAPM_PINGPONGSTOP,

    SAAPM_LOOP,
    SAAPM_PINGPONG,

    SAAPM_NUM
    
}ESAAPlayMode;
//-----------------------------------------------------------------------------
class CFESkelAnimAction : public CFENamedObject
{
    public:

        /// Default constructor of this element
        CFESkelAnimAction(const CFEString& _sName) : CFENamedObject(_sName), m_rActionTime(_0r), m_rMaxActionTime(_0r), m_ePlayMode(SAAPM_NONE)
        {
        }
		
		/// Destructor of the class
        ~CFESkelAnimAction();

		/// Adds a new action into the  Element.
		uint uiAddNodeAction(CFESkelAnimNodeAction* _poNodeAction)
		{
			m_oNodeActions.push_back(_poNodeAction);
			return(m_oNodeActions.size()-1);
		}

		/// Retrieves the action identified by the given index.
		CFESkelAnimNodeAction* poGetNodeAction(uint _uiNodeAction)
		{
			return( m_oNodeActions[_uiNodeAction] );
		}

		/// Deletes a action in the  element.
		void DeleteNodeAction(uint _uiNodeAction)
		{
			m_oNodeActions.erase(m_oNodeActions.begin() + _uiNodeAction);
		}

        /// Swaps the two given actions
        void Swap(uint _uiIdxA,uint _uiIdxB)
        {
            CFESkelAnimNodeAction* poAux = m_oNodeActions[_uiIdxA];
            m_oNodeActions[_uiIdxA] = m_oNodeActions[_uiIdxB];
            m_oNodeActions[_uiIdxB] = poAux;
        }

		/// Retrieves the number of actions in the current  element.
		uint uiNumNodeActions()
		{
			return(m_oNodeActions.size());
		}

        /// Perform processing over the object
		virtual void Accept(CFESkelAnimVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

        /// Sets the Skel Anim Action Play Mode
        void SetPlayMode(ESAAPlayMode _ePlayMode)
        {
			m_ePlayMode = _ePlayMode;
        }

		/// Retrieves the Skel Anim Action Play Mode
        ESAAPlayMode eGetPlayMode()
        {
			return(m_ePlayMode);
        }

		/// Sets the maximum time of the animation without looping or -1 if infinite (when looping or pingpoing)
		void SetActionTime(FEReal _rActionTime)
		{
			m_rActionTime = _rActionTime;
		}

		/// Retrieves the maximum time of the animation without looping or -1 if infinite (when looping or pingpoing)
		FEReal rGetActionTime()
		{
			return(m_rActionTime);
		}
		
		/// Sets the maximum time of the animation taking into account the length of looping or pingpong cycles.
		void SetMaxActionTime(FEReal _rMaxActionTime)
		{
			m_rMaxActionTime = _rMaxActionTime;
		}

		/// Retrieves the maximum time of the animation taking into account the length of looping or pingpong cycles.
		FEReal rGetMaxActionTime()
		{
			return(m_rMaxActionTime);
		}
		
	protected:

		CFEArray<CFESkelAnimNodeAction*>	m_oNodeActions;
		FEReal								m_rActionTime;
		FEReal								m_rMaxActionTime;
		ESAAPlayMode						m_ePlayMode;
};
//-----------------------------------------------------------------------------
class CFESkelAnimActionSet
{
	public:
		
		/// Destructor of the class
		~CFESkelAnimActionSet();
		
		/// Adds a new action into the animation.
		uint uiAddAction(CFESkelAnimAction* _poAction)
		{
			m_oActions.push_back(_poAction);
			return(m_oActions.size()-1);
		}
		
		/// Retrieves the action identified by the given index.
		CFESkelAnimAction* poGetAction(uint _uiAction)
		{
			return( m_oActions[_uiAction] );
		}

		/// Deletes a action in the  element.
		void DeleteAction(uint _uiAction)
		{
			m_oActions.erase(m_oActions.begin() + _uiAction);
		}

        /// Swaps the two given actions
        void Swap(uint _uiIdxA,uint _uiIdxB)
        {
            CFESkelAnimAction* poAux = m_oActions[_uiIdxA];
            m_oActions[_uiIdxA] = m_oActions[_uiIdxB];
            m_oActions[_uiIdxB] = poAux;
        }

		/// Retrieves the number of actions in the current  element.
		uint uiNumActions()
		{
			return(m_oActions.size());
		}

		CFESkelAnimAction* poGetAction(const CFEString& _sActionName)
		{
			int iIdx = iGetActionIdx(_sActionName);
			if (iIdx == -1) return(NULL);

			return( m_oActions[iIdx] );
		}

		int iGetActionIdx(const CFEString& _sActionName)
		{
			for (uint i=0;i<this->m_oActions.size();i++)
				if ((m_oActions[i]!=NULL) && (m_oActions[i]->sGetName() |= _sActionName))
					return((int)i);

			return(-1);		
		}

	protected:

		CFEArray<CFESkelAnimAction*>		m_oActions;
};
//-----------------------------------------------------------------------------
class CFESkelAnim : public CFENamedObject
{
    public:

        /// Default constructor of this element
        CFESkelAnim(const CFEString& _sName) :  CFENamedObject(_sName), m_poActionSet(NULL), m_poAnimNode(NULL)
        {
        }
		
		/// Destructor of the class
		~CFESkelAnim();
		
		/// Sets the animation node for this animation.
		void SetAnimNode(CFESkelAnimNode* _poAnimNode)
		{
			m_poAnimNode = _poAnimNode;
		}

		/// Retrieves the animation node of this animation.
		CFESkelAnimNode* poGetAnimNode()
		{
			return (m_poAnimNode);
		}

		/// Sets the animation action set for the associated anim node.
		void SetActionSet(CFESkelAnimActionSet* _poActionSet)
		{
			m_poActionSet = _poActionSet;
		}

		/// Retrieves the animation action set for the associated anim node.
		CFESkelAnimActionSet* poGetAnimActionSet()
		{
			return (m_poActionSet);
		}

        /// Perform processing over the object
		virtual void Accept(CFESkelAnimVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:

		// Set of actions related to this animation.
		CFESkelAnimActionSet*				m_poActionSet;

		// The node hierarchy this animation belongs.
		CFESkelAnimNode*					m_poAnimNode;

	public:	
		// WARNING: Should not be public.
		// Array of correspondences between node actions and nodes.
		CFEArray<CFESkelAnimNode*>			m_oNodeTab;
};
// ----------------------------------------------------------------------------
class CFESkelAnimInst
{
    public:

        /// Current status of the skeleton animation, i.e. animation and other things.

        /// Current action being played.
        uint m_uiAction;

        /// Moment in time related to the current action.
        FEReal m_rActionTime;

		/// The original skeleton animation referenced by this instance.
        CFESkelAnim* m_poSkelAnim;

		// The instanced node hierarchy this animation belongs.
		CFESkelAnimNode*		   m_poInstAnimNode;

		// Array of correspondences between node actions and instance nodes
		CFEArray<CFESkelAnimNode*>* m_poNodeInstTab;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
