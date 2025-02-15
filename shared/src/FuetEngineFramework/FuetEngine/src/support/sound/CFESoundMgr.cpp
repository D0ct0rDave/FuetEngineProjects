// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Resource Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundMgr.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
static bool gsbBGMResource = false;
// ----------------------------------------------------------------------------
FEHandler* CFESoundMgr::poLoadResource(const CFEString& _sFilename)
{
    FEHandler hHandler = CFESystem::Sound::hLoadSound(_sFilename,gsbBGMResource);
    return( (FEHandler*)hHandler);
}
// ----------------------------------------------------------------------------
FEHandler CFESoundMgr::hLoad(const CFEString& _sFilename,bool _bBGM)
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
	CFESystem::Sound::DeleteSound((FEHandler)_poRes);
}
// ----------------------------------------------------------------------------
bool CFESoundMgr::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFESystem::Sound::bGetSoundProperty(_hSound,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
bool CFESoundMgr::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFESystem::Sound::bSetSoundProperty(_hSound,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
