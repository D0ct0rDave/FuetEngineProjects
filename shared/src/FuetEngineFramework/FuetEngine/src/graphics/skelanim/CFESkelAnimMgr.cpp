// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnim.h"
#include "CFESkelAnimMgr.h"
#include "CFESkelAnimLoader.h"
// ----------------------------------------------------------------------------
CFESkelAnim* CFESkelAnimMgr::poLoadResource(const CFEString& _sFilename)
{
    return( CFESkelAnimLoader::poLoad(_sFilename) );
}
// ----------------------------------------------------------------------------
