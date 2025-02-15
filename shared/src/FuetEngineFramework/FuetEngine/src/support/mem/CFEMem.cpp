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
#include "support/misc/CFEStringUtils.h"
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
typedef struct TMEMNode{

    /// Pointer to the dynamically allocated node.
    FEPointer       m_pPtr;
    
    /// Size allocated in the ptr member of this node.
    uint            m_uiSize;

    /// Next node in the list chain.
    TMEMNode*       m_poNext;

}TMEMNode;
// ----------------------------------------------------------------------------
typedef struct TMEMNodeList
{
    TMEMNode*        m_poFirst;
    uint             m_uiNodes;
}TMEMNodeList;
// ----------------------------------------------------------------------------
const uint MEM_CTXHT_SIZE = 512;
typedef struct TMEMContext
{
    public:

        // Hash table to speed up frees.
        TMEMNodeList       m_poHT[MEM_CTXHT_SIZE];

}TMEMContext;
// ----------------------------------------------------------------------------
typedef struct TMEMGlobals
{
    public: 

        // Current allocation policy.
        EFEMemAllocPolicy   m_ePolicy;

        // Current memory alignment.
        uint                m_uiAlignment;

        // ------------------------
        // Memory contexts.
        // ------------------------
        TMEMContext*        m_oCtx;

        // Current context.
        TMEMContext*        m_poCurCtx;

        // Maximum number of contexts.
        uint                m_uiMaxCtx;

}TMEMGlobals;

