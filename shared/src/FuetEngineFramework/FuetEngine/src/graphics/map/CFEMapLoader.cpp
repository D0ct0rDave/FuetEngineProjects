// ----------------------------------------------------------------------------
/*! \class CFEMapLoader
 *  \brief A class to load a FE map.
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMapLoader.h"
#include "graphics/Sprite/CFESpriteInstMgr.h"
#include "Support/Misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
/// Loads an element
CFEMapElement* CFEMapLoader::poLoadElement(const CFEConfigFile& _oCfg,const CFEString& _sElement,const CFEArray<CFEString>& _oSpriteList)
{
	CFEMapElement* poElem = new CFEMapElement;	
	uint uiSpriteID = _oCfg.iGetInteger(_sElement + ".SpriteIdx",0);

	poElem->m_oPos.x   = _oCfg.rGetReal(_sElement + ".Position.x",_0r);
	poElem->m_oPos.y   = _oCfg.rGetReal(_sElement + ".Position.y",_0r);
	
	poElem->m_oScale.x = _oCfg.rGetReal(_sElement + ".Scale.x",_1r);
	poElem->m_oScale.y = _oCfg.rGetReal(_sElement + ".Scale.y",_1r);

	poElem->m_rDepth   = _oCfg.rGetReal(_sElement + ".Depth",_0r);
	
	poElem->m_oColor.r = _oCfg.rGetReal(_sElement + ".Color.r",_1r);
	poElem->m_oColor.g = _oCfg.rGetReal(_sElement + ".Color.g",_1r);
	poElem->m_oColor.b = _oCfg.rGetReal(_sElement + ".Color.b",_1r);
	poElem->m_oColor.a = _oCfg.rGetReal(_sElement + ".Color.a",_1r);
	
	poElem->m_rAngle   = _oCfg.rGetReal(_sElement + ".Angle",_0r);
	poElem->m_bVisible = _oCfg.bGetBool(_sElement + ".Visible",true);

	poElem->m_hSprInst = CFESpriteInstMgr::hGetInstance(_oSpriteList[uiSpriteID]);
	CFESpriteInstMgr::ManageRender(poElem->m_hSprInst,false);
	CFESpriteInstMgr::Enable(poElem->m_hSprInst);
	CFESpriteInstMgr::SetPos(poElem->m_hSprInst,poElem->m_oPos);
	CFESpriteInstMgr::SetScale(poElem->m_hSprInst,poElem->m_oScale);
	CFESpriteInstMgr::SetAngle(poElem->m_hSprInst,poElem->m_rAngle);

	return(poElem);
}
// ----------------------------------------------------------------------------
CFEMapSector* CFEMapLoader::poLoadSector(const CFEConfigFile& _oCfg,const CFEString& _sSector,const CFEArray<CFEString>& _oSpriteList)
{
    CFEMapSector* poSector = new CFEMapSector;
    
    // Bounding volume
    poSector->m_oBV.m_oIni.x = _oCfg.rGetReal(_sSector+".BV.Mins.x",_0r);
    poSector->m_oBV.m_oIni.y = _oCfg.rGetReal(_sSector+".BV.Mins.y",_0r);
    poSector->m_oBV.m_oEnd.x = _oCfg.rGetReal(_sSector+".BV.Maxs.x",_0r);
    poSector->m_oBV.m_oEnd.y = _oCfg.rGetReal(_sSector+".BV.Maxs.y",_0r);
	poSector->m_bVisible     = _oCfg.bGetBool(_sSector+".Visible",true);

	// Read elements
	uint uiNumElements = _oCfg.iGetInteger(_sSector+".Elements.NumElements",0);

	for (uint i=0;i<uiNumElements;i++)
	{
		CFEMapElement* poElem = poLoadElement(_oCfg,_sSector + CFEString(".Elements.Element") + CFEString(i),_oSpriteList );
		poSector->m_oElements.push_back( *poElem );
	}

	return(poSector);
}
// ----------------------------------------------------------------------------
CFEMapLayer* CFEMapLoader::poLoadLayer(const CFEConfigFile& _oCfg,const CFEString& _sLayer,const CFEArray<CFEString>& _oSpriteList)
{
    CFEMapLayer* poLayer = new CFEMapLayer;

    poLayer->m_sName    = _oCfg.sGetString(_sLayer+".Name","");
    poLayer->m_bVisible = _oCfg.bGetBool(_sLayer+".Visible",true);
    poLayer->m_bVisible = _oCfg.bGetBool(_sLayer+".Visible",true);
    
    poLayer->m_oParallaxFact.x = _oCfg.rGetReal(_sLayer+".Parallax.x",_0r);
    poLayer->m_oParallaxFact.y = _oCfg.rGetReal(_sLayer+".Parallax.y",_0r);
    poLayer->m_rDepth = _oCfg.rGetReal(_sLayer+".Depth",_0r);
    poLayer->m_oSpeed.x = _0r;
    poLayer->m_oSpeed.y = _0r;

    // Number of sectors.
    uint uiNumSectors = _oCfg.iGetInteger(_sLayer+".Sectors.NumSectors",0);

    for (uint j=0;j<uiNumSectors;j++)
    {	
		CFEMapSector* poSector = poLoadSector(_oCfg,_sLayer + ".Sectors.Sector" + CFEString(j),_oSpriteList);
		poLayer->m_poSectors.push_back(poSector);
	}

	return(poLayer);
}
// ----------------------------------------------------------------------------
CFEArray<CFEString>* CFEMapLoader::poLoadSpriteSet(const CFEConfigFile& _oCfg,const CFEString& _sWorkingDir)
{
	// uint uiNumSprites = _oCfg
	CFEArray<CFEString>* poSpriteList = new CFEArray<CFEString>;
	
    // Number of sprites
    uint uiNumSprites = _oCfg.iGetInteger("Map.SpriteSet.NumSprites",0);
	for (uint i=0;i<uiNumSprites;i++)
	{
		CFEString sVar		= CFEString("Map.SpriteSet.Sprite") + CFEString(i);
		CFEString sSprite	= _sWorkingDir + CFEString("/") + _oCfg.sGetString(sVar,"");

		poSpriteList->push_back(sSprite);

		// preload sprite
		CFESpriteInstMgr::Load(sSprite);
	}
	
	return(poSpriteList);
}
// ----------------------------------------------------------------------------
CFEMap* CFEMapLoader::poLoad(const CFEString& _sFilename)
{
    CFEString sFilename = _sFilename + ".map";
    CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

    CFEConfigFile oConfig(sFilename);
    if (! oConfig.bInitialized() ) return(NULL);

    CFEMap* poMap = new CFEMap;

    // Retrieve map name.
    poMap->m_sName = oConfig.sGetString("Map.Name","NonamedMap");

    // Bounding volume
    poMap->m_oBV.m_oIni.x = oConfig.rGetReal("Map.BV.Mins.x",_0r);
    poMap->m_oBV.m_oIni.y = oConfig.rGetReal("Map.BV.Mins.y",_0r);
    poMap->m_oBV.m_oEnd.x = oConfig.rGetReal("Map.BV.Maxs.x",_0r);
    poMap->m_oBV.m_oEnd.y = oConfig.rGetReal("Map.BV.Maxs.y",_0r);

	// Load the sprite set.
	CFEArray<CFEString>* poSpriteSet = poLoadSpriteSet(oConfig,sWorkingDir);

    // Number of layers.
    uint uiNumLayers = oConfig.iGetInteger("Map.Layers.NumLayers",0);

    for (uint j=0;j<uiNumLayers;j++)
    {
		CFEMapLayer* poLayer = poLoadLayer(oConfig,CFEString("Map.Layers.Layer") + CFEString(j),*poSpriteSet);
		poMap->m_poLayers.push_back(poLayer);
	}

    return( poMap );
}
// ----------------------------------------------------------------------------
