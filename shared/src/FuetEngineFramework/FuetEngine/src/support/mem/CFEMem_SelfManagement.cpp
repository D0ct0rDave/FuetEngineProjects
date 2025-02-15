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
#include <stdlib.h>
#include "CFEMem.h"
#include "System/CFESystem.h"
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
inline FEPointer pRoundUp(FEPointer _pPtr,uint _uiAlignment)
{
    uint uiMask = _uiAlignment-1;
    return( FEPointer( ((uint)_pPtr + uiMask) & (uint)(~uiMask) ));
}
// ----------------------------------------------------------------------------
// Override new/delete methods.
// ----------------------------------------------------------------------------
void* operator new(size_t sz)
{
	if (! sz) return(NULL);

	void *pData = CFEMem::pAlloc(sz);

	CFESystem::Check((pData!=NULL),"Unable to allocate data");
	return(pData);	
}
// ----------------------------------------------------------------------------
void operator delete(void* _pPtr)
{
    if (_pPtr == NULL) return;
	CFEMem::Free((FEPointer)_pPtr);
}
// ----------------------------------------------------------------------------
typedef struct TMEMDynNode{

    /// Pointer to the dynamically allocated node.
    FEPointer m_pPtr;

    ///
    TMEMDynNode* m_poNext;

    ///
    TMEMDynNode* m_poPrev;

}TMEMDynNode;

// ----------------------------------------------------------------------------
typedef struct TMEMStaticContext
{
    public:
        FEPointer   m_pBase;
        FEPointer   m_pCurPtr;
}TMEMStaticContext;

typedef struct TMEMGlobals
{
    public: 

            // Current allocation policy.
            EFEMemAllocPolicy   m_ePolicy;
    
            // Current memory alignment.
            uint                m_uiAlignment;

            // ------------------------
            // Static contexts.
            // ------------------------
            TMEMStaticContext*  m_oCtx;

            // Current context.
            TMEMStaticContext*  m_poCurCtx;

            // Maximum number of contexts.
            uint                m_uiMaxCtx;

            // Top of the memory block.
            FEPointer           m_pTop;

            // ------------------------
            /// Dynamic memory node list.
            // ------------------------
            TMEMDynNode*        m_poFirst;
            ///
            TMEMDynNode*        m_poLast;
            
            uint                m_uiNodes;
}TMEMGlobals;

