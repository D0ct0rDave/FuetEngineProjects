// ----------------------------------------------------------------------------
/*! \class CFESkelAnimSprite
 *  \brief This class stores the features of a sprite inside a skeletal hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

	    CFESkelAnimSpriteModel(const CFEString& _sName) : CFESkelAnimNode(_sName), m_sSpriteModel("")
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

        /// Perform processing over the object
	    virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
	    {
	        _poVisitor->Visit(this);
	    }

    private:	

	    CFEString   m_sSpriteModel;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
