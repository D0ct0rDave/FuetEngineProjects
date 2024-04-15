// -----------------------------------------------------------------------------
/*! \class CFEResourceQueue
 *  \brief A class to manage all the entities of the system.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEResourceQueue.h"

// -----------------------------------------------------------------------------
#ifdef _DEBUG
static void Test()
{
	CFEResourceQueue<uint> lolo;

	lolo.iAdd((uint*)1);
	lolo.iAdd((uint*)2);
	lolo.iAdd((uint*)3);
	lolo.iAdd((uint*)4);
	
	if (lolo.bIsFree(1))
		lolo.Release(1);

	uint* lala = lolo.poGetAt(0);
	lolo.Release(lala);
	
	lolo.bIsFree(2);
}
#endif
// -----------------------------------------------------------------------------