static TMEMGlobals* gpoMEMGlobals = NULL;
// ----------------------------------------------------------------------------
void CFEMem::Init(uint _uiStaticMemorySize,uint _uiMaxContexts)
{
    gpoMEMGlobals = (TMEMGlobals*)CFESystem::Mem::pAlloc(sizeof(TMEMGlobals));

    gpoMEMGlobals->m_uiMaxCtx = _uiMaxContexts;
    gpoMEMGlobals->m_oCtx     = (TMEMStaticContext*)CFESystem::Mem::pAlloc(sizeof(TMEMStaticContext)*gpoMEMGlobals->m_uiMaxCtx);
    gpoMEMGlobals->m_poCurCtx = gpoMEMGlobals->m_oCtx;
    gpoMEMGlobals->m_poCurCtx->m_pBase = CFESystem::Mem::pAlloc(_uiStaticMemorySize);
    gpoMEMGlobals->m_poCurCtx->m_pCurPtr = gpoMEMGlobals->m_poCurCtx->m_pBase;
    gpoMEMGlobals->m_pTop     = gpoMEMGlobals->m_poCurCtx->m_pBase + _uiStaticMemorySize;
    
    gpoMEMGlobals->m_poFirst  = NULL;
    gpoMEMGlobals->m_poLast   = NULL;
    gpoMEMGlobals->m_uiNodes  = 0;

    gpoMEMGlobals->m_ePolicy  = MP_SYSTEM_ALLOCATION;
    gpoMEMGlobals->m_uiAlignment = 1;
}
// ----------------------------------------------------------------------------
void CFEMem::SwitchPolicy(EFEMemAllocPolicy _eMemPolicy)
{   
    /*
    if ( (gpoMEMGlobals->m_ePolicy==MP_DYNAMIC_ALLOCATION) && (_eMemPolicy != gpoMEMGlobals->m_ePolicy))
    {
        // free up everything!
        TMEMDynNode* poNode = gpoMEMGlobals->m_poFirst;
        while (poNode != NULL)
        {
            TMEMDynNode* poNextNode = poNode->m_poNext;

            // free data
            CFESystem::Mem::Free(poNode->m_pPtr);

            // free node
            CFESystem::Mem::Free((FEPointer)poNode);
        
            // Next node
            poNode = poNextNode; 
        }

        gpoMEMGlobals->m_poFirst = NULL;
        gpoMEMGlobals->m_poLast  = NULL;
    }
    */

    gpoMEMGlobals->m_ePolicy = _eMemPolicy;
}
// ----------------------------------------------------------------------------
void CFEMem::PushContext()
{
    uint uiCurCtx = gpoMEMGlobals->m_oCtx - gpoMEMGlobals->m_poCurCtx;
    CFESystem::Check((uiCurCtx+1)<gpoMEMGlobals->m_uiMaxCtx,"Unable to push memory context. Maximum context pushs reached.");
 
    FEPointer pBase = gpoMEMGlobals->m_poCurCtx->m_pCurPtr;

    gpoMEMGlobals->m_poCurCtx++;
    gpoMEMGlobals->m_poCurCtx->m_pBase   = pBase;
    gpoMEMGlobals->m_poCurCtx->m_pCurPtr = pBase;
}
// ----------------------------------------------------------------------------
void CFEMem::PopContext()
{
    CFESystem::Check(gpoMEMGlobals->m_poCurCtx>gpoMEMGlobals->m_oCtx,"Unable to pop memory context. Base context reached.");
    gpoMEMGlobals->m_poCurCtx--;
}
// ----------------------------------------------------------------------------
void CFEMem::ResetContext()
{
    switch (gpoMEMGlobals->m_ePolicy)
    {
        case MP_DYNAMIC_ALLOCATION:
        {
            // free up everything!
            TMEMDynNode* poNode = gpoMEMGlobals->m_poFirst;
            while (poNode != NULL)
            {
                TMEMDynNode* poNextNode = poNode->m_poNext;

                // free data
                CFESystem::Mem::Free(poNode->m_pPtr);

                // free node
                CFESystem::Mem::Free((FEPointer)poNode);
            
                // Next node
                poNode = poNextNode; 
            }

            gpoMEMGlobals->m_poFirst = NULL;
            gpoMEMGlobals->m_poLast  = NULL;        
        }        
        break;

        case MP_STATIC_ALLOCATION:
        gpoMEMGlobals->m_poCurCtx->m_pCurPtr = gpoMEMGlobals->m_poCurCtx->m_pBase;
        break;
    }
}
// ----------------------------------------------------------------------------
void CFEMem::SetAlignment(EFEMemAlignment _eMemAlignment)
{
    gpoMEMGlobals->m_uiAlignment = _eMemAlignment;
}
// ----------------------------------------------------------------------------
EFEMemAlignment CFEMem::eGetAlignment()
{
    return( EFEMemAlignment(gpoMEMGlobals->m_uiAlignment) );
}
// ----------------------------------------------------------------------------
FEPointer CFEMem::pAlloc(uint _uiSize)
{
    if (gpoMEMGlobals == NULL)
    {
        // like MP_SYSTEM_ALLOCATION
        return( CFESystem::Mem::pAlloc(_uiSize) );
    }
    
    switch (gpoMEMGlobals->m_ePolicy)
    {
        // -------------------------------------------------------
        case MP_DYNAMIC_ALLOCATION:
        {
            // Dynamic memory allocation doesn't have anything to do with aligments...
            TMEMDynNode* poNode = (TMEMDynNode*)CFESystem::Mem::pAlloc( sizeof(TMEMDynNode) );

            if (gpoMEMGlobals->m_poFirst == NULL)
            {
                gpoMEMGlobals->m_poFirst = poNode;
                gpoMEMGlobals->m_poLast  = poNode;

                poNode->m_poPrev = NULL;
                poNode->m_poNext = NULL;
            }
            else
            {
                gpoMEMGlobals->m_poLast->m_poNext = poNode;
                poNode->m_poPrev = gpoMEMGlobals->m_poLast;
                poNode->m_poNext = NULL;
                gpoMEMGlobals->m_poLast = poNode;
            }

            gpoMEMGlobals->m_uiNodes++;

            poNode->m_pPtr = CFESystem::Mem::pAlloc(_uiSize);
            return( poNode->m_pPtr );            
        }
        break;
        
        // -------------------------------------------------------
        case MP_STATIC_ALLOCATION:
        {
            // roundup pointer to fullfill alignment.
            FEPointer pIniPtr = pRoundUp(gpoMEMGlobals->m_poCurCtx->m_pCurPtr, gpoMEMGlobals->m_uiAlignment);

            // See if we have enough memory.
            if ((pIniPtr + _uiSize) > gpoMEMGlobals->m_pTop)
            {
                /// unable to fit size into current free memory.
                return(NULL);
            }

            // Update next pointer
            gpoMEMGlobals->m_poCurCtx->m_pCurPtr = pIniPtr + _uiSize;
            return ( pIniPtr );
        }
        break;

        // -------------------------------------------------------
        case MP_SYSTEM_ALLOCATION:
        return( CFESystem::Mem::pAlloc(_uiSize) );
        break;
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
void CFEMem::Free(FEPointer _pPtr)
{
    if (gpoMEMGlobals == NULL)
    {
        // like MP_SYSTEM_ALLOCATION
        CFESystem::Mem::Free(_pPtr);
        return;
    }
    
    // look if this pointer is inside the static blocks
    if ((_pPtr>= gpoMEMGlobals->m_oCtx[0].m_pBase) && (_pPtr<=gpoMEMGlobals->m_pTop))
    {
        // ok nothing to do
        return;
    }
    else
    {
        // look if the pointer is inside the dynamic list
        if (gpoMEMGlobals->m_poLast != NULL)
        {
            TMEMDynNode* poNode = NULL;

            // Special case ?
            if (_pPtr == gpoMEMGlobals->m_poLast->m_pPtr)
                poNode = gpoMEMGlobals->m_poLast;
            else
            {
                poNode = gpoMEMGlobals->m_poFirst;
                while ((poNode != NULL) && (poNode->m_pPtr != _pPtr))
                    poNode = poNode->m_poNext;
            }

            if (poNode == NULL)
            {
                // pointer was previously allocated to the initialization of the memory system???
                CFESystem::Mem::Free(_pPtr);
                return;
                // CFESystem::Check(poNode != NULL,"Trying to free wrong pointer 2.");
            }
            
            // we've found the node to delete
            if (poNode->m_poPrev != NULL)
                poNode->m_poPrev->m_poNext = poNode->m_poNext;

            if (poNode->m_poNext != NULL)
                poNode->m_poNext->m_poPrev = poNode->m_poPrev;

            if (gpoMEMGlobals->m_poLast == poNode)
                gpoMEMGlobals->m_poLast = poNode->m_poPrev;

            if (gpoMEMGlobals->m_poFirst == poNode)
                gpoMEMGlobals->m_poFirst = poNode->m_poNext;

            gpoMEMGlobals->m_uiNodes--;

            // free data
            CFESystem::Mem::Free(poNode->m_pPtr);

            // free node
            CFESystem::Mem::Free((FEPointer)poNode);
        }
        else
        {
            // System ?
            CFESystem::Mem::Free(_pPtr);
        }
    }

    /*
    switch (gpoMEMGlobals->m_ePolicy)
    {
        // -------------------------------------------------------
        case MP_DYNAMIC_ALLOCATION:
        {
            CFESystem::Check(gpoMEMGlobals->m_poLast != NULL,"Trying to free wrong pointer 1.");

            TMEMDynNode* poNode = NULL;

            // Special case ?
            if (_pPtr == gpoMEMGlobals->m_poLast->m_pPtr)
                poNode = gpoMEMGlobals->m_poLast;
            else
            {
                poNode = gpoMEMGlobals->m_poFirst;
                while ((poNode != NULL) && (poNode->m_pPtr != _pPtr))
                    poNode = poNode->m_poNext;

                if (poNode == NULL)
                {
                    // pointer was previously allocated to the initialization of the memory system???
                    CFESystem::Mem::Free(_pPtr);
                    return;
                    // CFESystem::Check(poNode != NULL,"Trying to free wrong pointer 2.");
                }
                else
                {
                    // we've found the node to delete
                    if (poNode->m_poPrev != NULL)
                        poNode->m_poPrev->m_poNext = poNode->m_poNext;

                    if (poNode->m_poNext != NULL)
                        poNode->m_poNext->m_poPrev = poNode->m_poPrev;

                    // free data
                    CFESystem::Mem::Free(poNode->m_pPtr);

                    // free node
                    CFESystem::Mem::Free((FEPointer)poNode);
                    return;
                }
            }
        }
        break;
            
        }
        break;
    }
    */
}
// ----------------------------------------------------------------------------
