// ----------------------------------------------------------------------------
/*! \class CFEHUDRectGen
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDRectGen.h"

#include "CFEHUDRenderer.h"
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"

#include "graphics/font/CFEFont.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDObject* _poObj)
{
	_poObj->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDGroup* _poObj)
{
	m_oTransf.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_oTransf.Rotate(_poObj->rGetAngle());
	m_oTransf.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	if( m_poTarget == _poObj)
	{
		CFERect oRect;
		oRect.m_oIni.x =  _INFr;
		oRect.m_oIni.y =  _INFr;
		oRect.m_oEnd.x = -_INFr;
		oRect.m_oEnd.y = -_INFr;

		m_bTargetFound = true;

		for (uint i=0;i<_poObj->uiNumObjs();i++)
		{
			// force the construction of the group rect using this trick.
			m_poTarget = _poObj->poGetObject(i);
			
			// recurse into childs.
			_poObj->poGetObject(i)->Accept(this);

			oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x,m_oRect.m_oIni.x);
			oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y,m_oRect.m_oIni.y);
			oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x,m_oRect.m_oEnd.x);
			oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y,m_oRect.m_oEnd.y);
		}

		m_oRect = oRect;
	}
	else
	{
		for (uint i=0;i<_poObj->uiNumObjs();i++)
		{
			_poObj->poGetObject(i)->Accept(this);
			if( m_bTargetFound ) return;
		}
	}
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDLabel* _poObj)
{
	if( m_poTarget != _poObj) return;
	m_bTargetFound = true;

    if (_poObj->poGetFont() == NULL) return;

	FEReal rLen    = _poObj->poGetFont()->rStringLen(_poObj->sGetText());
	FEReal rHeight = _poObj->poGetFont()->rDefCharHeight();
	FEReal rXOfs;
	FEReal rYOfs;

	switch (_poObj->eGetHAlignment())
	{
		case THAM_LEFT:     rXOfs = 0;			break;
		case THAM_RIGHT:    rXOfs = -rLen;		break;
		case THAM_CENTER:   rXOfs = -rLen*_05r;	break;
	}
	switch (_poObj->eGetVAlignment())
	{
		case TVAM_TOP:      rYOfs = 0;				break;
		case TVAM_BOTTOM:   rYOfs = -rHeight;		break;
		case TVAM_CENTER:   rYOfs = -rHeight*_05r;	break;
	}

	m_oRect.m_oIni.x = rXOfs;
	m_oRect.m_oIni.y = rYOfs;
	m_oRect.m_oEnd.x = rXOfs + rLen;
	m_oRect.m_oEnd.y = rYOfs + rHeight;

	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect.m_oIni = oMat.Transform(m_oRect.m_oIni);
	m_oRect.m_oEnd = oMat.Transform(m_oRect.m_oEnd);	
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDIcon* _poObj)
{
	if( m_poTarget != _poObj) return;
	m_bTargetFound = true;

	CFEVect2 oVXs[4];
	CFESpriteInstMgr::SetPos(_poObj->hGetIcon(),_poObj->oGetPos());
	CFESpriteInstMgr::SetScale(_poObj->hGetIcon(),_poObj->oGetScale());
	CFESpriteInstMgr::SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());
	CFESpriteInstMgr::GetGeometry(_poObj->hGetIcon(),oVXs);

	FEReal rXMin = oVXs[0].x;
	FEReal rXMax = oVXs[0].x;
	FEReal rYMin = oVXs[0].y;
	FEReal rYMax = oVXs[0].y;

	for (uint i=1;i<4;i++)
	{
		rXMin = CFEMath::rMin(rXMin,oVXs[i].x);
		rXMax = CFEMath::rMax(rXMax,oVXs[i].x);
		rYMin = CFEMath::rMin(rYMin,oVXs[i].y);
		rYMax = CFEMath::rMax(rYMax,oVXs[i].y);
	}
	
	m_oRect.m_oIni.x = rXMin;
	m_oRect.m_oIni.y = rYMin;
	m_oRect.m_oEnd.x = rXMax;
	m_oRect.m_oEnd.y = rYMax;

	m_oRect.m_oIni = m_oTransf.Transform(m_oRect.m_oIni);
	m_oRect.m_oEnd = m_oTransf.Transform(m_oRect.m_oEnd);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDRect* _poObj)
{
	if( m_poTarget != _poObj) return;
	m_bTargetFound = true;

	FEReal rWidth  =  _poObj->rGetWidth();
	FEReal rHeight =  _poObj->rGetHeight();
	FEReal rXOfs   = -_poObj->oGetPivot().x  * rWidth;
	FEReal rYOfs   = -_poObj->oGetPivot().y  * rHeight;

	m_oRect.m_oIni.x = rXOfs;
	m_oRect.m_oIni.y = rYOfs;
	m_oRect.m_oEnd.x = rXOfs + rWidth;
	m_oRect.m_oEnd.y = rYOfs + rHeight;

	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect.m_oIni = oMat.Transform(m_oRect.m_oIni);
	m_oRect.m_oEnd = oMat.Transform(m_oRect.m_oEnd);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDShape* _poObj)
{
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUD* _poObj)
{
};

void CFEHUDRectGen::Visit(CFEHUDElement* _poObj)
{
};

void CFEHUDRectGen::Visit(CFEHUDElementAction* _poObj)
{
};

void CFEHUDRectGen::Visit(CFEHUDObjectAction* _poObj)
{
};
//-----------------------------------------------------------------------------
CFEHUDRectGen::CFEHUDRectGen(CFEHUDObject* _poTarget)
{
	m_poTarget = _poTarget;
	m_bTargetFound = false;
	m_oTransf.LoadIdentity();

	m_oRect.m_oIni.x =  _INFr;
	m_oRect.m_oIni.y =  _INFr;
	m_oRect.m_oEnd.x = -_INFr;
	m_oRect.m_oEnd.y = -_INFr;
}
//-----------------------------------------------------------------------------
CFERect CFEHUDRectGen::oGetRect(CFEHUDGroup* _poParent,CFEHUDObject* _poObj)
{
	CFEHUDRectGen oRG(_poObj);

	if (_poParent != NULL) 
		_poParent ->Accept(&oRG);
	else
		_poObj->Accept(&oRG);

	return(oRG.m_oRect);
}
//-----------------------------------------------------------------------------
