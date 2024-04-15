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
#include <stdlib.h>
#include "CFEMem.h"
#include "core/CFECore.h"
#include "support/misc/CFEStringUtils.h"
#include "FEBasicTypes.h"

// ----------------------------------------------------------------------------

#if 0 // def N3DS

#endif
// ----------------------------------------------------------------------------
void CFEMem::SetAlignment(EFEMemAlignment _eMemAlignment)
{
}
// ----------------------------------------------------------------------------
EFEMemAlignment CFEMem::eGetAlignment()
{
	return(MA_NONE);
}
// ----------------------------------------------------------------------------
FEPointer CFEMem::pAlloc(uint _uiSize)
{
	return( CFECore::Mem::pAlloc(_uiSize) );
}
// ----------------------------------------------------------------------------
void CFEMem::Free(FEPointer _pPtr)
{
    // special case
    if (_pPtr == NULL) return;
    CFECore::Mem::Free(_pPtr);
}
// ----------------------------------------------------------------------------
FEBool CFEMem::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(CFECore::Mem::bGetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
FEBool CFEMem::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(CFECore::Mem::bSetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
