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
#ifndef CFEHUDActionH
#define CFEHUDActionH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFENamedObject.h"
#include "types/CFEKFBFunc.h"
#include "types/FEKFBFLerpFuncs.h"
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
#include "CFEHUDEventFunc.h"
//-----------------------------------------------------------------------------
class CFEHUDObjectAction
{
	public:
		
		/// Default constructor for the class.
		CFEHUDObjectAction() :
			m_poHUDObject(NULL)
		{
		}

		/// Destructor of the class.
		~CFEHUDObjectAction();

		/// Retrieves the configuration of the HUDObject at a given moment in time.
		const CFEHUDObject& oGetValue(FEReal _rTime)
		{
			// Computes all the values for all the properties of the HUD object.		
			m_poHUDObject->SetCurVis( m_bVisFunc.oGetValue(_rTime) );

			/// this properties should only be setup if the object is visible.
			if (m_poHUDObject->bIsVisible())
			{
				m_poHUDObject->SetCurAngle( m_rAngleFunc.oGetValue(_rTime) );
				m_poHUDObject->SetCurPos( CFEVect2(m_rXFunc.oGetValue(_rTime),m_rYFunc.oGetValue(_rTime)) );
				m_poHUDObject->SetCurScale( m_oScaleFunc.oGetValue(_rTime) );
				m_poHUDObject->SetCurColor( m_oColorFunc.oGetValue(_rTime) );
				m_poHUDObject->SetCurDepth( m_rDepthFunc.oGetValue(_rTime) );
				m_poHUDObject->SetCurAction( m_iActionFunc.oGetValue(_rTime) );

				// if the object is not visible it shoudn't trigger an event.
				m_oEventFunc.Check(_rTime);
			}

			return( *m_poHUDObject );
		}

		/// Sets the HUD object associated to this layer
		void SetHUDObject(CFEHUDObject* _poHUDObject)
		{
			m_oEventFunc.SetObject(_poHUDObject);
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

		CFEKFBFunc<FEReal>		m_rXFunc;
		CFEKFBFunc<FEReal>		m_rYFunc;
		CFEKFBFunc<CFEVect2>	m_oScaleFunc;
		CFEKFBFunc<CFEColor>	m_oColorFunc;
		CFEKFBFunc<FEReal>		m_rAngleFunc;
		CFEKFBFunc<FEReal>		m_rDepthFunc;
		CFEKFBFunc<int>			m_iActionFunc;
		CFEKFBFunc<FEBool>		m_bVisFunc;
		CFEHUDEventFunc			m_oEventFunc;

	protected:

		CFEHUDObject*			m_poHUDObject;
};
//-----------------------------------------------------------------------------
class CFEHUDElementAction : public CFENamedObject
{
    public:

        /// Default constructor of this element
        CFEHUDElementAction(const CFEString& _sName) : CFENamedObject(_sName), m_rActionTime(_0r), m_rMaxActionTime(_0r)
        {
        }
		
		/// Destructor of the class
		~CFEHUDElementAction();

		/// Adds a new action into the HUD Element.
		uint uiAddAction(CFEHUDObjectAction* _poAction)
		{
			m_oObjectActions.push_back(_poAction);
			return(m_oObjectActions.size()-1);
		}
			
		/// Inserts a new action into the given position.
		void InsertAction(CFEHUDObjectAction* _poAction,uint _uiIdx)
		{
			m_oObjectActions.insert(m_oObjectActions.begin() + (long)_uiIdx, _poAction);
		}		

		/// Retrieves the action identified by the given index.
		CFEHUDObjectAction* poGetAction(uint _uiAction)
		{
			return( m_oObjectActions[_uiAction] );
		}

		/// Deletes a action in the HUD element.
		void DeleteAction(uint _uiAction)
		{
			m_oObjectActions.erase(m_oObjectActions.begin() + (long)_uiAction);
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
		
		/// Sets the maximum time of the animation without looping or -1 if infinite (when looping).
		void SetActionTime(FEReal _rActionTime)
		{
			m_rActionTime = _rActionTime;
		}

		/// Retrieves the maximum time of the animation without looping or -1 if infinite (when looping)
		FEReal rGetActionTime()
		{
			return(m_rActionTime);
		}

		/// Sets the maximum time of the animation taking into account the length of looping cycles.
		void SetMaxActionTime(FEReal _rMaxActionTime)
		{
			m_rMaxActionTime = _rMaxActionTime;
		}

		/// Retrieves the maximum time of the animation taking into account the length of looping cycles.
		FEReal rGetMaxActionTime()
		{
			return(m_rMaxActionTime);
		}

	protected:

		FEReal							m_rActionTime;
		FEReal							m_rMaxActionTime;
		CFEArray<CFEHUDObjectAction*>   m_oObjectActions;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
