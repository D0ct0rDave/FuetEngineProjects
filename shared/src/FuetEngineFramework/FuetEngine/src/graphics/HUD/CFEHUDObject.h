// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
		    m_rIniDepth = _0r;
            m_oIniColor	= CFEColor::WHITE();
            m_iIniAction= -1;
            m_bIniVis   = true;

            m_sTAG		= "";

		    m_oCurPos	= CFEVect2::ZERO();
		    m_oCurScale	= CFEVect2::ONE();
		    m_rCurAngle = _0r;
		    m_rCurDepth = _0r;
            m_oCurColor = CFEColor::WHITE();
			m_iCurAction= -1;
			m_bCurVis = true;
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

		/// Sets the initial depth for the HUD Object.
		void SetIniDepth(FEReal _rDepth)
		{
			m_rIniDepth = _rDepth;
		}

		/// Retrieves the initial depth of the HUD Object.
		FEReal rGetIniDepth()
		{
			return(m_rIniDepth);
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

		/// Sets the initial visibility of the object
		void SetIniVis(FEBool _bVisible)
		{
			m_bIniVis = _bVisible;
		}

		/// Retrieves the initial visibility of the object
		FEBool bGetIniVis()
		{
			return(m_bIniVis);
		}

		/// Makes this HUD Object visible.
		void ShowObj(FEBool _bShowObj = true)
		{
			SetIniVis(_bShowObj);
		}

		/// Makes this HUD Object invisible.
		void HideObj()
		{
			ShowObj(false);
		}

		/// Retrieves whether this object is visible or not.
		FEBool bIsObjVisible()
		{
			return(bGetIniVis());
		}

		/// Sets the initial action for this object. (only sprites, meshes, (and skel anims)).
		void SetIniAction(int _iAction)
		{
			m_iIniAction = _iAction;
		}

		/// Retrieves the initial action of this object. (only sprites, meshes, (and skel anims)).
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

		/// Sets the current depth for the HUD Object.
		void SetCurDepth(FEReal _rDepth)
		{
			m_rCurDepth = _rDepth;
		}

		/// Retrieves the current depth of the HUD Object.
		FEReal rGetCurDepth()
		{
			return(m_rCurDepth);
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
		
		/// Sets the action visibility
		void SetCurVis(FEBool _bCurVis)
		{
			m_bCurVis = _bCurVis;
		}

		/// Retrieves whether this object is not visible or should take into account object visibiliy.
		FEBool bGetCurVis()
		{
			return( m_bCurVis );
		}

		/// Sets the current action for this object. (only sprites, meshes, (and skel anims)).
		virtual void SetCurAction(int _iAction)
		{
			m_iCurAction = _iAction;
		}

		/// Retrieves the current action of this object. (only sprites, meshes, (and skel anims)).
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

		/// Retrieves the current depth of the object
		FEReal rGetDepth()
		{
			return (m_rCurDepth + m_rIniDepth);
		}

		/// Retrieves the color of the HUD Object.
		CFEColor oGetColor()
		{
			return(m_oCurColor*m_oIniColor);
		}

		/// Retrieves the object action.
		int iGetAction()
		{
			if (m_iCurAction == -1)
				return(m_iIniAction);
			else
				return(m_iCurAction);
		}

		/// Retrieves whether this object is visible or not.
		FEBool bIsVisible()
		{
			return(m_bIniVis && m_bCurVis);
		}
		/// Sets the TAG string for this object
		void SetTAG(const CFEString& _sTAG)
		{
			m_sTAG = _sTAG;
		}
		/// Retrieves the TAG string of this object
		const CFEString& sGetTAG()
		{
			return(m_sTAG);
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
		FEReal		m_rIniDepth;
		CFEColor	m_oIniColor;
		int			m_iIniAction;
		FEBool		m_bIniVis;

		//
		CFEString	m_sTAG;

		/// Values set up by actions
		CFEVect2	m_oCurPos;
		CFEVect2	m_oCurScale;
		FEReal		m_rCurAngle;
		FEReal		m_rCurDepth;
		CFEColor	m_oCurColor;
		int			m_iCurAction;
		FEBool		m_bCurVis;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
