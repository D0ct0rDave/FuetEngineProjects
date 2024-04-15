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
#include "CFEMesh.h"
#include "core/CFECore.h"
#include "FEConsts.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// PRE: 
//	0 <= _rTime <= m_rActionTime
//	0 <= _uiSeekFrame < uiMaxFrames
// ----------------------------------------------------------------------------
uint uiSafeGetFrame(CFEMeshAction* _poAction,FEReal _rTime,uint _uiSeekFrame)
{
	uint uiStartFrame; 
	uint uiEndFrame; 

	uint uiMaxFrames = _poAction->m_oSeq.size();

	// Seek frame is occurs after current time
	if (_poAction->m_oSeq[_uiSeekFrame]->m_rStartTime > _rTime)
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
        if (_poAction->m_oSeq[i+1]->m_rStartTime > _rTime)
            return(i);
    }

    return(uiMaxFrames-1);
}
// ----------------------------------------------------------------------------
uint CFEMeshAction::uiGetFrame(FEReal _rTime,uint _uiSeekFrame)
{
    uint uiMaxFrames = m_oSeq.size();
    CFECoreCheck(_uiSeekFrame<uiMaxFrames,"Seek frame > max action frames");

    switch (m_ePlayMode)
    {
        // ------------------------------------------------
        case MAPM_NONE:
        case MAPM_ONESHOT:
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
        // break;	// avoid "statement is un reachable" warning

        // ------------------------------------------------
        case MAPM_LOOP:
        {
			uint uiLoops = _rTime / m_rActionTime;
			FEReal rTime = _rTime - ((FEReal)uiLoops*m_rActionTime);

			return (uiSafeGetFrame(this,rTime,_uiSeekFrame));
        }
        // break;	// avoid "statement is un reachable" warning

        // ------------------------------------------------
        case MAPM_PINGPONGSTOP:

			// #pragma message(__FUNCTION__"/"__FILE__)
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

        case MAPM_PINGPONG:
        {
        	// #pragma message(__FUNCTION__"/"__FILE__)
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
                ((poMesh->m_eBlendMode == BM_ALPHA) || (poMesh->m_eBlendMode == BM_ALPHAADD))
                )
                RenderMeshFrames(&poAction->m_oSeq[uiIFrame],&poAction->m_oSeq[iEFrame],rFactor,_oPos,_oSize,_rAngle,_oColor);
            else
                RenderMeshFrame(&poAction->m_oSeq[uiIFrame],_oPos,_oSize,_rAngle,_oColor,1.0f);
            */
        }
        break;
    };
    
    return(0);
}
// ----------------------------------------------------------------------------
uint CFEMeshAction::uiNextFrame(uint _uiFrame)
{
	uint uiMaxFrames = m_oSeq.size();

    switch (m_ePlayMode)
    {
        // ------------------------------------------------
        case MAPM_NONE:
        case MAPM_ONESHOT:
        {
			if (_uiFrame < (uiMaxFrames-1))
				return(_uiFrame+1);
			else
				return(uiMaxFrames-1);
		}			
        // break;	// avoid "statement is un reachable" warning

        // ------------------------------------------------
        case MAPM_LOOP:
        {
			return ( (_uiFrame+1) % uiMaxFrames );
        }
        // break;	// avoid "statement is un reachable" warning

        // ------------------------------------------------
        case MAPM_PINGPONGSTOP:
        	// #pragma message(__FUNCTION__"/"__FILE__)
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
        case MAPM_PINGPONG:
        {
        	// #pragma message(__FUNCTION__"/"__FILE__)
			#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")        
        }
        break;
    };

    return(0);
}
// ----------------------------------------------------------------------------
int CFEMesh::iGetActionIdx(const CFEString& _sActionName)
{
	for (uint i=0;i<m_poActions.size();i++)
		if (m_poActions[i]->sGetName() |= _sActionName)
			return(i);

	return(-1);
}
// ----------------------------------------------------------------------------
CFEMeshAction* CFEMesh::poGetAction(uint _uiActionIdx)
{
	if (_uiActionIdx>=m_poActions.size()) return(NULL);
	return( m_poActions[_uiActionIdx] );
}
// ----------------------------------------------------------------------------
CFEMeshAction* CFEMesh::poGetAction(const CFEString& _sActionName)
{
	int iIdx = iGetActionIdx(_sActionName);
	if (iIdx == -1) return(NULL);

	return( poGetAction((uint)iIdx) );
}
// ----------------------------------------------------------------------------
