// ----------------------------------------------------------------------------
/*! \class CFEHUDRectGen
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDRectGen.h"

#include "CFEHUDRenderer.h"
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
#include "graphics/font/CFEFont.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
#include "graphics/mesh/CFEMesh.h"
//-----------------------------------------------------------------------------
inline FEReal rGetRenderDepth(FEReal _rDepth,FEReal _rDepthFact,CFEHUDObject* _poObj)
{
	return(_rDepth + _poObj->rGetDepth()*_rDepthFact);
}
//-----------------------------------------------------------------------------
CFERect oComputeBoundingRect(CFEVect2* _poVXs, uint _uiNumVXs,const CFEMatrix& _oMat)
{
	CFERect oRect;
	oRect.m_oIni.x =  _INFr;
	oRect.m_oIni.y =  _INFr;
	oRect.m_oEnd.x = -_INFr;
	oRect.m_oEnd.y = -_INFr;

	for (uint i=0;i<_uiNumVXs;i++)
	{
		// transform vertices
		CFEVect2 oNewPos = _oMat.Transform( _poVXs[i] );
		
		// regenerate rect
		oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x,oNewPos.x);
		oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y,oNewPos.y);
		oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x,oNewPos.x);
		oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y,oNewPos.y);
	}

	return(oRect);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDObject* _poObj)
{

}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDGroup* _poObj)
{
	CFEMatrix oOldMat  = m_oTransf;

	m_oTransf.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_oTransf.Rotate(_poObj->rGetAngle());
	m_oTransf.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	if (m_poTarget == (FEPointer)_poObj)
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
			m_poTarget = (FEPointer)_poObj->poGetObject(i);
			if (m_poTarget == NULL) continue;
			
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

	m_oTransf = oOldMat;
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDLabel* _poObj)
{
	if( m_poTarget != (FEPointer)_poObj) return;
	m_bTargetFound = true;

    if (_poObj->poGetFont() == NULL)
    {
		GenericRectGen(_poObj);
		return;
	}

	FEReal rLen    = _poObj->poGetFont()->rStringLen(_poObj->sGetText());
	FEReal rHeight = _poObj->poGetFont()->rDefCharHeight();
	FEReal rXOfs = 0;
	FEReal rYOfs = 0;

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

	CFEVect2 oVX[4];
	oVX[0].x = rXOfs;
	oVX[0].y = rYOfs;
	oVX[1].x = rXOfs + rLen;
	oVX[1].y = rYOfs;
	oVX[2].x = rXOfs + rLen;
	oVX[2].y = rYOfs + rHeight;
	oVX[3].x = rXOfs;
	oVX[3].y = rYOfs + rHeight;

	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect = oComputeBoundingRect(oVX,4,oMat);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDIcon* _poObj)
{
	if( m_poTarget != (FEPointer)_poObj) return;
	m_bTargetFound = true;

	if (_poObj->hGetIcon() == NULL)
	{
		GenericRectGen(_poObj);
		return;
	}

	CFESpriteInstMgr::I()->SetPos(_poObj->hGetIcon(),_poObj->oGetPos());
	CFESpriteInstMgr::I()->SetScale(_poObj->hGetIcon(),_poObj->oGetScale());
	CFESpriteInstMgr::I()->SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());

	CFEVect2 oVXs[4];
	CFESpriteInstMgr::I()->GetGeometry(_poObj->hGetIcon(),oVXs);

	m_oRect = oComputeBoundingRect(oVXs, 4,m_oTransf);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDRect* _poObj)
{
	if( m_poTarget != (FEPointer)_poObj) return;
	m_bTargetFound = true;

	FEReal rWidth  =  _poObj->rGetWidth();
	FEReal rHeight =  _poObj->rGetHeight();
	FEReal rXOfs   = -_poObj->oGetPivot().x  * rWidth;
	FEReal rYOfs   = -_poObj->oGetPivot().y  * rHeight;

	/*
	m_oRect.m_oIni.x = rXOfs;
	m_oRect.m_oIni.y = rYOfs;
	m_oRect.m_oEnd.x = rXOfs + rWidth;
	m_oRect.m_oEnd.y = rYOfs + rHeight;
	*/
	
	// Create rect vertices
	CFEVect2 oVX[4];
	oVX[0].x = rXOfs;
	oVX[0].y = rYOfs;
	oVX[1].x = rXOfs + rWidth;
	oVX[1].y = rYOfs;
	oVX[2].x = rXOfs + rWidth;
	oVX[2].y = rYOfs + rHeight;
	oVX[3].x = rXOfs;
	oVX[3].y = rYOfs + rHeight;

	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect = oComputeBoundingRect(oVX,4,oMat);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDShape* _poObj)
{
	if( m_poTarget != (FEPointer)_poObj) return;
	m_bTargetFound = true;

	CFEMeshInst* poMeshInst = (CFEMeshInst*)_poObj->hGetMesh();

	if ((poMeshInst == NULL) || (poMeshInst->m_oVX.size() < 2))
	{
		GenericRectGen(_poObj);
		return;
	}
	
	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect = oComputeBoundingRect(&poMeshInst->m_oVX[0],poMeshInst->m_oVX.size(),oMat);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDPSys* _poObj)
{
	if( m_poTarget != (FEPointer)_poObj) return;
	m_bTargetFound = true;

	GenericRectGen(_poObj);
}
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUD* _poObj)
{
	if( m_poTarget == (FEPointer)_poObj)
	{
		CFERect oRect;
		oRect.m_oIni.x =  _INFr;
		oRect.m_oIni.y =  _INFr;
		oRect.m_oEnd.x = -_INFr;
		oRect.m_oEnd.y = -_INFr;

		m_bTargetFound = true;

		for (uint i=0;i<_poObj->uiNumElements();i++)
		{
			// force the construction of the group rect using this trick.
			m_poTarget = (FEPointer)_poObj->poGetElement(i);
			
			// recurse into childs.
			_poObj->poGetElement(i)->Accept(this);

			oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x,m_oRect.m_oIni.x);
			oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y,m_oRect.m_oIni.y);
			oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x,m_oRect.m_oEnd.x);
			oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y,m_oRect.m_oEnd.y);
		}

		m_oRect = oRect;
	}
	else
	{
		for (uint i=0;i<_poObj->uiNumElements();i++)
		{
			_poObj->poGetElement(i)->Accept(this);
			if( m_bTargetFound ) return;
		}
	}
};
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDElement* _poObj)
{
	if( m_poTarget == (FEPointer)_poObj)
	{
		CFERect oRect;
		oRect.m_oIni.x =  _INFr;
		oRect.m_oIni.y =  _INFr;
		oRect.m_oEnd.x = -_INFr;
		oRect.m_oEnd.y = -_INFr;

		m_bTargetFound = true;

		for (uint i=0;i<_poObj->uiNumLayers();i++)
		{
			// force the construction of the group rect using this trick.
			m_poTarget = (FEPointer)_poObj->poGetLayer(i);

			// recurse into childs.
			_poObj->poGetLayer(i)->Accept(this);

			oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x,m_oRect.m_oIni.x);
			oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y,m_oRect.m_oIni.y);
			oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x,m_oRect.m_oEnd.x);
			oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y,m_oRect.m_oEnd.y);
		}

		m_oRect = oRect;
	}
	else
	{
		for (uint i=0;i<_poObj->uiNumLayers();i++)
		{
			_poObj->poGetLayer(i)->Accept(this);
			if( m_bTargetFound ) return;
		}
	}
};
//-----------------------------------------------------------------------------
void CFEHUDRectGen::Visit(CFEHUDElementAction* _poObj)
{
};

