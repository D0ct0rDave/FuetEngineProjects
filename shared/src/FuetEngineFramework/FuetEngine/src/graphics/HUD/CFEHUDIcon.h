// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDIconH
#define CFEHUDIconH
//-----------------------------------------------------------------------------
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDIcon : public CFEHUDObject
{
	public:		
				CFEHUDIcon(const CFEString& _sName) : CFEHUDObject(_sName)
				{
				}

        		/// Sets the image for this icon Object.
				void SetIcon(FEHandler _hSpriteInst)
				{
					m_hSpriteInst = _hSpriteInst;					

				}

				/// Sets the image for this icon Object.
				FEHandler hGetIcon()
				{
					return( m_hSpriteInst );
				}

				/// Sets the current action to be played by the sprite icon.
				void SetAction(uint _uiAction);

				/// Retrieves the current action being played by the sprite icon.
				uint uiGetAction()
				{
					return(m_uiAction);
				}
			
                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}

	private:	
				FEHandler	m_hSpriteInst;
				uint		m_uiAction;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
