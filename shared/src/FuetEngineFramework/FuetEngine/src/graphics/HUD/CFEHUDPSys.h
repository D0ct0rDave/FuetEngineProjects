// ----------------------------------------------------------------------------
/*! \class CFEHUDPSys
 *  \brief A class to store a 2D poligonal mesh.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDPSysH
#define CFEHUDPSysH
//-----------------------------------------------------------------------------
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDPSys : public CFEHUDObject
{
	public:

        CFEHUDPSys(const CFEString& _sName) : CFEHUDObject(_sName), m_hPSysInst(NULL)
        {

        };

		/// Destructor of the class
		virtual ~CFEHUDPSys();

		/// Sets the psys instance for this particle system Object.
		void SetPSys(FEHandler _hPSysInst)
		{
			m_hPSysInst = _hPSysInst;
		}

		/// Sets the psys instance for this particle system object.
		FEHandler hGetPSys()
		{
			return( m_hPSysInst );
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:
				FEHandler	m_hPSysInst;	
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
