// ----------------------------------------------------------------------------
/*! \class CFESkelAnimMesh
 *  \brief This class represents a mesh object inside a skeletal hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
/*
	A mesh of 4 vertices and a list of bones that affect each vertice.

	bone1 -> o-----o  <- bone2
			 |     |
			 |	   |
	bone3    o-----o  <- bone4

	If there are not enough bones, remaining vertices won't be affected
	If there are too much bones, remaining bones won't affect any vertex.
*/
// ----------------------------------------------------------------------------
#include "CFESkelAnimMesh.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
CFESkelAnimMesh::CFESkelAnimMesh(const CFEString& _sName) : CFESkelAnimNode(_sName), m_hSpriteInst(NULL), m_poLastSpriteFrame(NULL)
{
	m_usIdx.push_back(0);
	m_usIdx.push_back(1);
	m_usIdx.push_back(2);
	m_usIdx.push_back(0);
	m_usIdx.push_back(2);
	m_usIdx.push_back(3);

    // Create the 4 vertices
	for (uint i=0;i<4;i++)
	{
		m_oVX.push_back( CFEVect2::ZERO() );
		m_oUV.push_back( CFEVect2::ZERO() );
		m_oSFVX.push_back( CFEVect2::ZERO() );
    }
};
//-----------------------------------------------------------------------------
CFESkelAnimMesh::~CFESkelAnimMesh()
{
	m_usIdx.clear();

	m_oVX.clear();
	m_oUV.clear();
	m_oSFVX.clear();

	m_poBones.clear();
}
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::AttachBone(CFESkelAnimBone* _poBone)        
{
    m_poBones.push_back(_poBone);

	if (_poBone != NULL)
	{
		m_oIniBonePos.push_back( _poBone->oGetPos() );
		m_rIniBoneAngle.push_back( _poBone->rGetAngle() );
	}
	else
	{
		m_oIniBonePos.push_back( CFEVect2::ZERO() );
		m_rIniBoneAngle.push_back( _0r );
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::SetAction(uint _uiAction)
{
	CFESpriteInstMgr::I()->SetAction(m_hSpriteInst,_uiAction);
}
//-----------------------------------------------------------------------------
uint CFESkelAnimMesh::uiGetAction()
{
	return (CFESpriteInstMgr::I()->uiGetAction(m_hSpriteInst) );
}
//-----------------------------------------------------------------------------
