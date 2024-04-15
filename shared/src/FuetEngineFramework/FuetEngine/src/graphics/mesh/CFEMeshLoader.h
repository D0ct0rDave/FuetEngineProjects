// ----------------------------------------------------------------------------
/*! \class CFEMeshLoader
 *  \brief A class to load Meshs.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMeshLoaderH
#define CFEMeshLoaderH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFEMesh.h"
// ----------------------------------------------------------------------------
class CFEMeshLoader
{
    public:
            /// Loads an Mesh from a given file
            static CFEMesh* poLoad(const CFEString& _sFilename);

    protected:
            /// Build a basic Mesh around a given material
            static CFEMesh* poBuildBasicMesh(FEHandler _hTex,const CFEString& _sMeshName);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
