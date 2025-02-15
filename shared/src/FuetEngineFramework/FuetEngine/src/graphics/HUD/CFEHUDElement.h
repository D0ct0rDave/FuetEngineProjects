// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDElementH
#define CFEHUDElementH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "types/CFEArray.h"
#include "types/CFENamedObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDElementAction;
class CFEHUDObject;
//-----------------------------------------------------------------------------
class CFEHUDElement : public CFENamedObject
{
	public:

        /// Default constructor of this element
        CFEHUDElement(const CFEString& _sName) : CFENamedObject(_sName)
        {
        }
        
        /// Destructor of the class.
        ~CFEHUDElement();

		/// Inserts an action at the given position
		void InsertAction(uint _uiIdx,CFEHUDElementAction* _poAction)
		{
			m_oActions.insert(this->m_oActions.begin() + _uiIdx,_poAction);
		}

		/// Adds a new action into the HUD Element.
		uint uiAddAction(CFEHUDElementAction* _poAction)
		{
			m_oActions.push_back(_poAction);
			return(m_oActions.size()-1);
		}

		/// Retrieves the action identified by the given index.
		CFEHUDElementAction* poGetAction(uint _uiAction)
		{
			return( m_oActions[_uiAction] );
		}

		/// Deletes a action in the HUD element.
		void DeleteAction(uint _uiAction)
		{
			m_oActions.erase(m_oActions.begin() + _uiAction);
		}

		/// Retrieves the number of actions in the current HUD element.
		uint uiNumActions()
		{
			return(m_oActions.size());
		}

		/// Adds a new layer into the HUD Element.
		uint uiAddLayer(CFEHUDObject* _poLayer)
		{
			m_oLayers.push_back(_poLayer);
			return(m_oLayers.size()-1);
		}

		/// Retrieves the layer identified by the given index.
		CFEHUDObject* poGetLayer(uint _uiLayer)
		{
			return( m_oLayers[_uiLayer] );
		}

		/// Deletes a layer in the HUD element.
		void DeleteLayer(uint _uiLayer)
		{
			m_oLayers.erase(m_oLayers.begin() + _uiLayer);
		}

		/// Swap the contents of layer A and B.
		void SwapLayers(uint _uiLayerA,uint _uiLayerB)
		{
		    CFEHUDObject* poAuxLayer = m_oLayers[_uiLayerA];
		    m_oLayers[_uiLayerA] = m_oLayers[_uiLayerB];
		    m_oLayers[_uiLayerB] = poAuxLayer;
		}

		/// Retrieves the number of layers in the HUD element.
		uint uiNumLayers()
		{
			return(m_oLayers.size());
		}

        /// Perform processing over the object
		virtual void Accept(CFEHUDVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

	protected:
		
		CFEArray<CFEHUDObject*> m_oLayers;
		CFEArray<CFEHUDElementAction*> m_oActions;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
