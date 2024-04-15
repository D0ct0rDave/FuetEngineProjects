// ----------------------------------------------------------------------------
/*! \class CFEFontMgr
 *  \brief Class responsible of font object management
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFontMgr.h"
#include "CFEFontLoader.h"
// ----------------------------------------------------------------------------
CFEFont* CFEFontMgr::poLoadResource(const CFEString& _sFilename)
{
    return ( CFEFontLoader::poLoad(_sFilename) );
}
// ----------------------------------------------------------------------------
void CFEFontMgr::InvalidateResource(CFEFont* _poRes)
{
	delete _poRes;    
}
// ----------------------------------------------------------------------------
