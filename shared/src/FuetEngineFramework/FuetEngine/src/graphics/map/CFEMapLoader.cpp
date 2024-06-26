// ----------------------------------------------------------------------------
/*! \class CFEMapLoader
 *  \brief A class to load a FE map.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMapLoader.h"
#include "graphics/Sprite/CFESpriteInstMgr.h"
#include "Support/Misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
/// Loads an element
CFEMapElement CFEMapLoader::oLoadElement(const CFEConfigFile& _oCfg,const CFEString& _sElement,const CFEArray<CFEString>& _oSpriteList)
{
	CFEMapElement oElem;	
	uint uiSpriteID = _oCfg.iGetInteger(_sElement + ".SpriteIdx",0);

	oElem.m_oPos.x   = _oCfg.rGetReal(_sElement + ".Position.x",_0r);
	oElem.m_oPos.y   = _oCfg.rGetReal(_sElement + ".Position.y",_0r);
	
	oElem.m_oScale.x = _oCfg.rGetReal(_sElement + ".Scale.x",_1r);
	oElem.m_oScale.y = _oCfg.rGetReal(_sElement + ".Scale.y",_1r);

	oElem.m_rDepth   = _oCfg.rGetReal(_sElement + ".Depth",_0r);
	
	oElem.m_oColor.r = _oCfg.rGetReal(_sElement + ".Color.r",_1r);
	oElem.m_oColor.g = _oCfg.rGetReal(_sElement + ".Color.g",_1r);
	oElem.m_oColor.b = _oCfg.rGetReal(_sElement + ".Color.b",_1r);
	oElem.m_oColor.a = _oCfg.rGetReal(_sElement + ".Color.a",_1r);
	
	oElem.m_rAngle   = _oCfg.rGetReal(_sElement + ".Angle",_0r);
	oElem.m_bVisible = _oCfg.bGetBool(_sElement + ".Visible",true);

	oElem.m_hSprInst = CFESpriteInstMgr::I()->hGetInstance(_oSpriteList[uiSpriteID]);
	CFESpriteInstMgr::I()->ManageRender(oElem.m_hSprInst,false);
	CFESpriteInstMgr::I()->Enable(oElem.m_hSprInst);
	CFESpriteInstMgr::I()->SetPos(oElem.m_hSprInst,oElem.m_oPos);
	CFESpriteInstMgr::I()->SetScale(oElem.m_hSprInst,oElem.m_oScale);
	CFESpriteInstMgr::I()->SetAngle(oElem.m_hSprInst,oElem.m_rAngle);

	return(oElem);
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
		poSector->m_oElements.push_back( oLoadElement(_oCfg,_sSector + CFEString(".Elements.Element") + CFEString((int)i),_oSpriteList ) );

	return(poSector);
}
// ----------------------------------------------------------------------------
CFEMapLayer* CFEMapLoader::poLoadLayer(const CFEConfigFile& _oCfg,const CFEString& _sLayer,const CFEArray<CFEString>& _oSpriteList)
{
    CFEMapLayer* poLayer = new CFEMapLayer;

    poLayer->SetName( _oCfg.sGetString(_sLayer+".Name","") );
    poLayer->m_bVisible = _oCfg.bGetBool(_sLayer+".Visible",true);
    
    poLayer->m_oParallaxFact.x = _oCfg.rGetReal(_sLayer+".Parallax.x",_0r);
    poLayer->m_oParallaxFact.y = _oCfg.rGetReal(_sLayer+".Parallax.y",_0r);
    poLayer->m_rDepth = _oCfg.rGetReal(_sLayer+".Depth",_0r);
    poLayer->m_oSpeed.x = _0r;
    poLayer->m_oSpeed.y = _0r;

	// loop properties
	poLayer->m_uiLoopFlags	= _oCfg.iGetInteger(_sLayer+".LoopFlags",0);
	poLayer->m_rLoopStartX	= _oCfg.rGetReal(_sLayer+".LoopStartX",_0r);
	poLayer->m_rLoopEndX	= _oCfg.rGetReal(_sLayer+".LoopEndX",_0r);
	poLayer->m_rLoopStartY	= _oCfg.rGetReal(_sLayer+".LoopStartY",_0r);
	poLayer->m_rLoopEndY	= _oCfg.rGetReal(_sLayer+".LoopEndY",_0r);

    // Number of sectors.
    uint uiNumSectors = _oCfg.iGetInteger(_sLayer+".Sectors.NumSectors",0);

    for (uint j=0;j<uiNumSectors;j++)
    {	
		CFEMapSector* poSector = poLoadSector(_oCfg,_sLayer + ".Sectors.Sector" + CFEString((int)j),_oSpriteList);
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
		CFEString sVar		= CFEString("Map.SpriteSet.Sprite") + CFEString((int)i);
		CFEString sSprite	= _sWorkingDir + CFEString("/") + _oCfg.sGetString(sVar,"");

		poSpriteList->push_back(sSprite);

		// preload sprite
		CFESpriteInstMgr::I()->Preload(sSprite);
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
    poMap->SetName( oConfig.sGetString("Map.Name","NonamedMap") );

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
		CFEMapLayer* poLayer = poLoadLayer(oConfig,CFEString("Map.Layers.Layer") + CFEString((int)j),*poSpriteSet);
		poMap->m_poLayers.push_back(poLayer);
	}
	
	// Delete preloaded sprite set.
	poSpriteSet->clear();
	delete poSpriteSet;

	/*
	// Compute Map Bounding Volume
	poMap->m_oBV.m_oEnd.x = -1e6;
	poMap->m_oBV.m_oEnd.y = -1e6;
	poMap->m_oBV.m_oIni.x =  1e6;
	poMap->m_oBV.m_oIni.y =  1e6;

    for (uint uiSector=0;uiSector<uiNumSectors;uiSector++)
    {
        CFEMapSector* poSector = poMap->m_poSectors[uiSector];
        if (poSector == NULL) continue;
    
       	poSector->m_oBV.m_oEnd.x = -1e6;
	    poSector->m_oBV.m_oEnd.y = -1e6;
	    poSector->m_oBV.m_oIni.x =  1e6;
	    poSector->m_oBV.m_oIni.y =  1e6;
	
	    for (uint uiLayer=0;uiLayer<poSector->m_poLayers.size();uiLayer++)
        {
            CFEMapLayer* poLayer = poSector->m_poLayers[uiLayer];
            if (poLayer == NULL) continue;

		    FEBool bXIsValid = (poLayer->m_oSpeed.x == _1r);
		    FEBool bYIsValid = (poLayer->m_oSpeed.y == _1r);
    		
		    if ( bXIsValid || bYIsValid)
		    {
			    for (uint uiElem=0;uiElem<poLayer->m_oElements.size();uiElem++)
			    {
				    CFEMapElement* poElem = &poLayer->m_oElements[uiElem];

				    CFEVect2 oVX[4];
				    CFESpriteInstMgr::GetGeometry(poElem->m_hSprInst,oVX);

				    for (uint i=0;i<4;i++)
				    {
					    if (bXIsValid)
					    {
						    if (oVX[i].x < poSector->m_oBV.m_oIni.x) poSector->m_oBV.m_oIni.x = oVX[i].x;
				       else if (oVX[i].x > poSector->m_oBV.m_oEnd.x) poSector->m_oBV.m_oEnd.x = oVX[i].x;
					    }

					    if (bYIsValid)
					    {
						    if (oVX[i].y < poSector->m_oBV.m_oIni.y) poSector->m_oBV.m_oIni.y = oVX[i].y;
				       else if (oVX[i].y > poSector->m_oBV.m_oEnd.y) poSector->m_oBV.m_oEnd.y = oVX[i].y;
					    }
			       }
			    }
		    }
	    }

	    if (poSector->m_oBV.m_oIni.x < poMap->m_oBV.m_oIni.x) poMap->m_oBV.m_oIni.x = poSector->m_oBV.m_oIni.x;
        if (poSector->m_oBV.m_oEnd.x > poMap->m_oBV.m_oEnd.x) poMap->m_oBV.m_oEnd.x = poSector->m_oBV.m_oEnd.x;
	    if (poSector->m_oBV.m_oIni.y < poMap->m_oBV.m_oIni.y) poMap->m_oBV.m_oIni.y = poSector->m_oBV.m_oIni.y;
        if (poSector->m_oBV.m_oEnd.y > poMap->m_oBV.m_oEnd.y) poMap->m_oBV.m_oEnd.y = poSector->m_oBV.m_oEnd.y;
	}
	*/

    return( poMap );
}
// ----------------------------------------------------------------------------
