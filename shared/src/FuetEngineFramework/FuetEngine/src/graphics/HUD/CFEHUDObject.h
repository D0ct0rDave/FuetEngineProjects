// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDObjectH
#define CFEHUDObjectH
//-----------------------------------------------------------------------------
#include "types/CFENamedObject.h"
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDObject : public CFENamedObject
{
	public:

        /// Default constructor of this element
        CFEHUDObject(const CFEString& _sName) : CFENamedObject(_sName)
        {
		    m_oPos = CFEVect2::oZERO();
		    m_oScale= CFEVect2::oONE();
		    m_rAngle = _0r;
            m_oColor = CFEColor(1,1,1,1);
            m_bVisible = true;
        }

    	/// Sets the position for the HUD Object.
		void SetPos(const CFEVect2& _oPos)
		{
			m_oPos = _oPos;
		}

		/// Retrieves the position of the HUD Object.
		const CFEVect2& oGetPos()
		{
			return (m_oPos);
		}

		/// Sets the size for the HUD Object.
		void SetScale(const CFEVect2& _oScale)
		{
			m_oScale = _oScale;
		}

		/// Retrieves the size of the HUD Object.
		const CFEVect2& oGetScale()
		{
			return (m_oScale);
		}

		/// Sets the angle for the HUD Object.
		void SetAngle(FEReal _rAngle)
		{
			m_rAngle = _rAngle;
		}

		/// Retrieves the angle of the HUD Object.
		FEReal rGetAngle()
		{
			return(m_rAngle);
		}

		/// Sets the color for the HUD Object.
		void SetColor(const CFEColor& _oColor)
		{
			m_oColor = _oColor;
		}

		/// Retrieves the color of the HUD Object.
		const CFEColor& oGetColor()
		{
			return(m_oColor);
		}

		/// Makes this HUD Object visible.
		void Show(bool _bShow = true)
		{
			m_bVisible = _bShow;
		}

		/// Makes this HUD Object invisible.
		void Hide()
		{
			m_bVisible = false;
		}

		/// Retrieves whether this object is visible or not.
		bool bIsVisible()
		{
			return(m_bVisible);
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}


	protected:

		CFEVect2	m_oPos;
		CFEVect2	m_oScale;
		FEReal		m_rAngle;
		CFEColor	m_oColor;
		CFEString   m_sName;
		bool		m_bVisible;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------