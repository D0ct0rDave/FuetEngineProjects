// ----------------------------------------------------------------------------
/*! \class CFEMem
 *  \brief Basic Memory Management Functionalities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMemH
#define CFEMemH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
namespace CFEMem
{   
    /// Sets the alignment of the pointers to be returned during further allocations.
    void SetAlignment(EFEMemAlignment _eMemAlignment);
    
    /// Retrieves the currently used alignment in the memory system.
    EFEMemAlignment eGetAlignment();

    /// Retrieves a pointer to a free memory block of the given size.
    FEPointer pAlloc(uint _uiSize);
    
    /// Frees the memory associated to the given pointer. Does nothing with static allocation policy.
    void Free(FEPointer _pPtr);
    
    /// Retrieves a memory property if exists.
    FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

    /// Sets a memory property if exists and it's possible.
    FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
