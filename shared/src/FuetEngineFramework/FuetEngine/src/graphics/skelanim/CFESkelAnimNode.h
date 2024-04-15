// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNode
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
            m_oPos( CFEVect2::ZERO() ),
			m_rAngle(_0r),

			#ifdef USE_SKELANIM_SCALE_COLOR
            m_oScale( CFEVect2::ONE() ),
            m_oColor( CFEColor::WHITE()),
			#endif

            m_rDepth(_0r),
            m_bVisible(true)
        {
        }

        /// Destructor of the class
        virtual ~CFESkelAnimNode(){};

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

		#ifdef USE_SKELANIM_SCALE_COLOR
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
		#endif

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
		void Show(FEBool _bShow = true)
		{
			m_bVisible = _bShow;
		}

		/// Makes this object invisible.
		void Hide()
		{
			m_bVisible = false;
		}

		/// Retrieves whether this object is visible or not.
		FEBool bIsVisible()
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
		FEReal		m_rAngle;

		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2	m_oScale;
		CFEColor	m_oColor;
		#endif

		FEReal		m_rDepth;
		FEBool		m_bVisible;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
