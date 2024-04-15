// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#if 0 // ndef MASTER_RELEASE_VERSION
const uint MAX_SAVED_PTRS = 16*1024;
char* gsszPtrs[MAX_SAVED_PTRS];
uint gsuiCurPtr = 0;
uint gsuiAllocNum = 0;
#endif
// ----------------------------------------------------------------------------
/// Allocates a given amount of memory and returns a pointer to the allocated buffer.
// ----------------------------------------------------------------------------

FEPointer CFECore::Mem::pAlloc(uint _uiSize,uint _uiAlignment)
{		
	FEPointer p = (FEPointer)malloc(_uiSize);
	/*
	if ( (((uint)p & 0x0000ffff) == 0x000011C8) && (_uiSize == 36) )
	{
		int a = 0;
	}
		static int iDo = 0;
		if (iDo == 1)
		{
			
			if (_uiSize == 32768)
			{
				int a = 0;
			}
		}
	*/
/*
	if ( (reinterpret_cast<unsigned int>(p) == 0x030B16F0) && (_uiSize == 32))
	{
		int a = 0;
	}
*/
	#if 0 // ndef MASTER_RELEASE_VERSION
	gsuiAllocNum++;

	{
		// code to detect allocation patterns for memory leaks! 
		// Check only a pattern in sizes
		const uint MAX_IDXS = 3;
		static uint uiAllocSizes[] = { 88, 8, 8, 32, 8, 2};
		static uint uiPos = 0;

		if (_uiSize == uiAllocSizes[0])
		{
			if (gsuiCurPtr < MAX_SAVED_PTRS)
			{
				if (gsuiCurPtr == 0)
					memset(gsszPtrs,0,sizeof(gsszPtrs));

				gsszPtrs[gsuiCurPtr] = (char*)p;
				gsuiCurPtr++;
			}
		}

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
	#endif

	#if 0 // ndef MASTER_RELEASE_VERSION	
	{
		const uint MAX_IDXS = 3;

		/*
		static uint uiAllocIdx[] = {1209977,1210021,1210022,1210024,1210025,1210026,1210027,1210028};
		static uint uiAllocSizes[] = {504, 8, 8, 8, 8, 8, 8, 8};
		*/
		static uint uiAllocIdx[] = {263702,263706,263707,1966820,1966821,1966822,1201048,1201051};
		static uint uiAllocSizes[] = { 8, 16, 4, 32, 8, 2};
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
	
	#if 0 // ndef MASTER_RELEASE_VERSION
	for (uint i=0;i<gsuiCurPtr;i++)
	{
		if ((FEPointer)gsszPtrs[i] == _pPtr)
		{
			gsszPtrs[i]= NULL;
			return;
		}
	}
	#endif
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
