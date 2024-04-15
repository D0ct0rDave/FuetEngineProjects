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
#include "CFEMesh.h"
#include "CFEMeshMgr.h"
#include "CFEMeshLoader.h"
// ----------------------------------------------------------------------------
CFEMesh* CFEMeshMgr::poLoadResource(const CFEString& _sFilename)
{
    return( CFEMeshLoader::poLoad(_sFilename) );
}
// ----------------------------------------------------------------------------
void CFEMeshMgr::InvalidateResource(CFEMesh* _poRes)
{
	delete _poRes;
}
// ----------------------------------------------------------------------------
