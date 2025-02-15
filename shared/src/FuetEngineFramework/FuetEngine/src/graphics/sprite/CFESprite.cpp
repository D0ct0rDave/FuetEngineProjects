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
#include "CFESprite.h"
#include "system/CFESystem.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// PRE: 
//	0 <= _rTime <= m_rActionTime
//	0 <= _uiSeekFrame < uiMaxFrames
// ----------------------------------------------------------------------------
uint uiSafeGetFrame(CFESpriteAction* _poAction,FEReal _rTime,uint _uiSeekFrame)
{
	uint uiStartFrame; 
	uint uiEndFrame; 

	uint uiMaxFrames = _poAction->m_oSeq.size();

	// Seek frame is occurs after current time
	if (_poAction->m_oSeq[_uiSeekFrame].m_rStartTime > _rTime)
	{
		// Search from the begining
		uiStartFrame = 0;
		uiEndFrame   = _uiSeekFrame-1;
	}
	else
	{
		// Search from
		uiStartFrame = _uiSeekFrame;
		uiEndFrame   = uiMaxFrames-1;
	}

    for (uint i=uiStartFrame;i<uiEndFrame;i++)
    {
        if (_poAction->m_oSeq[i+1].m_rStartTime > _rTime)
            return(i);
    }

    return(uiMaxFrames-1);
}
// ----------------------------------------------------------------------------
uint CFESpriteAction::uiGetFrame(FEReal _rTime,uint _uiSeekFrame)
{
    uint uiMaxFrames = m_oSeq.size();
    CFESystemCheck(_uiSeekFrame<uiMaxFrames,"Seek frame > max action frames");

    switch (m_ePlayMode)
    {
        // ------------------------------------------------
        case SFSPM_NONE:
        case SFSPM_ONESHOT:
        {
            if (_rTime >= m_rActionTime)
            {
                return ( uiMaxFrames-1 );
            }
            else
            {
				return ( uiSafeGetFrame(this,_rTime,_uiSeekFrame) );
            }
        }
        break;

        // ------------------------------------------------
        case SFSPM_LOOP:
        {
			uint uiLoops = _rTime / m_rActionTime;
			FEReal rTime = _rTime - ((FEReal)uiLoops*m_rActionTime);

			return (uiSafeGetFrame(this,rTime,_uiSeekFrame));
        }
        break;

        // ------------------------------------------------
        case SFSPM_PINGPONGSTOP:

			#pragma message(__FUNCTION__"/"__FILE__)
			#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")

        /*
        if (rFrame > 2*uiMaxFrames)
            return;
        */
        /*
        else
            continue; // in the next switch branch.
        */

        case SFSPM_PINGPONG:
        {
			#pragma message(__FUNCTION__"/"__FILE__)
			#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")
        
            /*
            // initial frame.
            uint uiIFrame = (uint)rFrame % (2*uiMaxFrames); // LOOP
            int iEFrame = 0;

            // Blend ractor between frames.
            FEReal rFactor = rFrame - (uint)rFrame;

            if (uiIFrame >= uiMaxFrames)
            {
                // backward direction
                uiIFrame = (2*uiMaxFrames - uiIFrame)-1;

                iEFrame = uiIFrame-1;
                if (iEFrame < 0)
                    iEFrame = 0;
            }
            else
            {
                // initial frame.
                uint uiIFrame = (uint)rFrame % uiMaxFrames;

                // Last frame.
                iEFrame = uiIFrame + 1;
                if (iEFrame >= uiMaxFrames)
                    iEFrame = uiMaxFrames-1;
            }

            if (
                (uiIFrame != iEFrame)
                &&
                ((poSprite->m_eBlendMode == BM_ALPHA) || (poSprite->m_eBlendMode == BM_ALPHAADD))
                )
                RenderSpriteFrames(&poAction->m_oSeq[uiIFrame],&poAction->m_oSeq[iEFrame],rFactor,_oPos,_oSize,_rAngle,_oColor);
            else
                RenderSpriteFrame(&poAction->m_oSeq[uiIFrame],_oPos,_oSize,_rAngle,_oColor,1.0f);
            */
        }
        break;
    };
    
    return(0);
}
// ----------------------------------------------------------------------------
uint CFESpriteAction::uiNextFrame(uint _uiFrame)
{
	uint uiMaxFrames = m_oSeq.size();

    switch (m_ePlayMode)
    {
        // ------------------------------------------------
        case SFSPM_NONE:
        case SFSPM_ONESHOT:
        {
			if (_uiFrame < (uiMaxFrames-1))
				return(_uiFrame+1);
			else
				return(uiMaxFrames-1);
		}			
        break;

        // ------------------------------------------------
        case SFSPM_LOOP:
        {
			return ( (_uiFrame+1) % uiMaxFrames );
        }
        break;

        // ------------------------------------------------
        case SFSPM_PINGPONGSTOP:
			#pragma message(__FUNCTION__"/"__FILE__)
			#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")        
        /*
        if (rFrame > 2*uiMaxFrames)
            return;
        */
        /*
        else
            continue; // in the next switch branch.
        */
        case SFSPM_PINGPONG:
        {
			#pragma message(__FUNCTION__"/"__FILE__)
			#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")        
        }
        break;
    };

    return(0);
}
// ----------------------------------------------------------------------------
int CFESprite::iGetActionIdx(const CFEString& _sActionName)
{
	for (uint i=0;i<this->m_oActions.size();i++)
		if (m_oActions[i].sGetName() |= _sActionName)
			return(i);

	return(-1);
}
// ----------------------------------------------------------------------------
CFESpriteAction* CFESprite::poGetAction(uint _uiActionIdx)
{
	if (_uiActionIdx>=m_oActions.size()) return(NULL);
	return( &m_oActions[_uiActionIdx] );
}
// ----------------------------------------------------------------------------
CFESpriteAction* CFESprite::poGetAction(const CFEString& _sActionName)
{
	int iIdx = iGetActionIdx(_sActionName);
	if (iIdx == -1) return(NULL);

	return( poGetAction((uint)iIdx) );
}
// ----------------------------------------------------------------------------
