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
#ifndef CFEHUDRectH
#define CFEHUDRectH
//-----------------------------------------------------------------------------
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"

/*
	CFEShape -> es una malla de polígonos 3D (triángulos)
	CFEShapeLoader -> Una clase para cargar esta información y devolver un shape.
	CFEShapeInstMgr -> ??? no se si será interesante
*/
//-----------------------------------------------------------------------------
class CFEHUDRect : public CFEHUDObject
{
	public:		
	            CFEHUDRect(const CFEString& _sName) : CFEHUDObject(_sName)
	            {

	            };

                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    _poVisitor->Visit(this);
        		}
        		
        		/// Sets the pivot of the rect
        		void SetPivot(const CFEVect2& _oPivot)
        		{
        		    m_oPivot = _oPivot;
        		}

        		/// Retrieves the pivot of the rect
        		const CFEVect2& oGetPivot()
        		{
        		    return(m_oPivot);
        		}
        		
        		/// Retrieves the width of the rect
        		void SetWidth(FEReal _rWidth)
        		{
        		    m_rWidth = _rWidth;
        		}

        		/// Retrieves the width of the rect
        		FEReal rGetWidth()        		    
        		{
        		    return(m_rWidth);
        		}

        		/// Retrieves the height of the rect
        		void SetHeight(FEReal _rHeight)
        		{
        		    m_rHeight = _rHeight;
        		}

        		/// Retrieves the height of the rect
        		FEReal rGetHeight()
        		{
        		    return(m_rHeight);
        		}
        		
        		/// Sets one of ther corners colors
        		void SetCornerColor(uint _uiCorner,const CFEColor& _oColor)
        		{
        		    m_oCorners[_uiCorner] = _oColor;
        		}

        		/// Retrieves one of ther corners colors
        		const CFEColor& oGetCornerColor(uint _uiCorner)
        		{
        		    return( m_oCorners[_uiCorner] );
        		}

    protected:
	
	            FEReal                  m_rWidth;
	            FEReal                  m_rHeight;
	            CFEColor                m_oCorners[4];
				CFEVect2                m_oPivot;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------