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

#include "../CFECore.h"
#include "FEBasicTypes.h"

// ----------------------------------------------------------------------------
// Override new/delete methods.
// ----------------------------------------------------------------------------
void* operator new (size_t sz)
{
	if (! sz) return(NULL);
	void *pData = CFECore::Mem::pAlloc(sz);
	return(pData);	
}

void operator delete(void* _pPtr)
{
    if (_pPtr == NULL) return;
	CFECore::Mem::Free((FEPointer)_pPtr);
}

void* operator new[] (size_t sz)
{
	if(! sz) return(NULL);
	void *pData = CFECore::Mem::pAlloc(sz);
	return(pData);
}

void operator delete[] (void* p)
{
    operator delete(p);
}

// ----------------------------------------------------------------------------
/// Allocates a given amount of memory and returns a pointer to the allocated buffer.
// ----------------------------------------------------------------------------
FEPointer CFECore::Mem::pAlloc(uint _uiSize,uint _uiAlignment)
{	
	FEPointer p = (FEPointer)malloc(_uiSize);
/*
	if ( (reinterpret_cast<unsigned int>(p) == 0x030B16F0) && (_uiSize == 32))
	{
		int a = 0;
	}
*/
	#if 0 // ndef MASTER_RELEASE_VERSION

		// code to detect allocation patterns for memory leaks!
		const uint MAX_IDXS = 4;
		
		/*
		static uint uiAllocIdx[] = {1209977,1210021,1210022,1210024,1210025,1210026,1210027,1210028};
		static uint uiAllocSizes[] = {504, 8, 8, 8, 8, 8, 8, 8};
		*/
		
		static uint uiAllocIdx[] = {1966817,1966818,1966819,1966820,1966821,1966822,1201048,1201051};
		static uint uiAllocSizes[] = { 4, 16, 2, 4, 16, 2};
		
		static uint uiBaseAllocIdx = 0;
		static uint uiPos = 0;
		static uint uiStep = 0;

		if (uiPos == 0)
		{
			if (_uiSize == uiAllocSizes[0])
			{
				uiBaseAllocIdx = uiAllocIdx[0];
				uiPos++;
				uiStep = 0;
			}
		}
		else
		{
			if ((uiBaseAllocIdx+uiStep) == uiAllocIdx[uiPos])
			{
				if (_uiSize == uiAllocSizes[uiPos])
				{
					uiPos++;
					if (uiPos == MAX_IDXS)
					{
						int a = 0;
					}
				}
				else
				{
					uiPos = 0;
				}
			}
		}

		uiStep++;

		if (_uiSize == 76)
		{
			int a = 0;
		}
	#endif

    return(p);
}
// ----------------------------------------------------------------------------
/// Frees the buffer associated by the given pointer.
// ----------------------------------------------------------------------------
void CFECore::Mem::Free(FEPointer _pPtr)
{
    free(_pPtr);
}
// ----------------------------------------------------------------------------
/// Retrieves a memory property if exists.
// ----------------------------------------------------------------------------
FEBool CFECore::Mem::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	if (_sProperty == "FreeMemory")
    {
        *(uint*)_pParam = 0;
        return (true);
    }

else if (_sProperty == "TotalMemory")
    {
        *(uint*)_pParam = 0;
        return (true);
    }

	return(false);
}
// ----------------------------------------------------------------------------
/// Sets a memory property if exists and it's possible.
// ----------------------------------------------------------------------------
FEBool CFECore::Mem::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
