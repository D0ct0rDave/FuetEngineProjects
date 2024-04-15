// -----------------------------------------------------------------------------
/*! \class CActionPlaySound
 *  \brief The base class for script actions.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFESoundAction.h"
// -----------------------------------------------------------------------------
void CFESoundActionQueue::Clear()
{
	m_oQueue.clear();
}
// -----------------------------------------------------------------------------
FEHandler CFESoundActionQueue::hGetSound(FEHandler _hSoundResource)
{
	for (uint i=0;i<m_oQueue.size();i++)
	{
		if (m_oQueue[i].m_hSoundResource == _hSoundResource)
		{
			return(m_oQueue[i].m_hSound);
		}
	}

	return(NULL);
}
// -----------------------------------------------------------------------------
void CFESoundActionQueue::Queue(FEHandler _hSoundResource,FEHandler _hSound)
{
	CSoundActionEntry oSAE;
	oSAE.m_hSound			= _hSound;
	oSAE.m_hSoundResource	= _hSoundResource;

	m_oQueue.push_back(oSAE);
}
// -----------------------------------------------------------------------------		
void CFESoundActionQueue::Dequeue(FEHandler _hSoundResource)
{
	for (uint i=0;i<m_oQueue.size();i++)
	{
		if (m_oQueue[i].m_hSoundResource == _hSoundResource)
		{
			m_oQueue.Delete(i);
			return;
		}
	}	
}
// -----------------------------------------------------------------------------
