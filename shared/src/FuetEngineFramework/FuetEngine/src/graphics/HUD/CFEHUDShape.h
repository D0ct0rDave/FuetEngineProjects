// ----------------------------------------------------------------------------
/*! \class CFEHUDShape
 *  \brief A class to store a 2D poligonal mesh.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDShapeH
#define CFEHUDShapeH
//-----------------------------------------------------------------------------
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
/*
	CFEShape -> es una malla de polígonos 2D (triángulos)
	CFEShapeLoader -> Una clase para cargar esta información y devolver un shape.
	CFEShapeInstMgr -> ??? no se si será interesante
*/
//-----------------------------------------------------------------------------
class CFEHUDShape : public CFEHUDObject
{
	public:		

        CFEHUDShape(const CFEString& _sName) : CFEHUDObject(_sName)
        {

        };

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------