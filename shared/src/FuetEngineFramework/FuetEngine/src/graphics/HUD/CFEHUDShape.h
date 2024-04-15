// ----------------------------------------------------------------------------
/*! \class CFEHUDShape
 *  \brief A class to store a 2D poligonal mesh.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDShapeH
#define CFEHUDShapeH
//-----------------------------------------------------------------------------
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDShape : public CFEHUDObject
{
	public:		

        CFEHUDShape(const CFEString& _sName) : 
			CFEHUDObject(_sName),
			m_hMeshInst(NULL)
        {

        };
		
		/// Destructor of the class
		virtual ~CFEHUDShape();

		/// Sets the image for this icon Object.
		void SetMesh(FEHandler _hMeshInst)
		{
			m_hMeshInst = _hMeshInst;
		}

		/// Sets the image for this icon Object.
		FEHandler hGetMesh()
		{
			return( m_hMeshInst );
		}
		
        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:
		
		FEHandler	m_hMeshInst;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
