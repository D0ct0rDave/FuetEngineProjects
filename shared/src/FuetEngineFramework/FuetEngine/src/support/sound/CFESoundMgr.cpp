// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Resource Manager Class Definition
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundMgr.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
static FEBool gsbBGMResource = false;
// ----------------------------------------------------------------------------
FEHandler* CFESoundMgr::poLoadResource(const CFEString& _sFilename)
{
    FEHandler hHandler = CFECore::Sound::hLoadSound(_sFilename,gsbBGMResource);
    return( (FEHandler*)hHandler);
}
// ----------------------------------------------------------------------------
FEHandler CFESoundMgr::hLoad(const CFEString& _sFilename,FEBool _bBGM)
{
	gsbBGMResource = _bBGM;
	FEHandler hHnd = (FEHandler)poLoad(_sFilename);
	gsbBGMResource = false;

	return(hHnd);
}
// ----------------------------------------------------------------------------
FEHandler CFESoundMgr::hLoad(const CFEString& _sFilename)
{
	return(hLoad(_sFilename,false));
}
// ----------------------------------------------------------------------------
void CFESoundMgr::InvalidateResource(FEHandler* _poRes)
{
	CFECore::Sound::DeleteSound((FEHandler)_poRes);
}
// ----------------------------------------------------------------------------
FEBool CFESoundMgr::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFECore::Sound::bGetSoundProperty(_hSound,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
FEBool CFESoundMgr::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFECore::Sound::bSetSoundProperty(_hSound,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
