// ----------------------------------------------------------------------------
/*! \class CFEMaterialMgr
 *  \brief Material Manager Class implementation
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMaterialMgr.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
FEHandler* CFEMaterialMgr::poLoadResource(const CFEString& _sFilename)
{
    FEHandler hHandler = CFESystem::Graphics::hLoadMaterial(_sFilename);
    return( (FEHandler*) hHandler);
}
// ----------------------------------------------------------------------------
void CFEMaterialMgr::InvalidateResource(FEHandler* _poRes)
{
	CFESystem::Graphics::DeleteMaterial((FEHandler)_poRes);
}
// ----------------------------------------------------------------------------
bool CFEMaterialMgr::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFESystem::Graphics::bGetMaterialProperty(_hMaterial,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
bool CFEMaterialMgr::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    return ( CFESystem::Graphics::bSetMaterialProperty(_hMaterial,_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
void CFEMaterialMgr::Reload()
{
    for (uint i=0;i<CFEResDB::I()->m_oResDB.uiNumElems();i++)
    {
		FEHandler hHnd = (FEHandler)CFEResDB::I()->m_oResDB.poGet(i)->m_poRes;
        CFEString sMaterial = CFEResDB::I()->m_oResDB.sGetVariable(i);

        CFESystem::Graphics::ReloadMaterial(hHnd,sMaterial);
    }
}
// ----------------------------------------------------------------------------