static TMEMGlobals* gpoMEMGlobals = NULL;
// ----------------------------------------------------------------------------
// Override new/delete methods.
// ----------------------------------------------------------------------------
void* operator new(size_t sz)
{
	if (! sz) return(NULL);

	void *pData = CFEMem::pAlloc(sz);

	/*
		// forbidden to use CFEString here: CFEString uses new, so an stack overflow will happen.
		if (gpoMEMGlobals != NULL)
			CFESystemCheck((pData!=NULL),"Unable to allocate data");
	*/

	return(pData);	
}
// ----------------------------------------------------------------------------
void operator delete(void* _pPtr)
{
    if (_pPtr == NULL) return;
	CFEMem::Free((FEPointer)_pPtr);
}
// ----------------------------------------------------------------------------
inline FEPointer pRoundUp(FEPointer _pPtr,uint _uiAlignment)
{
    uint uiMask = _uiAlignment-1;
    return( FEPointer( ((uint)_pPtr + uiMask) & (uint)(~uiMask) ));
}
// ----------------------------------------------------------------------------
void CFEMem::Init(uint _uiMaxContexts)
{
    gpoMEMGlobals = (TMEMGlobals*)CFESystem::Mem::pAlloc(sizeof(TMEMGlobals));

    gpoMEMGlobals->m_uiMaxCtx = _uiMaxContexts;
    gpoMEMGlobals->m_oCtx     = (TMEMContext*)CFESystem::Mem::pAlloc(sizeof(TMEMContext)*gpoMEMGlobals->m_uiMaxCtx);
    gpoMEMGlobals->m_poCurCtx = gpoMEMGlobals->m_oCtx;

    memset(gpoMEMGlobals->m_oCtx,0,sizeof(TMEMContext)*gpoMEMGlobals->m_uiMaxCtx);

    gpoMEMGlobals->m_ePolicy  = MP_SYSTEM_ALLOCATION;
    gpoMEMGlobals->m_uiAlignment = 1;
}
// ----------------------------------------------------------------------------
void CFEMem::Finish()
{
	if (gpoMEMGlobals==NULL) return;

    bool bExit = false;
    while (! bExit)
    {
        ResetContext();

        if (gpoMEMGlobals->m_poCurCtx != gpoMEMGlobals->m_oCtx)
            gpoMEMGlobals->m_poCurCtx--;
        else
            bExit = true;
    };

    // free contexts
    CFESystem::Mem::Free((FEPointer)gpoMEMGlobals->m_oCtx);

    // 
    CFESystem::Mem::Free((FEPointer)gpoMEMGlobals);
    gpoMEMGlobals = NULL;
}
// ----------------------------------------------------------------------------
void CFEMem::SwitchPolicy(EFEMemAllocPolicy _eMemPolicy)
{   
	if (gpoMEMGlobals == NULL) return;
	gpoMEMGlobals->m_ePolicy = _eMemPolicy;
}
// ----------------------------------------------------------------------------
EFEMemAllocPolicy CFEMem::eGetMemPolicy()
{
	if (gpoMEMGlobals == NULL) return(MP_SYSTEM_ALLOCATION);
	return(gpoMEMGlobals->m_ePolicy);
}
// ----------------------------------------------------------------------------
void CFEMem::PushContext()
{
	if (gpoMEMGlobals == NULL) return;

    CFESystemCheck(((gpoMEMGlobals->m_poCurCtx - gpoMEMGlobals->m_oCtx)+1)<gpoMEMGlobals->m_uiMaxCtx,"Unable to push memory context. Maximum context pushs reached.");
 
    gpoMEMGlobals->m_poCurCtx++;

    // Initialize everything in this context.
    memset(gpoMEMGlobals->m_poCurCtx,0,sizeof(TMEMContext));
}
// ----------------------------------------------------------------------------
void CFEMem::PopContext()
{
	if (gpoMEMGlobals == NULL) return;

    CFESystemCheck(gpoMEMGlobals->m_poCurCtx>=gpoMEMGlobals->m_oCtx,"Unable to pop memory context. Base context reached.");
    
    // Free all the allocated pointers in the current context.
    ResetContext();
    
    gpoMEMGlobals->m_poCurCtx--;
}
// ----------------------------------------------------------------------------
void CFEMem::ResetContext()
{
	if (gpoMEMGlobals == NULL) return;

    // free up everything in this context.
    for (uint i=0;i<MEM_CTXHT_SIZE;i++)
    {
        TMEMNodeList* poList = &gpoMEMGlobals->m_poCurCtx->m_poHT[i];
        TMEMNode* poNode = poList->m_poFirst;

        while (poNode != NULL)
        {
            TMEMNode* poNextNode = poNode->m_poNext;

            // free data
            CFESystem::Mem::Free(poNode->m_pPtr);

            // free node
            CFESystem::Mem::Free((FEPointer)poNode);

            // Next node
            poNode = poNextNode;
            
            #ifdef _DEBUG
            poList->m_uiNodes--;
            #endif
        }

        poList->m_poFirst = NULL;
        poList->m_uiNodes = 0;
    }
}
// ----------------------------------------------------------------------------
void CFEMem::SetAlignment(EFEMemAlignment _eMemAlignment)
{
	if (gpoMEMGlobals == NULL) return;
    gpoMEMGlobals->m_uiAlignment = _eMemAlignment;
}
// ----------------------------------------------------------------------------
EFEMemAlignment CFEMem::eGetAlignment()
{
	if (gpoMEMGlobals == NULL) return(MA_NONE);
    return( EFEMemAlignment(gpoMEMGlobals->m_uiAlignment) );
}
// ----------------------------------------------------------------------------
FEPointer CFEMem::pAlloc(uint _uiSize)
{
	if ((gpoMEMGlobals == NULL) || (gpoMEMGlobals->m_ePolicy == MP_SYSTEM_ALLOCATION))
    {
        return( CFESystem::Mem::pAlloc(_uiSize) );
    }

    // Dynamic memory allocation doesn't have anything to do with aligments...
     
    TMEMNode* poNode = (TMEMNode*)CFESystem::Mem::pAlloc( sizeof(TMEMNode) );
    poNode->m_pPtr   = CFESystem::Mem::pAlloc(_uiSize);
    poNode->m_uiSize = _uiSize;
    
    // Retrieve the hash index of this pointer
    uint uiHashIdx = (CFEStringUtils::uiGetCRC32((char*)&poNode->m_pPtr,4) % MEM_CTXHT_SIZE);

    poNode->m_poNext = gpoMEMGlobals->m_poCurCtx->m_poHT[uiHashIdx].m_poFirst;
    
    // Insert the node in the list of this hash entry.
    gpoMEMGlobals->m_poCurCtx->m_poHT[uiHashIdx].m_poFirst = poNode;
    gpoMEMGlobals->m_poCurCtx->m_poHT[uiHashIdx].m_uiNodes++;
       
    // return pointer
    return( poNode->m_pPtr );
}
// ----------------------------------------------------------------------------
void CFEMem::Free(FEPointer _pPtr)
{
    // special case
    if (_pPtr == NULL) return;

    if (gpoMEMGlobals == NULL)
    {
        // like MP_SYSTEM_ALLOCATION
        CFESystem::Mem::Free(_pPtr);
        return;
    }

    // Retrieve the has index from the pointer value.
    uint uiHashIdx = (CFEStringUtils::uiGetCRC32((char*)&_pPtr,4) % MEM_CTXHT_SIZE);

    // Start looking from the current context.
    bool bExit = false;
    TMEMContext* poCtx = gpoMEMGlobals->m_poCurCtx;

    do {

        // Retrieve the list of nodes of this hash table entry.
        TMEMNodeList* poList = &poCtx->m_poHT[uiHashIdx];

        // Look if the pointer is inside the dynamic list of the hash table entry.
        TMEMNode* poPrevNode = NULL;
        TMEMNode* poNode     = poList->m_poFirst;

        while ((poNode != NULL) && (poNode->m_pPtr != _pPtr))
        {
            poPrevNode = poNode;
            poNode = poNode->m_poNext;
        }

        if (poNode != NULL)
        {
            // we've found the node to delete
            if (poList->m_poFirst == poNode)
                poList->m_poFirst = poNode->m_poNext;
            else
                poPrevNode->m_poNext = poNode->m_poNext;

            poList->m_uiNodes--;

            // free data
            CFESystem::Mem::Free(poNode->m_pPtr);

            // free node
            CFESystem::Mem::Free((FEPointer)poNode);

            // we can safely return
            return;
        }
        else
        {
            if (poCtx != gpoMEMGlobals->m_oCtx)
                poCtx--;
            else
            {
                // we've looked into all the context with no luck.
                bExit = true;
            }
        }
    }
    while (! bExit);

    // Data has been allocated by system allocations???
    CFESystem::Mem::Free(_pPtr);
}
// ----------------------------------------------------------------------------
bool CFEMem::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(CFESystem::Mem::bGetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
bool CFEMem::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(CFESystem::Mem::bSetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