void CFEHUDRectGen::Visit(CFEHUDObjectAction* _poObj)
{
};
//-----------------------------------------------------------------------------
void CFEHUDRectGen::GenericRectGen(CFEHUDObject* _poObj)
{
	CFEMatrix oMat = m_oTransf;

	oMat.Translate(_poObj->oGetPos().x,_poObj->oGetPos().y);
	oMat.Rotate(_poObj->rGetAngle());
	oMat.Scale(_poObj->oGetScale().x,_poObj->oGetScale().y);

	m_oRect.m_oIni.x = -100;
	m_oRect.m_oIni.y = -100;
	m_oRect.m_oEnd.x =  100;
	m_oRect.m_oEnd.y =  100;

	m_oRect.m_oIni = oMat.Transform(m_oRect.m_oIni);
	m_oRect.m_oEnd = oMat.Transform(m_oRect.m_oEnd);

	// due to scaling or rotation, ini can be greater than end (!) for instance scale X by -1
	if (m_oRect.m_oIni.x > m_oRect.m_oEnd.x) std::swap(m_oRect.m_oIni.x,m_oRect.m_oEnd.x);
	if (m_oRect.m_oIni.y > m_oRect.m_oEnd.y) std::swap(m_oRect.m_oIni.y,m_oRect.m_oEnd.y);	
}
//-----------------------------------------------------------------------------
CFEHUDRectGen::CFEHUDRectGen(FEPointer _poTarget)
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
	CFEHUDRectGen oRG((FEPointer)_poObj);

	if (_poParent != NULL) 
		_poParent ->Accept(&oRG);
	else
		_poObj->Accept(&oRG);

	return(oRG.m_oRect);
}
//-----------------------------------------------------------------------------        
CFERect CFEHUDRectGen::oGetRect(CFEHUD* _poObj)
{
	CFEHUDRectGen oRG((FEPointer)_poObj);
	_poObj->Accept(&oRG);
	return(oRG.m_oRect);
}
//-----------------------------------------------------------------------------
