// ----------------------------------------------------------------------------
/*! \class CFEMem
 *  \brief Basic Memory Management Functionalities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMemH
#define CFEMemH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"

namespace CFEMem
{   
    /// Initializes the memory system. Should be called only once per application execution. 
    /// @Param _uiMaxContexts: Is the size of the context stack.
    void Init(uint _uiMaxContexts);

    /// Finalizes the memory system.    
    void Finish();

    /// Switch between the different policies in the system. Static allocation, Dynamic Allocation, Hardware Allocation, ...
    /// Switching from dynamic to static policy implies deallocating ALL the current dynamically allocated data.
    void SwitchPolicy(EFEMemAllocPolicy _eMemPolicy);

    /// Push the current status of the memory system into the context stack. The current base pointer plus the allocated size becomes the new base pointer. 
    /// Allocations will be done relative to the new base pointer.
    void PushContext();

    /// Pops the current status of the memory system from the context stack. Allocations will be done from the previous pointer on.
    void PopContext();
    
    /// Resets the current allocation pointer to the base pointer of the current context.
    void ResetContext();
    
    /// Sets the alignment of the pointers to be returned during further allocations.
    void SetAlignment(EFEMemAlignment _eMemAlignment);
    
    /// Retrieves the currently used alignment in the memory system.
    EFEMemAlignment eGetAlignment();

    /// Retrieves a pointer to a free memory block of the given size.
    FEPointer pAlloc(uint _uiSize);
    
    /// Frees the memory associated to the given pointer. Does nothing with static allocation policy.
    void Free(FEPointer _pPtr);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
