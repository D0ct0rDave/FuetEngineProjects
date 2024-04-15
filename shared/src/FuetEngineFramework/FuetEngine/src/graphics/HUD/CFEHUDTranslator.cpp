// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDTranslator.h"
#include "support/local/CFEDictionary.h"
// -----------------------------------------------------------------------------
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
// -----------------------------------------------------------------------------
class CHUDTranslator : public CFEHUDVisitor
{
    public:

		CHUDTranslator(CFEDictionary* _poDictionary)
		{
			m_poDictionary = _poDictionary;
		}

        static void Translate(CFEHUD* _poHUD,CFEDictionary* _poDict)
        {
			CHUDTranslator oTranslator(_poDict);
			_poHUD->Accept(&oTranslator);
        }

		virtual const CFEString sGetType() { return(CFEString::sNULL()); };
		
        virtual void Visit(CFEHUDObject* _poObj);
        /// 
        virtual void Visit(CFEHUDGroup* _poObj);
        /// 
        virtual void Visit(CFEHUDLabel* _poObj);
        /// 
        virtual void Visit(CFEHUDIcon* _poObj);
        /// 
        virtual void Visit(CFEHUDRect* _poObj);
        /// 
        virtual void Visit(CFEHUDShape* _poObj);
        /// 
        virtual void Visit(CFEHUDPSys* _poObj);

        virtual void Visit(CFEHUD* _poObj);
        /// 
        virtual void Visit(CFEHUDElement* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        ///        
        virtual void Visit(CFEHUDObjectAction* _poObj);

    protected:
        
		CFEDictionary* m_poDictionary;
};
//-----------------------------------------------------------------------------
void CHUDTranslator::Visit(CFEHUDIcon*){}
void CHUDTranslator::Visit(CFEHUDRect*){}
void CHUDTranslator::Visit(CFEHUDShape*){}
void CHUDTranslator::Visit(CFEHUDPSys*){}
void CHUDTranslator::Visit(CFEHUDElementAction*){}
void CHUDTranslator::Visit(CFEHUDObjectAction*){}
void CHUDTranslator::Visit(CFEHUDObject*){}
//-----------------------------------------------------------------------------
void CHUDTranslator::Visit(CFEHUD* _poObj)
{
    for (uint i=0;i<_poObj->uiNumElements();i++)
        _poObj->poGetElement(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CHUDTranslator::Visit(CFEHUDElement* _poObj)
{
    for (uint i=0;i<_poObj->uiNumLayers();i++)
        _poObj->poGetLayer(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CHUDTranslator::Visit(CFEHUDGroup* _poObj)
{	
	for (uint i=0;i<_poObj->uiNumObjs();i++)
		_poObj->poGetObject(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CHUDTranslator::Visit(CFEHUDLabel* _poObj)
{	
	// Here is the chicha :)
	_poObj->SetText( m_poDictionary->sGetString(_poObj->sGetName(),_poObj->sGetText()) );	
}
//-----------------------------------------------------------------------------
void CFEHUDTranslator::Translate(CFEHUD* _poHUD,CFEDictionary* _poDict)
{
	CHUDTranslator::Translate(_poHUD,_poDict);
}
//-----------------------------------------------------------------------------
