// ----------------------------------------------------------------------------
/*! \class CFESkelAnimMeshModel
 *  \brief A class that stores features of a mesh inside a skeletal hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimMeshModelH
#define CFESkelAnimMeshModelH
//-----------------------------------------------------------------------------
#include "CFESkelAnimNode.h"
//-----------------------------------------------------------------------------
class CFESkelAnimVisitor;
//-----------------------------------------------------------------------------
class CFESkelAnimMeshModel : public CFESkelAnimNode
{
    public:		

			CFESkelAnimMeshModel(const CFEString& _sName) : CFESkelAnimNode(_sName), m_sMaterial("")
			{
			}

			/// Attach a bone to the mesh model
			void AttachBone(const CFEString& _sBoneName)
			{
			    m_sAttachedBones.push_back(_sBoneName);
			}

			/// Retrieves the name of an attached bone identified by the given index.
			const CFEString& sGetAttachedBone(uint _uiBoneIdx)
			{
			    return( m_sAttachedBones[_uiBoneIdx] );
			}

			/// Retrieves then number of attached bones to this mesh model.
			uint uiGetNumAttachedBones()
			{
			    return( m_sAttachedBones.size() );
			}

			/// Sets the material used by this mesh model.
			void SetMaterial(const CFEString& _sMaterial)
			{
				m_sMaterial = _sMaterial;
			}

			/// Retrieves the material used by this mesh model.
			const CFEString& sGetMaterial()
			{
				return( m_sMaterial );
			}

            /// Perform processing over the object
    		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
    		{
    		    _poVisitor->Visit(this);
    		}

	private:
				CFEString           m_sMaterial;
				CFEArray<CFEString> m_sAttachedBones;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
