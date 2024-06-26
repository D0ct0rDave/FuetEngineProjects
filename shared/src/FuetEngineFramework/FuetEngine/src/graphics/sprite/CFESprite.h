// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESpriteH
#define CFESpriteH
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
class CFESpriteFrame
{
    public:

		/// Should we use the same scale of X for the U tex coord?
		FEBool m_bScaleXUEqually;

		/// Should we use the same scale of Y for the V tex coord?
		FEBool m_bScaleYVEqually;

		/// Should this frame use projected world coordinates instead of UV coords? (U component)
		FEBool m_bUWorldCoords;

		/// Should this frame use projected world coordinates instead of UV coords? (V component)
		FEBool m_bVWorldCoords;
				
        /// Rectangle inside the sprite material defining this sprite frame.
		CFERect  m_oUV;

		/// Size in pixels (of virtual screen dimensions) of the frame.
		CFEVect2 m_oSize;

		/// Time this frame should be rendered without blending.
		FEReal m_rDelay;

		/// The blending time between this and the next frame.
		FEReal m_rBlendTime;

		/// Total time between this and the next frame.
		FEReal m_rFrameTime;

		/// The moment in whole action this frame should be played.
		FEReal m_rStartTime;

		/// Pivot point of the sprite frame.
		// pivots go from 0,0 to 1,1 relative to the frame
        CFEVect2 m_oPivot;

        /// The material used by this sprite frame.
        FEHandler m_hMaterial;

		/// Inverse of the material dimensions
		CFEVect2 m_o1OverDims;
};
// ----------------------------------------------------------------------------
typedef enum ESFSPlayMode {

    SFSPM_NONE,

    SFSPM_ONESHOT,
    SFSPM_PINGPONGSTOP,

    SFSPM_LOOP,
    SFSPM_PINGPONG,

    SFSPM_NUM,
   	SFSPM_MAX = 0xffffffff // to force 32 bit enum

} ESFSPlayMode;

class CFESpriteAction : public CFENamedObject
{
    public:

		CFESpriteAction() : CFENamedObject(""), m_rActionTime(_0r), m_eBlendMode(BM_ALPHA) , m_ePlayMode(SFSPM_NONE), m_rRandStartTime(_0r) {};

		/// Returns the sprite frame number corresponding to the given time, searching from SeekFrame.
		uint uiGetFrame(FEReal _rTime,uint _uiSeekFrame = 0);

		/// Returns the following frame to the given one taking into account the playing mode.
		uint uiNextFrame(uint _uiFrame);
		
		/// Retrieves the action play mode.
 		ESFSPlayMode eGetPlayMode() { return(m_ePlayMode); };

		/// Retrieves the maximum time of the animation without looping or -1 if infinite (when looping)
		FEReal rGetMaxActionTime() { return(m_rActionTime); };

        /// Retrieves the maximum time of the animation taking into account the length of looping cycles.
		FEReal rGetActionTime() { return(m_rActionTime); };
		
		/// Total time of one loop of the animation.
        FEReal					m_rActionTime;

        /// The blending mode to use for this sprite action
        EFEBlendMode            m_eBlendMode;

        /// Play mode
        ESFSPlayMode			m_ePlayMode;

        /// Random start time: range of random time to start the action.
        FEReal					m_rRandStartTime;

        /// Sequence of frames composing the sprite
        CFEArray<CFESpriteFrame> m_oSeq;
};
// ----------------------------------------------------------------------------
class CFESprite : public CFENamedObject
{
    public:
        
		CFESprite() : CFENamedObject(""){};

		/// Retrieves an action by the given index.
		CFESpriteAction* poGetAction(uint _uiActionIdx);

		/// Retrieves an action by its name or NULL if the action is not found.
		CFESpriteAction* poGetAction(const CFEString& _sActionName);

		/// Retrieves an action index by its name or -1 if the action is not found.
		int iGetActionIdx(const CFEString& _sActionName);

        /// CFESpriteSequence list that builds the different animation one sprite may contain
        /// Reference to the sprite material used by this sprite ...
        CFEArray<CFESpriteAction>   m_oActions;
};
// ----------------------------------------------------------------------------
class CFESpriteInst
{
    public:

		CFESpriteInst() : m_uiSpriteAction(0), m_poSprite(NULL), m_rActionTime(_0r), m_uiCurrentActionFrame(0) {};

        /// Current status of the sprite, i.e. animation and other things.

        /// Current sprite action being played.
        uint m_uiSpriteAction;

        /// NOT supposed to place the world position because, this should be handled
        /// by another entity, probably the CGameEntity or the SpriteTile or something like this.
        CFESprite* m_poSprite;

        /// Moment in the sequence related to the current action.
        FEReal m_rActionTime;
	
        /// Current frame in the action.
        uint m_uiCurrentActionFrame;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
