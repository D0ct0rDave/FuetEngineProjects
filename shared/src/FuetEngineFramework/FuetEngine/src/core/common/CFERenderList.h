// ----------------------------------------------------------------------------
/*! \class CFERenderList
 *  \brief A class to store render a list of polygons groups to render
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFERenderListH
#define CFERenderListH
// ----------------------------------------------------------------------------
#include "support/math/CFEMath.h"
#include "FEBasicTypes.h"
#include "FEConsts.h"
#include "types/CFEVect2.h"
#include "types/CFEMatrix.h"
#include "types/CFEColor.h"
#include "FEEnums.h"
#include "types/CFEArray.h"
// ----------------------------------------------------------------------------
class CFERenderElem
{
	public:

		CFEMatrix				m_oTransf;
		FEReal              	m_rDepth;
		uint					m_uiShaderID;
        FEHandler           	m_hMaterial;
	    EFEBlendMode        	m_eBlendMode;

		CFEVect2*				m_poVXs;
		CFEVect2*				m_poUVs;
		CFEColor*				m_poVCs;
		unsigned short*			m_pusIdx;
		uint					m_uiNumVXs;
};
// ----------------------------------------------------------------------------
/// Class CFERenderList
// ----------------------------------------------------------------------------
typedef CFEArray<CFERenderElem> CFERenderList;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
