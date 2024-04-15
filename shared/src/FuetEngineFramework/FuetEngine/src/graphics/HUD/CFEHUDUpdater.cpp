// ----------------------------------------------------------------------------
/*! \class CFEHUDUpdater
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDUpdater.h"

#include "CFEHUDAction.h"

#include "CFEHUD.h"
#include "CFEHUDElement.h"

#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDRect.h"
#include "CFEHUDIcon.h"
#include "CFEHUDShape.h"
#include "CFEHUDPSys.h"
#include "CFEHUDInstMgr.h"
#include "CFEHUDActionDefaultValues.h"

#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
#include "graphics/particlesys/CFEParticleSysInstMgr.h"

#include "types/CFEMatrix.h"
//-----------------------------------------------------------------------------
class CFEHUDActionUpdaterINT : public CFEHUDVisitor
{
	public:

        static void Update(CFEHUDObject* _poObj)
        {
			CFEHUDActionUpdaterINT oUpdaterINT;
			_poObj->Accept(&oUpdaterINT);
        }

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFEHUDVisitor::Visit;

        virtual void Visit(CFEHUD* _poObj) {};
        virtual void Visit(CFEHUDElement* _poObj) {};
        virtual void Visit(CFEHUDObject* _poObj) {};
        virtual void Visit(CFEHUDElementAction* _poObj) {};
        virtual void Visit(CFEHUDObjectAction* _poObj) {};

		virtual const CFEString sGetType() { return(CFEString::sNULL()); };

		virtual void Visit(CFEHUDPSys* _poObj)
		{
			// Update the values that doesn't depend on parent changes.
			if (! _poObj->bIsVisible())
				CFEParticleSysInstMgr::I()->Disable(_poObj->hGetPSys());
			else
				CFEParticleSysInstMgr::I()->Enable(_poObj->hGetPSys());
		}
};
//-----------------------------------------------------------------------------
class CFEHUDUpdaterINT : public CFEHUDVisitor
{
	public:

        static void Update(CFEHUD* _poHud,FEReal _rDeltaT)
        {
			CFEHUDUpdaterINT oUpdaterINT;
			oUpdaterINT.m_rDeltaT = _rDeltaT;
			_poHud->Accept(&oUpdaterINT);
        }

        static void Update(CFEHUDElement* _poElement,FEReal _rDeltaT)
        {
			CFEHUDUpdaterINT oUpdaterINT;
			oUpdaterINT.m_rDeltaT = _rDeltaT;
			_poElement->Accept(&oUpdaterINT);
        }

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFEHUDVisitor::Visit;
		
		virtual const CFEString sGetType() { return(CFEString::sNULL()); };

		virtual void Visit(CFEHUDElement* _poObj)
		{
			// OPTIMIZATION: process only objects, not element actions.
		    for (uint i=0;i<_poObj->uiNumLayers();i++)
				if (_poObj->poGetLayer(i) != NULL)
					_poObj->poGetLayer(i)->Accept(this);
		}

		virtual void Visit(CFEHUDGroup* _poObj)
		{
			if (! _poObj->bIsVisible()) return;
			CFEHUDVisitor::Visit(_poObj);
		}

		virtual void Visit(CFEHUDIcon* _poObj)
		{
			if (! _poObj->bIsVisible()) return;
			CFESpriteInstMgr::I()->Update(_poObj->hGetIcon(),m_rDeltaT);
		}

		virtual void Visit(CFEHUDShape* _poObj)
		{
			if (! _poObj->bIsVisible()) return;
			CFEMeshInstMgr::I()->Update(_poObj->hGetMesh(),m_rDeltaT);
		}

		virtual void Visit(CFEHUDPSys* _poObj)
		{
			// Update the values that doesn't depend on parent changes.
			if (! _poObj->bIsVisible())
				CFEParticleSysInstMgr::I()->Disable(_poObj->hGetPSys());
			else
				CFEParticleSysInstMgr::I()->Enable(_poObj->hGetPSys());
		}

	protected:

		FEReal m_rDeltaT;
};
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Process(CFEHUDElementAction* _poObj,FEReal _rTime)
{
	if (_poObj == NULL) return;

	FEReal rTime = _rTime;

		/*
		// Update the element anim
		FEReal rMaxActionTime = _poObj->rGetMaxActionTime();
		FEReal rActionTime    = _poObj->rGetActionTime();

		// To prevent overflows
		if (rActionTime >= _0r)
		{
			// One shot anims ...
			rTime = CFEMath::rMin(_rTime,rMaxActionTime);
		}
		else
		{
			// loop anims...

			// WARNING: This is not valid, if one object has a loop animation of 2 seconds and other of 3,
			// when time = 3 it will restart all the animations
			// m_rTime = CFEMath::rMod(m_rTime,rMaxActionTime);
			rTime = CFEMath::rMod(_rTime,1000.0f);
		}
		*/

		for (uint i=0;i<_poObj->uiNumActions();i++)
			Process( _poObj->poGetAction(i),rTime);
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Process(CFEHUDObjectAction* _poObj,FEReal _rTime)
{
	if (_poObj == NULL) return;

    _poObj->oGetValue(_rTime);
    CFEHUDActionUpdaterINT::Update(_poObj->poGetHUDObject());
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Update(CFEHUD* _poHUD,FEReal _rDeltaT)
{
	CFEHUDUpdaterINT::Update(_poHUD,_rDeltaT);
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::Update(CFEHUDElement* _poElement,FEReal _rDeltaT)
{
	CFEHUDUpdaterINT::Update(_poElement,_rDeltaT);
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::RestartActions(CFEHUDElementAction* _poObj)
{
	if (_poObj == NULL) return;

	for (uint i=0;i<_poObj->uiNumActions();i++)
		if (_poObj->poGetAction(i) != NULL)
			_poObj->poGetAction(i)->m_oEventFunc.Reset();
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::SetActionDefaultValues(CFEHUDObject* _poObj)
{
	CFEHUDActionDefaultValues::Setup(_poObj);
}
//-----------------------------------------------------------------------------
void CFEHUDUpdater::ResetHUD(CFEHUD* _poHUD)
{
	if (_poHUD==NULL) return;

	for (uint e=0;e<_poHUD->uiNumElements();e++)
	{
		CFEHUDElement* poElem = _poHUD->poGetElement(e);
		if (poElem == NULL) continue;

		for (uint l=0;l<poElem->uiNumLayers();l++)
			CFEHUDUpdater::SetActionDefaultValues( poElem->poGetLayer(l) );

		for (uint a=0;a<poElem->uiNumActions();a++)
			CFEHUDUpdater::RestartActions( poElem->poGetAction(a) );
	}
}
//-----------------------------------------------------------------------------
