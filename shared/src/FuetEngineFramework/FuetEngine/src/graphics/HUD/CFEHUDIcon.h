// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
				CFEHUDIcon(const CFEString& _sName) : CFEHUDObject(_sName), m_hSpriteInst(NULL)
				{
				}

				/// Destructor of the class
				virtual ~CFEHUDIcon();

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

                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}

	private:	
				FEHandler	m_hSpriteInst;				
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
