// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESprite.h"
#include "CFESpriteMgr.h"
#include "CFESpriteLoader.h"
// ----------------------------------------------------------------------------
CFESprite* CFESpriteMgr::poLoadResource(const CFEString& _sFilename)
{
    return( CFESpriteLoader::poLoad(_sFilename) );
}
// ----------------------------------------------------------------------------
void CFESpriteMgr::InvalidateResource(CFESprite* _poRes)
{
	delete _poRes;
}
// ----------------------------------------------------------------------------
