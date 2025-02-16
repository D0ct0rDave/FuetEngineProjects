// ----------------------------------------------------------------------------
/*! \class CFESkelAnimSprite
 *  \brief A class to load FuetEngine  layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimSpriteH
#define CFESkelAnimSpriteH
//-----------------------------------------------------------------------------
#include "CFESkelAnimNode.h"
//-----------------------------------------------------------------------------
class CFESkelAnimVisitor;
//-----------------------------------------------------------------------------
class CFESkelAnimSprite : public CFESkelAnimNode
{
	public:		
				CFESkelAnimSprite(const CFEString& _sName) : CFESkelAnimNode(_sName), m_hSpriteInst(NULL)
				{
				}

				/// Sets the sprite for this object.
				void SetSprite(FEHandler _hSpriteInst)
				{
					m_hSpriteInst = _hSpriteInst;
				}

				/// Sets the sprite for this object.
				FEHandler hGetSprite()
				{
					return( m_hSpriteInst );
				}

				/// Sets the current action to be played by the sprite.
				void SetAction(uint _uiAction);

				/// Retrieves the current action being played by the sprite.
				uint uiGetAction();
			
                /// Perform processing over the object
        		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}

	private:	
				FEHandler	m_hSpriteInst;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
