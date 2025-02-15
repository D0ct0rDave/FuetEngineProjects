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
		    m_oIniPos	= CFEVect2::ZERO();
		    m_oIniScale	= CFEVect2::ONE();
		    m_rIniAngle	= _0r;
            m_oIniColor	= CFEColor::WHITE();
            m_iIniAction= -1;
            m_bVisible	= true;
            m_rDepth	= _0r;

		    m_oCurPos	= CFEVect2::ZERO();
		    m_oCurScale	= CFEVect2::ONE();
		    m_rCurAngle = _0r;
            m_oCurColor = CFEColor::WHITE();
            m_iCurAction= -1;
        }

		/// Destructor of the class
		virtual ~CFEHUDObject(){};

		// -----------------------------------------------
    	/// Sets the initial position for the HUD Object.
		void SetIniPos(const CFEVect2& _oPos)
		{
			m_oIniPos = _oPos;
		}

		/// Retrieves the initial position of the HUD Object.
		const CFEVect2& oGetIniPos()
		{
			return (m_oIniPos);
		}

		/// Sets the initial scale for the HUD Object.
		void SetIniScale(const CFEVect2& _oScale)
		{
			m_oIniScale = _oScale;
		}

		/// Retrieves the initial scale of the HUD Object.
		const CFEVect2& oGetIniScale()
		{
			return (m_oIniScale);
		}

		/// Sets the initial angle for the HUD Object.
		void SetIniAngle(FEReal _rAngle)
		{
			m_rIniAngle = _rAngle;
		}

		/// Retrieves the initial angle of the HUD Object.
		FEReal rGetIniAngle()
		{
			return(m_rIniAngle);
		}

		/// Sets the color for the HUD Object.
		void SetIniColor(const CFEColor& _oColor)
		{
			m_oIniColor = _oColor;
		}

		/// Retrieves the color of the HUD Object.
		const CFEColor& oGetIniColor()
		{
			return(m_oIniColor);
		}

		/// Sets the initial action for this object. (only sprites (and skel anims)).
		void SetIniAction(int _iAction)
		{
			m_iIniAction = _iAction;
		}

		/// Retrieves the initial action of this object. (only sprites (and skel anims)).
		int iGetIniAction()
		{
			return(m_iIniAction);
		}



    	// -----------------------------------------------
    	/// Sets the current position for the HUD Object.
		void SetCurPos(const CFEVect2& _oPos)
		{
			m_oCurPos = _oPos;
		}

		/// Retrieves the current position of the HUD Object.
		const CFEVect2& oGetCurPos()
		{
			return (m_oCurPos);
		}

		/// Sets the current scale for the HUD Object.
		void SetCurScale(const CFEVect2& _oScale)
		{
			m_oCurScale = _oScale;
		}

		/// Retrieves the current scale of the HUD Object.
		const CFEVect2& oGetCurScale()
		{
			return (m_oCurScale);
		}

		/// Sets the current angle for the HUD Object.
		void SetCurAngle(FEReal _rAngle)
		{
			m_rCurAngle = _rAngle;
		}

		/// Retrieves the current angle of the HUD Object.
		FEReal rGetCurAngle()
		{
			return(m_rCurAngle);
		}

		/// Sets the color for the HUD Object.
		void SetCurColor(const CFEColor& _oColor)
		{
			m_oCurColor = _oColor;
		}

		/// Retrieves the color of the HUD Object.
		const CFEColor& oGetCurColor()
		{
			return(m_oCurColor);
		}

		/// Sets the current action for this object. (only sprites (and skel anims)).
		virtual void SetCurAction(int _iAction)
		{
			m_iCurAction = _iAction;
		}

		/// Retrieves the current action of this object. (only sprites (and skel anims)).
		int iGetCurAction()
		{
			return(m_iCurAction);
		}


		// -----------------------------------------------
		/// Retrieves the initial position of the HUD Object.
		CFEVect2 oGetPos()
		{
			return (m_oCurPos + m_oIniPos);
		}

		/// Retrieves the initial scale of the HUD Object.
		CFEVect2 oGetScale()
		{
			return (m_oCurScale * m_oIniScale);
		}

		/// Retrieves the initial angle of the HUD Object.
		FEReal rGetAngle()
		{
			return(m_rCurAngle + m_rIniAngle);
		}

		/// Retrieves the color of the HUD Object.
		CFEColor oGetColor()
		{
			return(m_oCurColor);
		}

		/// Sets the current action for this object. (only sprites (and skel anims)).
		int iGetAction()
		{
			return(m_iCurAction);
		}

		


    	// -----------------------------------------------
    	/// Sets the current depth of the object
		void SetDepth(FEReal _rDepth)
		{
			m_rDepth = _rDepth;
		}

		/// Retrieves the current depth of the object
		FEReal rGetDepth()
		{
			return (m_rDepth);
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

		CFEVect2	m_oIniPos;
		CFEVect2	m_oIniScale;
		FEReal		m_rIniAngle;
		CFEColor	m_oIniColor;
		int			m_iIniAction;

		CFEVect2	m_oCurPos;
		CFEVect2	m_oCurScale;
		FEReal		m_rCurAngle;
		CFEColor	m_oCurColor;
		int			m_iCurAction;

		FEReal		m_rDepth;
		bool		m_bVisible;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
