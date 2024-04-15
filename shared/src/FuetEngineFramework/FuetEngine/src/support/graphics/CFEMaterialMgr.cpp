// ----------------------------------------------------------------------------
/*! \class CFEMaterialMgr
 *  \brief Material Manager Class implementation
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMaterialMgr.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
FEHandler* CFEMaterialMgr::poLoadResource(const CFEString& _sFilename)
{
    FEHandler hHandler = CFECore::Graphics::hLoadMaterial(_sFilename);
    return( (FEHandler*) hHandler);
}
// ----------------------------------------------------------------------------
void CFEMaterialMgr::InvalidateResource(FEHandler* _poRes)
{
	CFECore::Graphics::DeleteMaterial((FEHandler)_poRes);
}
// ----------------------------------------------------------------------------
FEBool CFEMaterialMgr::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
	if ((_hMaterial == NULL) && (_sProperty == "DUMP_MATERIAL_LIST"))
	{
		for (uint m=0;m<m_oResDB.uiNumElems();m++)
		{
			CFEString sMat = m_oResDB.sGetVariable(m);
			CFECore::Log::Print("%05d - %s\n",m,sMat.szString());
		}
	}

    return ( CFECore::Graphics::bGetMaterialProperty(_hMaterial,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
FEBool CFEMaterialMgr::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFECore::Graphics::bSetMaterialProperty(_hMaterial,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
void CFEMaterialMgr::Reload()
{
    for (uint i=0;i<m_oResDB.uiNumElems();i++)
    {
		FEHandler hHnd = (FEHandler)m_oResDB.poGetAt(i)->m_poRes;
        CFEString sMaterial = m_oResDB.sGetVariable(i);

        CFECore::Graphics::ReloadMaterial(hHnd,sMaterial);
    }
}
// ----------------------------------------------------------------------------
