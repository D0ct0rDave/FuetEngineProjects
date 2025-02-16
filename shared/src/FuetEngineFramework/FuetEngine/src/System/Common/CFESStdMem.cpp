// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (MEMORY    == USE_STDMEM)
// ----------------------------------------------------------------------------

#include <stdlib.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"

// ----------------------------------------------------------------------------
/// Allocates a given amount of memory and returns a pointer to the allocated buffer.
// ----------------------------------------------------------------------------
FEPointer CFESystem::Mem::pAlloc(uint _uiSize)
{
    return((FEPointer)malloc(_uiSize));
}
// ----------------------------------------------------------------------------
/// Frees the buffer associated by the given pointer.
// ----------------------------------------------------------------------------
void CFESystem::Mem::Free(FEPointer _pPtr)
{
    free(_pPtr);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
