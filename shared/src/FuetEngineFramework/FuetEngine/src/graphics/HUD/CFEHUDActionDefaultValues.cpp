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
#include "CFEHUDActionDefaultValues.h"
// -----------------------------------------------------------------------------
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"
#include "CFEHUDPSys.h"
// -----------------------------------------------------------------------------
class CHUDActionDefaultValues : public CFEHUDVisitor
{
    public:

		CHUDActionDefaultValues()
		{
		}
		
		static void Setup(CFEHUDObject* _poObj)
		{
			CHUDActionDefaultValues oSetup;
			_poObj->Accept(&oSetup);
		}

		virtual const CFEString sGetType() { return("CHUDActionDefaultValues"); };
		
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
        
        void CommonSetup(CFEHUDObject* _poObj);

    protected:
        
		CFEDictionary* m_poDictionary;
};
//-----------------------------------------------------------------------------
void CHUDActionDefaultValues::Visit(CFEHUDElementAction* _poObj){};
void CHUDActionDefaultValues::Visit(CFEHUDObjectAction* _poObj){};
void CHUDActionDefaultValues::Visit(CFEHUDLabel* _poObj) { CommonSetup(_poObj); };
void CHUDActionDefaultValues::Visit(CFEHUDIcon* _poObj) { CommonSetup(_poObj); };
void CHUDActionDefaultValues::Visit(CFEHUDRect* _poObj) { CommonSetup(_poObj); };
void CHUDActionDefaultValues::Visit(CFEHUDShape* _poObj) { CommonSetup(_poObj); };
void CHUDActionDefaultValues::Visit(CFEHUDPSys* _poObj) { CommonSetup(_poObj); };
void CHUDActionDefaultValues::Visit(CFEHUDObject* _poObj)  { CommonSetup(_poObj); };
//-----------------------------------------------------------------------------
void CHUDActionDefaultValues::CommonSetup(CFEHUDObject* _poObj)
{
	_poObj->SetCurVis(true);
	_poObj->SetCurAngle( _0r );
	_poObj->SetCurPos( CFEVect2::ZERO() );
	_poObj->SetCurScale( CFEVect2::ONE() );
	_poObj->SetCurColor( CFEColor::WHITE() );
	_poObj->SetCurDepth( _0r );
	_poObj->SetCurAction(-1);
}
//-----------------------------------------------------------------------------
void CHUDActionDefaultValues::Visit(CFEHUD* _poObj)
{
    for (uint i=0;i<_poObj->uiNumElements();i++)
        _poObj->poGetElement(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CHUDActionDefaultValues::Visit(CFEHUDElement* _poObj)
{
    for (uint i=0;i<_poObj->uiNumLayers();i++)
        _poObj->poGetLayer(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CHUDActionDefaultValues::Visit(CFEHUDGroup* _poObj)
{	
	CommonSetup(_poObj);

 	for (uint i=0;i<_poObj->uiNumObjs();i++)
		_poObj->poGetObject(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDActionDefaultValues::Setup(CFEHUDObject* _poObj)
{
	CHUDActionDefaultValues::Setup(_poObj);
}
//-----------------------------------------------------------------------------
