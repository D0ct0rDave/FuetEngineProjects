// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESpriteH
#define CFESpriteH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
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
};
// ----------------------------------------------------------------------------
typedef enum ESFSPlayMode {

    SFSPM_NONE,

    SFSPM_ONESHOT,
    SFSPM_PINGPONGSTOP,

    SFSPM_LOOP,
    SFSPM_PINGPONG,

    SFSPM_NUM
};

class CFESpriteAction
{
    public:

		// Returns the sprite frame number corresponding to the given time, searching from SeekFrame.
		uint uiGetFrame(FEReal _rTime,uint _uiSeekFrame = 0);

		// Returns the following frame to the given one taking into account the playing mode.
		uint uiNextFrame(uint _uiFrame);

        /// Total time of one loop of the animation.
        FEReal            m_rActionTime;

        /// Play mode
        ESFSPlayMode    m_ePlayMode;

        /// Random start time: range of random time to start the action.
        FEReal			m_rRandStartTime;

        /// Sequence of frames composing the sprite
        CFEArray<CFESpriteFrame> m_oSeq;

        /// Meaningful action name
        CFEString		m_sName;
};
// ----------------------------------------------------------------------------
class CFESprite
{
    public:
    
		/// Retrieves an action by the given index.
		CFESpriteAction* poGetAction(uint _uiActionIdx);

		/// Retrieves an action by its name or NULL if the action is not found.
		CFESpriteAction* poGetAction(const CFEString& _sActionName);

		/// Retrieves an action index by its name or -1 if the action is not found.
		int iGetActionIdx(const CFEString& _sActionName);
		
        /// The name of this sprite
        CFEString                   m_sName;

        /// The blending mode to use for this sprite
        EFEBlendMode                m_eBlendMode;

        /// CFESpriteSequence list that builds the different animation one sprite may contain
        /// Reference to the sprite material used by this sprite ...
        CFEArray<CFESpriteAction>   m_oActions;
};
// ----------------------------------------------------------------------------
class CFESpriteInst
{
    public:

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
