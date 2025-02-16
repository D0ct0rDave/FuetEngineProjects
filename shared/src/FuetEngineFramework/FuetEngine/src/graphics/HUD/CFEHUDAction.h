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
#ifndef CFEHUDActionH
#define CFEHUDActionH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEKFBFunc.h"
#include "types/FEKFBFLerpFuncs.h"
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDObjectAction
{
	public:

		/// Retrieves the configuration of the HUDObject at a given moment in time.
		const CFEHUDObject& oGetValue(FEReal _fTime)
		{
			// Computes all the values for all the properties of the HUD object.
			m_poHUDObject->SetAngle( m_rAngleFunc.oGetValue(_fTime) );
			m_poHUDObject->SetPos( m_oPosFunc.oGetValue(_fTime) );
			m_poHUDObject->SetScale( m_oScaleFunc.oGetValue(_fTime) );
			m_poHUDObject->SetColor( m_oColorFunc.oGetValue(_fTime) );

			return( *m_poHUDObject );
		}

		/// Sets the HUD object associated to this layer
		void SetHUDObject(CFEHUDObject* _poHUDObject)
		{
			m_poHUDObject = _poHUDObject;
		}

		/// Retrieves the HUD object associated to this layer
		CFEHUDObject* poGetHUDObject()
		{
			return(m_poHUDObject);
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	public:

		CFEKFBFunc<bool>		m_bVisibleFunc;
		CFEKFBFunc<CFEVect2>	m_oPosFunc;
		CFEKFBFunc<CFEVect2>	m_oScaleFunc;
		CFEKFBFunc<CFEColor>	m_oColorFunc;
		CFEKFBFunc<FEReal>		m_rAngleFunc;

		CFEHUDObject*			m_poHUDObject;
};
//-----------------------------------------------------------------------------
class CFEHUDElementAction
{
    public:

        /// Default constructor of this element
        CFEHUDElementAction(const CFEString& _sName)
        {
            SetName(_sName);
        }

		/// Sets the name for this HUD element.
		void SetName(const CFEString& _sName)
		{
		    m_sName = _sName;
		}

		/// Retrieves the name of this HUD element action.
		const CFEString sGetName()
		{
		    return ( m_sName );
		}

		/// Adds a new action into the HUD Element.
		uint uiAddAction(CFEHUDObjectAction* _poAction)
		{
			m_oObjectActions.push_back(_poAction);
			return(m_oObjectActions.size()-1);
		}
		
		/// Retrieves the action identified by the given index.
		CFEHUDObjectAction* poGetAction(uint _uiAction)
		{
			return( m_oObjectActions[_uiAction] );
		}

		/// Deletes a action in the HUD element.
		void DeleteAction(uint _uiAction)
		{
			m_oObjectActions.erase(m_oObjectActions.begin() + _uiAction);
		}
        
        /// Swaps the two given actions
        void Swap(uint _uiIdxA,uint _uiIdxB)
        {
            CFEHUDObjectAction* poAux = m_oObjectActions[_uiIdxA];
            m_oObjectActions[_uiIdxA] = m_oObjectActions[_uiIdxB];
            m_oObjectActions[_uiIdxB] = poAux;
        }

		/// Retrieves the number of actions in the current HUD element.
		uint uiNumActions()
		{
			return(m_oObjectActions.size());
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}
		
		/// Sets the maximum time of the animation without looping or -1 if infinite.
		void SetActionTime(FEReal _rActionTime)
		{
			m_rActionTime = _rActionTime;
		}

		/// Retrieves the maximum time of the animation without looping or -1 if infinite
		FEReal rGetActionTime()
		{
			return(m_rActionTime);
		}

	protected:

		FEReal							m_rActionTime;
		CFEString                       m_sName;
		CFEArray<CFEHUDObjectAction*>   m_oObjectActions;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
