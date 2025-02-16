// ----------------------------------------------------------------------------
/*! \class CFEHUD
 *  \brief A class to store all the HUD elements of the game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDH
#define CFEHUDH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDElement;
//-----------------------------------------------------------------------------
class CFEHUD
{
	public:

        /// Default constructor of the class.
        CFEHUD()
        {

        }

		/// Adds a new element into the HUD.
		uint uiAddElement(CFEHUDElement* _poElement)
		{
			m_oElements.push_back(_poElement);
			return(m_oElements.size()-1);
		}

		/// Retrieves the element identified by the given index.
		CFEHUDElement* poGetElement(uint _uiElement)
		{
			return( m_oElements[_uiElement] );
		}

		/// Retrieves the number of element in the HUD.
		uint uiNumElements()
		{
			return(m_oElements.size());
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:

		CFEArray<CFEHUDElement*> m_oElements;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
