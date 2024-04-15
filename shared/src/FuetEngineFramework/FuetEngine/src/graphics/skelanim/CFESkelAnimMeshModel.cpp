// ----------------------------------------------------------------------------
/*! \class CFESkelAnimMeshModel
 *  \brief A class that stores features of a mesh inside a skeletal hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimMeshModel.h"
//-----------------------------------------------------------------------------
CFESkelAnimMeshModel::~CFESkelAnimMeshModel()
{
	m_sAttachedBones.clear();
}
//-----------------------------------------------------------------------------
