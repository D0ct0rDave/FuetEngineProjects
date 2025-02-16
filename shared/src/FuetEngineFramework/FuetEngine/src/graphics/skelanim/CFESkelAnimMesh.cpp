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
#include "support/graphics/CFEMaterialMgr.h"
//-----------------------------------------------------------------------------
CFESkelAnimMesh::CFESkelAnimMesh(const CFEString& _sName) : CFESkelAnimNode(_sName), m_hMaterial(NULL), m_rHalfWidth(_0r)
{
    // Add 2 initial vertices
    m_oVX.push_back(CFEVect2::oZERO());
    m_oVX.push_back(CFEVect2::oZERO());
    m_oUV.push_back(CFEVect2::oZERO());
    m_oUV.push_back(CFEVect2::oZERO());
};
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::AttachBone(CFESkelAnimBone* _poBone)        
{
    m_poBones.push_back(_poBone);
    
    uint uiNumBones = m_poBones.size();
    uint uiTotalVertices = (uiNumBones-1)*4;

    if (uiTotalVertices == 0) return;

    while (m_oVX.size() < uiTotalVertices)
    {
        m_oVX.push_back(CFEVect2::oZERO());
        m_oUV.push_back(CFEVect2::oZERO());
    }

    // Recompute UVs
    FEReal rStep = _1r / (FEReal)(uiNumBones-1);
    FEReal rMargin = rStep * JOINT_MARGIN;
    FEReal rVal  = _0r;

    CFEVect2* poUV = &m_oUV[0];
    
    // First texcoords
    poUV[0].x = _0r;
    poUV[0].y = _0r;
    poUV[1].x = _1r;
    poUV[1].y = _0r;
    poUV+=2;

    // Inbetween texcoords
    for (uint i=1;i<uiNumBones-1;i++)
    {
        rVal += rStep;

        poUV[0].x = _0r;
        poUV[0].y = rVal - rMargin;
        poUV[1].x = _1r;
        poUV[1].y = poUV[0].y;
        poUV += 2;

        poUV[0].x = _0r;
        poUV[0].y = rVal + rMargin;
        poUV[1].x = _1r;
        poUV[1].y = poUV[0].y;
        poUV += 2;
    }
    
    // Last texcoords
    poUV[0].x = _0r;
    poUV[0].y = _1r;
    poUV[1].x = _1r;
    poUV[1].y = _1r;
}
//-----------------------------------------------------------------------------
void CFESkelAnimMesh::SetMaterial(FEHandler _hMaterial)
{
    m_hMaterial = _hMaterial;

    uint uiWidth = 0;
    CFEMaterialMgr::bGetMaterialProperty(m_hMaterial,"DiffuseMap.Width",(FEPointer)&uiWidth);
    m_rHalfWidth = _05r*(FEReal)uiWidth;
}
//-----------------------------------------------------------------------------
