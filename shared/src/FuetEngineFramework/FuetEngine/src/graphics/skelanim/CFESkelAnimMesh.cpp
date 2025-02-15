// ----------------------------------------------------------------------------
/*! \class CFESkelAnimMesh
 *  \brief This class represents a mesh object inside a skeletal hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimMesh.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
CFESkelAnimMesh::CFESkelAnimMesh(const CFEString& _sName) : CFESkelAnimNode(_sName), m_hSpriteInst(NULL)
{
    // Add 2 initial vertices
    m_oVX.push_back( CFEVect2::ZERO() );
    m_oVX.push_back( CFEVect2::ZERO() );
    m_oUV.push_back( CFEVect2::ZERO() );
    m_oUV.push_back( CFEVect2::ZERO() );
    m_oVC.push_back( CFEColor::WHITE() );
    m_oVC.push_back( CFEColor::WHITE() );
};
//-----------------------------------------------------------------------------
CFESkelAnimMesh::~CFESkelAnimMesh()
{
	m_oVX.clear();
	m_oUV.clear();
	m_oVC.clear();
	m_poBones.clear();
}
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::AttachBone(CFESkelAnimBone* _poBone)        
{
    m_poBones.push_back(_poBone);
    
    uint uiNumBones = m_poBones.size();
    uint uiTotalVertices = (uiNumBones-1)*4;

    if (uiTotalVertices == 0) return;
    while (m_oVX.size() < uiTotalVertices)
    {
        m_oVX.push_back(CFEVect2::ZERO());
        m_oUV.push_back(CFEVect2::ZERO());
		m_oVC.push_back(CFEColor::WHITE());
    }
}
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::SetAction(uint _uiAction)
{
	CFESpriteInstMgr::SetAction(m_hSpriteInst,_uiAction);
}
//-----------------------------------------------------------------------------
uint CFESkelAnimMesh::uiGetAction()
{
	return (CFESpriteInstMgr::uiGetAction(m_hSpriteInst) );
}
//-----------------------------------------------------------------------------
