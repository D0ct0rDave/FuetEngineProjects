// ----------------------------------------------------------------------------
/*! \class CFESkelAnimSprite
 *  \brief This class stores the features of a sprite inside a skeletal hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimSpriteModelH
#define CFESkelAnimSpriteModelH
//-----------------------------------------------------------------------------
#include "CFESkelAnimNode.h"
//-----------------------------------------------------------------------------
class CFESkelAnimVisitor;
class CFESprite;
//-----------------------------------------------------------------------------
class CFESkelAnimSpriteModel : public CFESkelAnimNode
{
	public:		

	    CFESkelAnimSpriteModel(const CFEString& _sName) : CFESkelAnimNode(_sName), m_sSpriteModel(""), m_uiSpriteAction(0)
	    {
	    }

		/// Destructor of the class.
		virtual ~CFESkelAnimSpriteModel();

	    /// Sets the sprite model for this object.
	    void SetSprite(const CFEString& _sSpriteModel)
	    {
		    m_sSpriteModel = _sSpriteModel;
	    }

	    /// Sets the sprite model for this object.
	    const CFEString& sGetSprite()
	    {
		    return( m_sSpriteModel );
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

	    CFEString   m_sSpriteModel;
		uint		m_uiSpriteAction;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
