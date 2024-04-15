// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMeshH
#define CFEMeshH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "types/CFENamedObject.h"
#include "types/CFEString.h"
#include "types/CFEVect2.h"
#include "types/CFERect.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CFEMaterial;
// ----------------------------------------------------------------------------
class CFEMeshFrame
{
    public:

        /// Rectangle inside the Mesh material defining this Mesh frame.
		CFEArray<CFEVect2> m_oVX;

		/// Time this frame should be rendered without blending.
		FEReal m_rDelay;

		/// The blending time between this and the next frame.
		FEReal m_rBlendTime;

		/// Total time between this and the next frame.
		FEReal m_rFrameTime;

		/// The moment in whole action this frame should be played.
		FEReal m_rStartTime;
};
// ----------------------------------------------------------------------------
typedef enum EMAPlayMode {

	MAPM_NONE,

    MAPM_ONESHOT,
    MAPM_PINGPONGSTOP,

    MAPM_LOOP,
    MAPM_PINGPONG,

    MAPM_NUM,
   	MAPM_MAX = 0xffffffff // to force 32 bit enums

} EMAPlayMode;

class CFEMeshAction : public CFENamedObject
{
    public:

		CFEMeshAction() : CFENamedObject(""), m_rActionTime(_0r), m_ePlayMode(MAPM_NONE), m_rRandStartTime(_0r) {};

		/// Returns the Mesh frame number corresponding to the given time, searching from SeekFrame.
		uint uiGetFrame(FEReal _rTime,uint _uiSeekFrame = 0);

		/// Returns the following frame to the given one taking into account the playing mode.
		uint uiNextFrame(uint _uiFrame);

		/// Retrieves the action play mode.
		EMAPlayMode eGetPlayMode() { return(m_ePlayMode); };

		/// Retrieves the maximum time of the animation without looping or -1 if infinite (when looping)
		FEReal rGetMaxActionTime() { return(m_rActionTime); };

        /// Retrieves the maximum time of the animation taking into account the length of looping cycles.
		FEReal rGetActionTime() { return(m_rActionTime); };
		
		/// Total time of one loop of the animation.
        FEReal					m_rActionTime;

        /// Play mode
        EMAPlayMode				m_ePlayMode;

        /// Random start time: range of random time to start the action.
        FEReal					m_rRandStartTime;

        /// Sequence of frames composing the Mesh
        CFEArray<CFEMeshFrame*> m_oSeq;
};
// ----------------------------------------------------------------------------
class CFEMesh : public CFENamedObject
{
    public:
        
		CFEMesh() : CFENamedObject("") {};

		/// Retrieves an action by the given index.
		CFEMeshAction* poGetAction(uint _uiActionIdx);

		/// Retrieves an action by its name or NULL if the action is not found.
		CFEMeshAction* poGetAction(const CFEString& _sActionName);

		/// Retrieves an action index by its name or -1 if the action is not found.
		int iGetActionIdx(const CFEString& _sActionName);

        /// CFEMeshSequence list that builds the different animation one Mesh may contain
        /// Reference to the Mesh material used by this Mesh ...
        CFEArray<CFEMeshAction*>   m_poActions;

        /// The sprite instance attached to this mesh.
        FEHandler m_hSpriteInst;

        /// UVs for this mesh
		CFEArray<CFEVect2>  m_oUV;

        /// Idxs for UV and VX in this mesh
		CFEArray<unsigned short>		m_usIdx;
};
// ----------------------------------------------------------------------------
class CFEMeshInst
{
    public:

		CFEMeshInst() : m_uiMeshAction(0), m_poMesh(NULL), m_rActionTime(_0r), m_uiCurrentActionFrame(0) {};

        /// Current status of the Mesh, i.e. animation and other things.

        /// Current Mesh action being played.
        uint m_uiMeshAction;

        /// NOT supposed to place the world position because, this should be handled
        /// by another entity, probably the CGameEntity or the MeshTile or something like this.
        CFEMesh* m_poMesh;

        /// Vertices of the instance.
		CFEArray<CFEVect2> m_oVX;

        /// Moment in the sequence related to the current action.
        FEReal m_rActionTime;
	
        /// Current frame in the action.
        uint m_uiCurrentActionFrame;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
