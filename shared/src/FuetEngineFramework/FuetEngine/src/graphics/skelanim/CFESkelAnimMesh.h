// ----------------------------------------------------------------------------
/*! \class CFESkelAnimRenderer
 *  \brief This class represents a mesh object inside a skeletal hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimMeshH
#define CFESkelAnimMeshH
// ----------------------------------------------------------------------------
#include "types/CFEArray.h"
#include "CFESkelAnimBone.h"
// ----------------------------------------------------------------------------
class CFESpriteFrame;
// ----------------------------------------------------------------------------
class CFESkelAnimMesh : public CFESkelAnimNode
{
    public:

        /// Default constructor for the class.
        CFESkelAnimMesh(const CFEString& _sName);

	    /// Destructor of the class
		virtual ~CFESkelAnimMesh();

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

        /// Retrieves the address of the index array.
        unsigned short* pusGetIdxs()
        {
            return( &m_usIdx[0] );
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

        /// Sets the sprite instance to use by the current mesh.
        void SetSprite(FEHandler _hSpriteInst)
		{
			m_hSpriteInst = _hSpriteInst;
		}

        /// Retrieves the sprite used by the current mesh.
        FEHandler hGetSprite()
        {
            return(m_hSpriteInst);
        }

		/// Sets the current action to be played by the sprite.
		void SetAction(uint _uiAction);

		/// Retrieves the current action being played by the sprite.
		uint uiGetAction();


        /// Perform processing over the object.
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

		/// Ultra convenient function to retrieve the last cached sprite frame (for rendering purposes)
		CFESpriteFrame* poGetLastSpriteFrame()
		{
			return(m_poLastSpriteFrame);
		}

		/// Ultra convenient function to cache the last sprite frame (for rendering purposes)
		void SetLastSpriteFrame(CFESpriteFrame* _poLastSpriteFrame)
		{
			m_poLastSpriteFrame = _poLastSpriteFrame;
		}

        /// Retrieves the address of the vertex array.
        CFEVect2* poGetLastSFVX()
        {
            return( &m_oSFVX[0] );
        }

		CFEArray<CFEVect2> m_oIniBonePos;
		CFEArray<FEReal> m_rIniBoneAngle;


	protected:

        /// The array of bones that influence this mesh.
        CFEArray<CFESkelAnimBone*> m_poBones;

        /// The array of vertices of this mesh.
        CFEArray<unsigned short>	m_usIdx;

        /// The array of vertices of this mesh.
        CFEArray<CFEVect2>	m_oVX;

        /// The array of texture coordinates.
        CFEArray<CFEVect2>	m_oUV;

        /// The sprite instance attached to this mesh.
        FEHandler m_hSpriteInst;

        /// The array of vertices of this mesh.
        CFEArray<CFEVect2>	m_oSFVX;

		/// Ultra convenient field to cache last sprite frame used on rendering...
		CFESpriteFrame*		m_poLastSpriteFrame;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
