// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNode
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeH
#define CFESkelAnimNodeH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "types/CFENamedObject.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNode : public CFENamedObject
{
	public:

        /// Default constructor of this element
        CFESkelAnimNode(const CFEString& _sName) :
            CFENamedObject(_sName),
            m_oPos( CFEVect2(_0r,_0r) ),
            m_oScale( CFEVect2(_1r,_1r) ),
            m_rAngle(_0r),
            m_oColor(CFEColor(_1r,_1r,_1r,_1r)),
            m_rDepth(_0r),
            m_bVisible(true)
        {
        }

    	/// Sets the position for the object.
		void SetPos(const CFEVect2& _oPos)
		{
			m_oPos = _oPos;
		}

		/// Retrieves the position of the object.
		const CFEVect2& oGetPos()
		{
			return (m_oPos);
		}

		/// Sets the size for the object.
		void SetScale(const CFEVect2& _oScale)
		{
			m_oScale = _oScale;
		}

		/// Retrieves the size of the object.
		const CFEVect2& oGetScale()
		{
			return (m_oScale);
		}

		/// Sets the angle for the object.
		void SetAngle(FEReal _rAngle)
		{
			m_rAngle = _rAngle;
		}

		/// Retrieves the angle of the object.
		FEReal rGetAngle()
		{
			return(m_rAngle);
		}

		/// Sets the color for the object.
		void SetColor(const CFEColor& _oColor)
		{
			m_oColor = _oColor;
		}

		/// Retrieves the color of the object.
		const CFEColor& oGetColor()
		{
			return(m_oColor);
		}

		/// Sets the depth for the object.
		void SetDepth(FEReal _rDepth)
		{
			m_rDepth = _rDepth;
		}

		/// Retrieves the depth of the object.
		FEReal rGetDepth()
		{
			return(m_rDepth);
		}

		/// Makes this object visible.
		void Show(bool _bShow = true)
		{
			m_bVisible = _bShow;
		}

		/// Makes this object invisible.
		void Hide()
		{
			m_bVisible = false;
		}

		/// Retrieves whether this object is visible or not.
		bool bIsVisible()
		{
			return(m_bVisible);
		}

        /// Perform processing over the object.
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:

		CFEVect2	m_oPos;
		CFEVect2	m_oScale;
		FEReal		m_rAngle;
		CFEColor	m_oColor;
		FEReal		m_rDepth;
		bool		m_bVisible;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
