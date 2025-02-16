// -----------------------------------------------------------------------------
/*! \class CFEResourceQueue
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEResourceQueue.h"

// -----------------------------------------------------------------------------
static void Test()
{
	CFEResourceQueue<uint> lolo;

	lolo.Init(10);
	
	lolo.iAdd((uint*)1);
	lolo.iAdd((uint*)2);
	lolo.iAdd((uint*)3);
	lolo.iAdd((uint*)4);
	
	uint* lala = lolo.poGet(0);
	lolo.Release(lala);
}
// -----------------------------------------------------------------------------