// -----------------------------------------------------------------------------
/*! \class CFESoundAction
 *  \brief An action to play sounds
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFESoundActionH
#define CFESoundActionH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "types/CFESingleton.h"
#include "types/CFEArray.h"
// -----------------------------------------------------------------------------
DECLARE_SINGLETON(CFESoundActionQueue)
{
	public:

		///
		void Clear();

		// 
		FEHandler hGetSound(FEHandler _hSoundResource);

		// 
		void Queue(FEHandler _hSoundResource,FEHandler _hSound);

		// 
		void Dequeue(FEHandler _hSoundResource);

	protected:

		// 
		class CSoundActionEntry
		{
			public:
				
				FEHandler m_hSoundResource;
				FEHandler m_hSound;
		};

		CFEArray<CSoundActionEntry> m_oQueue;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
