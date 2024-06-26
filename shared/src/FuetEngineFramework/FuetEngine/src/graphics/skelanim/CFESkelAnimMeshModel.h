// ----------------------------------------------------------------------------
/*! \class CFESkelAnimMeshModel
 *  \brief A class that stores features of a mesh inside a skeletal hierarchy.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimMeshModelH
#define CFESkelAnimMeshModelH
//-----------------------------------------------------------------------------
#include "types/CFEArray.h"
#include "CFESkelAnimNode.h"
//-----------------------------------------------------------------------------
class CFESkelAnimVisitor;
//-----------------------------------------------------------------------------
class CFESkelAnimMeshModel : public CFESkelAnimNode
{
    public:		

		CFESkelAnimMeshModel(const CFEString& _sName) : CFESkelAnimNode(_sName), m_sSprite(""), m_uiSpriteAction(0)
		{
		}

		/// Destructor of the class
		virtual ~CFESkelAnimMeshModel();

		/// Attach a bone to the mesh model
		void AttachBone(const CFEString& _sBoneName)
		{
			m_sAttachedBones.push_back(_sBoneName);
		}

		/// Retrieves the name of an attached bone identified by the given index.
		const CFEString& sGetAttachedBone(uint _uiBoneIdx)
		{
			return( m_sAttachedBones[_uiBoneIdx] );
		}

		/// Retrieves then number of attached bones to this mesh model.
		uint uiGetNumAttachedBones()
		{
			return( m_sAttachedBones.size() );
		}

		/// Sets the material used by this mesh model.
		void SetSprite(const CFEString& _sSprite)
		{
			m_sSprite = _sSprite;
		}

		/// Retrieves the material used by this mesh model.
		const CFEString& sGetSprite()
		{
			return( m_sSprite );
		}
			
		/// Sets the current action to be played by the sprite.
		void SetSpriteAction(uint _uiSpriteAction)
		{
			m_uiSpriteAction = _uiSpriteAction;
		}

		/// Sets the current action to be played by the sprite.
		const uint uiGetSpriteAction()
		{
			return( m_uiSpriteAction );
		}
            
		/// Perform processing over the object
    	virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
    	{
    		_poVisitor->Visit(this);
    	}

	private:

		CFEString           m_sSprite;
		uint				m_uiSpriteAction;
		CFEArray<CFEString> m_sAttachedBones;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
