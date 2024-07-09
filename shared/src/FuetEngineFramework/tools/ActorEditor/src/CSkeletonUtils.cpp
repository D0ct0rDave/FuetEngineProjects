//---------------------------------------------------------------------------
//
// Name:        CSkeleton.h
// Author:      David Márquez de la Cruz
// Created:     7/10/2010
// Description: CSkeleton class declaration
//
//---------------------------------------------------------------------------
#include "CSkeletonUtils.h"
//----------------------------------------------------------------------------
CLocalPosGetter::CLocalPosGetter(const CFEVect2& _oAbsPos,const CFEString& _sObjName,FEReal _rTime,CFESkelAnim* _poAnim) : m_oAbsPos(_oAbsPos), m_sObjName(_sObjName), m_rTime(_rTime), m_poAnim(_poAnim)
{
	m_oMat.LoadIdentity();
	m_bFound	= false;
	m_oPos		= CFEVect2::ZERO();
	
}		
//---------------------------------------------------------------------------
CFEVect2 CLocalPosGetter::oGetPos(const CFEVect2& _oAbsPos,const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst)
{
	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(_hAnimInst);
	CFESkelAnim*     poAnim = CFESkelAnimInstMgr::I()->poGetAnim(_hAnimInst);

	CLocalPosGetter oObj(_oAbsPos,_sObjName,_rTime,poAnim);
	poNode->Accept(&oObj);
	return(oObj.m_oPos);
}
//---------------------------------------------------------------------------
CFESkelAnimNodeAction* CLocalPosGetter::poGetNodeAction(CFESkelAnimNode* _poNode)
{
	CFESkelAnimAction* poAction = m_poAnim->poGetAnimActionSet()->poGetAction(0);

	for (uint i=0;i<poAction->uiNumNodeActions();i++)
	{
		CFESkelAnimNodeAction* poNA = poAction->poGetNodeAction(i);
		if (poNA != NULL)
		{
			int iIdx = poNA->uiGetNodeIdx();

			if ((iIdx != -1) && (m_poAnim->m_oNodeTab[iIdx]->sGetName() == _poNode->sGetName()))
				return( poNA );
		}
	}

	return(NULL);
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimGroup* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
		return;
	}

	CFEMatrix oOldMatrix = m_oMat;
	
		FEReal rAngle	= _poObj->rGetAngle();
		CFEVect2 oPos	= _poObj->oGetPos();

		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);

		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimBone* _poObj)
{
	CFEMatrix oOldMatrix = m_oMat;

		if (_poObj->sGetName() == m_sObjName)
		{
			FEReal rAngle	= _poObj->rGetAngle();
			CFEVect2 oPos	= _poObj->oGetBonePos() + _poObj->oGetPos();
			
			m_oMat.Translate(oPos.x,oPos.y);
			m_oMat.Rotate(rAngle);

			#ifdef USE_SKELANIM_SCALE_COLOR
			CFEVect2 oScale = _poObj->oGetScale();
			m_oMat.Scale(oScale.x,oScale.y);
			#endif

			m_oPos   = oComputePos( _poObj );
			m_bFound = true;
		}
		else
		{
			FEReal rAngle	= _poObj->rGetAngle();
			CFEVect2 oPos	= _poObj->oGetBonePos()  + _poObj->oGetPos();

			m_oMat.Translate(oPos.x,oPos.y);
			m_oMat.Rotate(rAngle);

			#ifdef USE_SKELANIM_SCALE_COLOR
			CFEVect2 oScale = _poObj->oGetScale();
			m_oMat.Scale(oScale.x,oScale.y);
			#endif

			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				if (_poObj->poGetChild(i) != NULL)
					_poObj->poGetChild(i)->Accept(this);

				if (m_bFound)
					break;
			}
			
		}
			
	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimSpriteModel* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimSprite* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimMeshModel* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CLocalPosGetter::Visit(CFESkelAnimMesh* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
CAbsPosGetter::CAbsPosGetter(const CFEVect2& _oLocalPos,const CFEString& _sObjName,FEReal _rTime,CFESkelAnim* _poAnim)  : m_oLocalPos(_oLocalPos), m_sObjName(_sObjName), m_rTime(_rTime), m_poAnim(_poAnim)
{
	m_oMat.LoadIdentity();
	m_bFound	= false;
	m_oPos		= CFEVect2::ZERO();
}
//---------------------------------------------------------------------------
CFESkelAnimNodeAction* CAbsPosGetter::poGetNodeAction(CFESkelAnimNode* _poNode)
{
	CFESkelAnimAction* poAction = m_poAnim->poGetAnimActionSet()->poGetAction(0);

	for (uint i=0;i<poAction->uiNumNodeActions();i++)
	{
		CFESkelAnimNodeAction* poNA = poAction->poGetNodeAction(i);
		if (poNA != NULL)
		{
			int iIdx = poNA->uiGetNodeIdx();

			if ((iIdx != -1) && (m_poAnim->m_oNodeTab[iIdx]->sGetName() == _poNode->sGetName()))
				return( poNA );
		}
	}

	return(NULL);
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimGroup* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
		return;
	}

	CFEMatrix oOldMatrix = m_oMat;

		FEReal rAngle	= _poObj->rGetAngle();
		CFEVect2 oPos	= _poObj->oGetPos();

		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimBone* _poObj)
{
	CFEMatrix oOldMatrix = m_oMat;

		if (_poObj->sGetName() == m_sObjName)
		{
			CFEVect2 oPos	= _poObj->oGetBonePos();
			FEReal rAngle	= _poObj->rGetAngle();
			
			if (m_bOfPos == false)
			{
				// if we don't want the absolute value of one local position, then we want the absolute position
				// of the bone itself (including the position animation)
				
				
				oPos += _poObj->oGetPos();
				// m_oLocalPos += _poObj->oGetPos(); // <--- no acabo de entender porque pero esto es lo correcto...
			}
			else
			{
				// we don't want the position animation to be counted as relative to the given local position
			}

			m_oMat.Translate(oPos.x,oPos.y);
			m_oMat.Rotate(rAngle);

			#ifdef USE_SKELANIM_SCALE_COLOR
			CFEVect2 oScale = _poObj->oGetScale();			
			m_oMat.Scale(oScale.x,oScale.y);
			#endif

			m_oPos   = m_oMat.Transform( m_oLocalPos );
			m_bFound = true;
		}
		else
		{
			CFEVect2 oPos	= _poObj->oGetBonePos() + _poObj->oGetPos();
			FEReal rAngle	= _poObj->rGetAngle();
			

			m_oMat.Translate(oPos.x,oPos.y);
			m_oMat.Rotate(rAngle);
			
			#ifdef USE_SKELANIM_SCALE_COLOR
			CFEVect2 oScale = _poObj->oGetScale();
			m_oMat.Scale(oScale.x,oScale.y);
			#endif

			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				if (_poObj->poGetChild(i) != NULL)
					_poObj->poGetChild(i)->Accept(this);

				if (m_bFound)
					break;
			}
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimSpriteModel* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimSprite* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimMeshModel* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
void CAbsPosGetter::Visit(CFESkelAnimMesh* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
	}
}
//---------------------------------------------------------------------------
CFEVect2 CAbsPosGetter::oGetPos(const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst)
{
	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(_hAnimInst);
	CFESkelAnim*     poAnim = CFESkelAnimInstMgr::I()->poGetAnim(_hAnimInst);

	CAbsPosGetter oObj(CFEVect2::ZERO(),_sObjName,_rTime,poAnim);
	oObj.m_bOfPos = false;
	poNode->Accept(&oObj);
	return(oObj.m_oPos);
}
//----------------------------------------------------------------------------
CFEVect2 CAbsPosGetter::oGetPos(const CFEVect2& _oLocalPos,const CFEString& _sObjName,FEReal _rTime,FEHandler _hAnimInst)
{
	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::I()->poGetInstancedNode(_hAnimInst);
	CFESkelAnim*     poAnim = CFESkelAnimInstMgr::I()->poGetAnim(_hAnimInst);

	CAbsPosGetter oObj(_oLocalPos,_sObjName,_rTime,poAnim);
	oObj.m_bOfPos = true;

	poNode->Accept(&oObj);
	return(oObj.m_oPos);
}
//----------------------------------------------------------------------------
CBoneLocalPosGetter::CBoneLocalPosGetter(const CFEVect2& _oAbsPos,const CFEString& _sObjName) : m_oAbsPos(_oAbsPos), m_sObjName(_sObjName)
{
	m_oMat.LoadIdentity();
	m_bFound	= false;
	m_oPos		= CFEVect2::ZERO();
}		
//---------------------------------------------------------------------------
CFEVect2 CBoneLocalPosGetter::oGetPos(const CFEVect2& _oAbsPos,CFESkelAnimNode* _poNode,const CFEString& _sObjName)
{
	CBoneLocalPosGetter oObj(_oAbsPos,_sObjName);
	_poNode->Accept(&oObj);
	return(oObj.m_oPos);
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimGroup* _poObj)
{
	CFEMatrix oOldMatrix = m_oMat;

		FEReal rAngle	= _poObj->rGetAngle();
		CFEVect2 oPos	= _poObj->oGetPos();
		
		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimBone* _poObj)
{
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = oComputePos( _poObj );
		m_bFound = true;
		return;
	}

	CFEMatrix oOldMatrix = m_oMat;

		FEReal rAngle	= _poObj->rGetAngle();
		CFEVect2 oPos	= _poObj->oGetBonePos() + _poObj->oGetPos();
		
		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimSpriteModel* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimSprite* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimMeshModel* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneLocalPosGetter::Visit(CFESkelAnimMesh* _poObj)
{
}
//----------------------------------------------------------------------------
CBoneAbsPosGetter::CBoneAbsPosGetter(const CFEString& _sObjName) : m_sObjName(_sObjName)
{
	m_oMat.LoadIdentity();
	m_bFound	= false;
	m_oPos		= CFEVect2::ZERO();
}		
//---------------------------------------------------------------------------
CFEVect2 CBoneAbsPosGetter::oGetPos(CFESkelAnimNode* _poNode,const CFEString& _sObjName)
{
	CBoneAbsPosGetter oObj(_sObjName);
	_poNode->Accept(&oObj);
	return(oObj.m_oPos);
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimGroup* _poObj)
{
	CFEMatrix oOldMatrix = m_oMat;

		FEReal rAngle	= _poObj->rGetAngle();
		CFEVect2 oPos	= _poObj->oGetPos();

		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);
		
		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimBone* _poObj)
{
	CFEVect2 oPos	= _poObj->oGetBonePos() + _poObj->oGetPos();
	
	if (_poObj->sGetName() == m_sObjName)
	{
		m_oPos   = m_oMat.Transform( oPos );
		m_bFound = true;
		return;
	}

	CFEMatrix oOldMatrix = m_oMat;

		FEReal rAngle	= _poObj->rGetAngle();

		m_oMat.Translate(oPos.x,oPos.y);
		m_oMat.Rotate(rAngle);

		#ifdef USE_SKELANIM_SCALE_COLOR
		CFEVect2 oScale = _poObj->oGetScale();
		m_oMat.Scale(oScale.x,oScale.y);
		#endif

		for (uint i=0;i<_poObj->uiNumChildren();i++)
		{
			if (_poObj->poGetChild(i) != NULL)
				_poObj->poGetChild(i)->Accept(this);

			if (m_bFound)
				break;
		}

	m_oMat = oOldMatrix;
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimSpriteModel* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimSprite* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimMeshModel* _poObj)
{
}
//---------------------------------------------------------------------------
void CBoneAbsPosGetter::Visit(CFESkelAnimMesh* _poObj)
{
}
//---------------------------------------------------------------------------
