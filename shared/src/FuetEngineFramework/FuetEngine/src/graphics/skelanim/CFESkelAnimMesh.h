// ----------------------------------------------------------------------------
/*! \class CFESkelAnimRenderer
 *  \brief This class represents a mesh object inside a skeletal hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimMeshH
#define CFESkelAnimMeshH
// ----------------------------------------------------------------------------
#include "CFESkelAnimBone.h"
// ----------------------------------------------------------------------------
const FEReal JOINT_MARGIN = _1r / _32r;
// ----------------------------------------------------------------------------
class CFESkelAnimMesh : public CFESkelAnimNode
{
    public:

        /// Default constructor for the class.
        CFESkelAnimMesh(const CFEString& _sName);

        /// Attach a new influencer bone to this mesh.
        void AttachBone(CFESkelAnimBone* _poBone);

        /// Retrieves the bone attached to this mesh with the given index.
        CFESkelAnimBone* poGetAttachedBone(uint _uiBoneIdx)
        {
            return(m_poBones[ _uiBoneIdx] );
        }

        /// Retrieves the total number of bones influencing this mesh.
        uint uiGetNumAttachedBones()
        {
            return(m_poBones.size());
        }

        /// Retrieves the address of the vertex array.
        CFEVect2* poGetVX()
        {
            return( &m_oVX[0] );
        }

        /// Retrieves the address of the texcoord array.
        CFEVect2* poGetUV()
        {
            return( &m_oUV[0] );
        }

        /// Retrieve the number of vertices
        uint uiGetNumVXs()
        {
            return((uint)m_oVX.size());           
        }

        /// Sets the material of the current mesh.
        void SetMaterial(FEHandler _hMaterial);

        /// Sets the material of the current mesh.
        FEHandler hGetMaterial()
        {
            return(m_hMaterial);
        }

        /// Retrieves the half width of the mesh. This function is purely by convenience, since it's useful on rendering.
        FEReal rGetHalfWidth()
        {
            return(m_rHalfWidth);
        }

        /// Perform processing over the object.
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

    public:

        /// The array of bones that influence this mesh.
        CFEArray<CFESkelAnimBone*> m_poBones;

        /// The array of vertices of this mesh.
        CFEArray<CFEVect2> m_oVX;

        /// The array of texture coordinates.
        CFEArray<CFEVect2> m_oUV;

        /// The material attached to this mesh.
        FEHandler m_hMaterial;
        
        /// The half of the width of the mesh
        FEReal  m_rHalfWidth;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
