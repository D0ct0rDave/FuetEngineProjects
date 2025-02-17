// SB2FE.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <tinyxml.h>
#include <FuetEngine.h>
#include <direct.h>
#include <math.h>

#include "CConfigFileWriter.h"
#include "CLibConfigFileWriter.h"
#include "CBinConfigFileWriter.h"
// ------------------------------------------------------------------------------------------------------
int m_iRequiredParams = 1;

CFEArray<CFEString>  goSprListFiles;
bool				m_bPowerOf2Texs = false;
bool				m_bBinaryOutput = false;
CFEString			m_sFilename = "";
// ----------------------------------------------------------------------------
void ShowUsage()
{
	printf("nothing to say...\n");
	exit(0);
}
// ----------------------------------------------------------------------------
void ParseCommandLine(int argc, _TCHAR* argv[])
{	
	if ((argc-1) < m_iRequiredParams)
	{
		ShowUsage();
	}

	for (int i=1; i<argc; i++)
	{
		if (!stricmp(argv[i], "-pow2tex"))
		{
			m_bPowerOf2Texs = true;
		}
	else if (!stricmp(argv[i], "-binary"))
		{
			m_bBinaryOutput = true;
		}
	else
		{
			// Takeout quotes (when starting directly using windows explorer features:
			// double click on an specific filetype, or using sendto feature)
			char szFilename[1024];

			if (argv[i][0] == '"')
			{
				strcpy(szFilename,argv[i]+1);
				int iLen = strlen(szFilename);

				if (szFilename[iLen-1] = '"')
					szFilename[iLen-1] = 0;
			}
			else
				strcpy( szFilename,argv[i] );
			
			m_sFilename = CFEString( szFilename );
		}
	}

	if (m_sFilename == "")
		ShowUsage();
}
//---------------------------------------------------------------------------
unsigned int uiLowerPowerOf2(unsigned int _uiValue)
{
	/*
	unsigned int uiLog = 0;
	while (_uiValue>1)
	{
		 _uiValue >>=1;
		 uiLog++;
	}
    return(1<<uiLog);
    */

	unsigned int uiLog   = logf(_uiValue) / logf(2.0f);
	return(1 << uiLog);
}
// ------------------------------------------------------------------------------------------------------
void errorLog(char* str)
{
	printf("ERROR %s\n",str);
	exit(1);
}

void infoLog(char* str)
{
	char szStr[1024];
	sprintf(szStr,"INFO %s",str);
    puts(szStr);
}

void warningLog(char* str)
{
	char szStr[1024];
	sprintf(szStr,"WARNING %s",str);
    puts(szStr);
}
// ------------------------------------------------------------------------------------------------------
// XML parsing functions
// ------------------------------------------------------------------------------------------------------
const char* readXmlString( TiXmlNode* parent, char* poNodeName, char* defaultValue, bool bCanBeEmpty = false);
const char* readXmlString( TiXmlNode* parent, char* poNodeName, char* defaultValue, bool bCanBeEmpty)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
		{
			return poNode->FirstChild()->Value();
		}
		else
		{
			if (! bCanBeEmpty)
			{
				// // warningLog( "[readXmlString] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		// // warningLog( "[readXmlString] '" + poNodeName + "' poNode not found" );
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
float readXmlFloat( TiXmlNode* parent, char* poNodeName, float defaultValue, bool bCanBeEmpty = false, bool bShouldExists = true);
float readXmlFloat( TiXmlNode* parent, char* poNodeName, float defaultValue, bool bCanBeEmpty, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
		{
			//float res = atof( poNode->FirstChild()->Value() );
			float res = atof( poNode->FirstChild()->Value() );
			return res;
		}
		else
		{
			if (!bCanBeEmpty)
			{
				// warningLog( "[readXmlFloat] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		if ( bShouldExists )
		{
			// warningLog( "[readXmlFloat] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
int readXmlInt( TiXmlNode* parent, char* poNodeName, int defaultValue, bool bCanBeEmpty = false, bool bShouldExists = true);
int readXmlInt( TiXmlNode* parent, char* poNodeName, int defaultValue, bool bCanBeEmpty, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
    	{
		    return atoi( poNode->FirstChild()->Value() );
		}
		else
		{
			if (!bCanBeEmpty)
			{
				// warningLog( "[readXmlInt] Using default value in " + poNodeName );
			}
			return defaultValue;
		}
	}
	else
	{
		if (bShouldExists)
		{
			// warningLog( "[readXmlInt] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
	}
}
// ------------------------------------------------------------------------------------------------------
bool readXmlBool( TiXmlNode* parent, char* poNodeName, bool defaultValue, bool bShouldExists = true);
bool readXmlBool( TiXmlNode* parent, char* poNodeName, bool defaultValue, bool bShouldExists)
{
	TiXmlNode* poNode;
	if ( ( poNode = parent->FirstChild( poNodeName ) ) )
	{
		if ( poNode->FirstChild() )
		{
		    const char *value;
		    if ( (value = poNode->FirstChild()->Value() ) && (! strcmp(value,"true" )))	
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			// warningLog( "[readXmlBool] Using default value in " + poNodeName );
			return defaultValue;
		}
	}
	else
	{
		if (bShouldExists)
		{
			// warningLog( "[readXmlBool] '" + poNodeName + "' poNode not found" );
		}
		return defaultValue;
    }
}
// ------------------------------------------------------------------------------------------------------
CFEString sProcessBitmapPath(const char *_szFilename)
{
    CFEString sRes = CFEStringUtils::sGetFilename(_szFilename);
    sRes = CFEStringUtils::sRemoveExtension(sRes.szString());
    return(sRes);
}
// ------------------------------------------------------------------------------------------------------
bool bFileExists(const CFEString& _sFilename)
{
	FILE *fd = fopen(_sFilename.szString(),"rb");

	if (fd != NULL)
		fclose(fd);

	return(fd != NULL);
}
// ------------------------------------------------------------------------------------------------------
CFEMapElement* poReadElement(TiXmlNode* _poNode)
{
	CFEMapElement* poElement = new CFEMapElement;
	TiXmlNode* poElementNode = _poNode->FirstChild();
	
	// Read simple properties:
	poElement->m_rAngle = readXmlFloat(_poNode,"Rotation",0.0,true);
    
    float fMultX = 1.0;
    float fMultY = 1.0;

	if (readXmlBool(_poNode,"FlipHorizontally",false,true))
		fMultX = -1.0;
	if (readXmlBool(_poNode,"FlipVertically",false,true))
	    fMultY = -1.0;

	// 	
	while (poElementNode)
    {
		CFEString sName(poElementNode->Value());

	    if (sName |= "Position")
		{
			poElement->m_oPos.x = readXmlFloat(poElementNode,"X",0.0,true) * fMultX;
			poElement->m_oPos.y = readXmlFloat(poElementNode,"Y",0.0,true) * fMultY;
		}
   else if (sName |= "Scale")
		{
			poElement->m_oScale.x = readXmlFloat(poElementNode,"X",1.0,true);
			poElement->m_oScale.y = readXmlFloat(poElementNode,"Y",1.0,true);
		}
   else if (sName |= "TintColor")
		{
			poElement->m_oColor.r = readXmlFloat(poElementNode,"R",255.0,true) / 255.0f;
			poElement->m_oColor.g = readXmlFloat(poElementNode,"G",255.0,true) / 255.0f;
			poElement->m_oColor.b = readXmlFloat(poElementNode,"B",255.0,true) / 255.0f;
			poElement->m_oColor.a = readXmlFloat(poElementNode,"A",255.0,true) / 255.0f;
		}

        poElementNode = _poNode->IterateChildren(poElementNode);
    }
	
	// Read sprite or texture.
	const char* szSpriteFilename = readXmlString(_poNode,"texture_filename","",true);
	CFEString sTexture = sProcessBitmapPath( szSpriteFilename );

	bool bExists = false;
	uint i = 0;
	for (i=0;i<goSprListFiles.size();i++)
	{
		if (goSprListFiles[i] == sTexture)
		{
			bExists = true;
			break;
		}
	}
	
	// Aaahh... those dirty tricks...
	if (bExists)
	{	
		*((uint*)&poElement->m_rDepth) = i;
	}
	else
	{
		*((uint*)&poElement->m_rDepth) = goSprListFiles.size();
		goSprListFiles.push_back( sTexture.szString() );
	}
	
	// Create a sprite instance.
	poElement->m_hSprInst = CFESpriteInstMgr::hGetInstance(sTexture);
	
	// Assign geometric properties to the sprite instance
	if (poElement->m_hSprInst!= NULL)
	{
		// If textures will be powerized to 2 then process element scale. Sprites have it's own scale parameters.
		if ((m_bPowerOf2Texs) && (! bFileExists(sTexture+".spr")))
		{		
			FEHandler* hTex = CFEMaterialMgr::poLoad(sTexture);
			
			uint uiWidth,uiHeight;
			CFEMaterialMgr::bGetMaterialProperty(hTex,"DiffuseMap.Width",(FEPointer)&uiWidth);
			CFEMaterialMgr::bGetMaterialProperty(hTex,"DiffuseMap.Height",(FEPointer)&uiHeight);

			FEReal rXRatio = (FEReal)uiWidth / (FEReal)uiLowerPowerOf2(uiWidth);
			FEReal rYRatio = (FEReal)uiHeight / (FEReal)uiLowerPowerOf2(uiHeight);

			// Fix element scale
			poElement->m_oScale.x *= rXRatio;
			poElement->m_oScale.y *= rYRatio;
		}

		// Set sprite properties
		CFESpriteInstMgr::SetPos(poElement->m_hSprInst,poElement->m_oPos);
		CFESpriteInstMgr::SetAngle(poElement->m_hSprInst,poElement->m_rAngle);
		CFESpriteInstMgr::SetScale(poElement->m_hSprInst,poElement->m_oScale);
	}
	else
	{
		CFEString sErrorStr = CFEString("Unable to load ") + sTexture + CFEString(" File");
		warningLog((char*)sErrorStr.szString());
	}

    return(poElement);
}
// ------------------------------------------------------------------------------------------------------
CFEMapLayer* poReadLayer(TiXmlNode* _poNode)
{ 
    CFEMapLayer* poLayer= new CFEMapLayer;
    CFEMapSector* poSector = new CFEMapSector;
    
    TiXmlAttribute* poName = ((TiXmlElement*)_poNode)->FirstAttribute();
    CFEString sName = poName->Value(); // readXmlString(_poNode,"Name","",false);
    // Retrieve layer depth.
    CFEString sDepth = sName.SubString(0,3);
    if (sscanf(sDepth.szString(),"%f",&poLayer->m_rDepth))
    {
        poLayer->m_sName = sName.SubString(4,sName.uiLen()-4);
        poLayer->m_rDepth /= 100.0f;
    }
    else
    {
        poLayer->m_sName = sName;
        poLayer->m_rDepth = _1r;
    }

    // 
    TiXmlNode* poSpeed = _poNode->FirstChild("ScrollSpeed");
    poLayer->m_oParallaxFact.x = readXmlFloat(poSpeed,"X",0.0,true);
    poLayer->m_oParallaxFact.y = readXmlFloat(poSpeed,"Y",0.0,true);

	TiXmlNode* poItems = _poNode->FirstChild("Items");
    TiXmlNode* poItemNode = poItems->FirstChild();
    
    ///
    while (poItemNode)
    {
        TiXmlAttribute* poAttr = ((TiXmlElement*)poItemNode)->FirstAttribute();
        
        while ((poAttr != NULL) && stricmp("xsi:type",poAttr->Name()))
            poAttr = poAttr->Next();
        
        if (poAttr!=NULL)
        {
            if (! stricmp(poAttr->Value(),"TextureItem"))
            {
                CFEMapElement* poElem = poReadElement(poItemNode);
                poSector->m_oElements.push_back(*poElem);
            }
        }

        poItemNode = poItems->IterateChildren(poItemNode);
    }

	poLayer->m_poSectors.push_back(poSector);
    return(poLayer);
}
// ------------------------------------------------------------------------------------------------------
void ProcessSector(CFEMapSector* _poSector)
{
	_poSector->m_oBV.m_oEnd = CFEVect2(-1e6f,-1e6f);
	_poSector->m_oBV.m_oIni = CFEVect2( 1e6f, 1e6f);
	bool bValidBV = false;

    // Compute Map Bounding Volume
    for (uint uiElem=0;uiElem<_poSector->m_oElements.size();uiElem++)
    {
	    CFEMapElement* poElem = &_poSector->m_oElements[uiElem];
	    
	    if (poElem->m_hSprInst != NULL)
	    {
			CFEVect2 oVX[4];
			CFESpriteInstMgr::GetGeometry(poElem->m_hSprInst,oVX);

			for (uint i=0;i<4;i++)
			{
				if (oVX[i].x < _poSector->m_oBV.m_oIni.x) _poSector->m_oBV.m_oIni.x = oVX[i].x;
				if (oVX[i].x > _poSector->m_oBV.m_oEnd.x) _poSector->m_oBV.m_oEnd.x = oVX[i].x;
				if (oVX[i].y < _poSector->m_oBV.m_oIni.y) _poSector->m_oBV.m_oIni.y = oVX[i].y;
				if (oVX[i].y > _poSector->m_oBV.m_oEnd.y) _poSector->m_oBV.m_oEnd.y = oVX[i].y;
			}
			
			bValidBV = true;
		}
    }

    if (bValidBV == false)
    {
		// Create a BV based on positions

		// Compute Map Bounding Volume
		for (uint uiElem=0;uiElem<_poSector->m_oElements.size();uiElem++)
		{
			CFEMapElement* poElem = &_poSector->m_oElements[uiElem];

			if (poElem->m_oPos.x < _poSector->m_oBV.m_oIni.x) _poSector->m_oBV.m_oIni.x = poElem->m_oPos.x;
			if (poElem->m_oPos.x > _poSector->m_oBV.m_oEnd.x) _poSector->m_oBV.m_oEnd.x = poElem->m_oPos.x;
			if (poElem->m_oPos.y < _poSector->m_oBV.m_oIni.y) _poSector->m_oBV.m_oIni.y = poElem->m_oPos.y;
			if (poElem->m_oPos.y > _poSector->m_oBV.m_oEnd.y) _poSector->m_oBV.m_oEnd.y = poElem->m_oPos.y;
		}
    }
}
// ------------------------------------------------------------------------------------------------------
void SectorizeLayer(CFEMapLayer* _poLayer)
{
    CFEMapSector* poOriginalSector = _poLayer->m_poSectors[0];

    // Compute original sector extents.
    CFERect oOriginalBV = poOriginalSector->m_oBV;

    ProcessSector(poOriginalSector);    
    CFEVect2 oExtents = poOriginalSector->m_oBV.m_oEnd - poOriginalSector->m_oBV.m_oIni;

	if ((oExtents.x<0.0) || (oExtents.y<0.0))
	{
		poOriginalSector->m_oBV = oOriginalBV;
		return;
	}

    const FEReal SECTOR_WIDTH  = 640;
    const FEReal SECTOR_HEIGHT = 480;    
    uint uiNumXSectors = (oExtents.x / SECTOR_WIDTH)+1;
    uint uiNumYSectors = (oExtents.y / SECTOR_HEIGHT)+1;
    
    CFEMapSector* poSectorList = new CFEMapSector[uiNumXSectors*uiNumYSectors];

    for (uint uiElem=0;uiElem<poOriginalSector->m_oElements.size();uiElem++)
    {
        CFEMapElement* poElem = &poOriginalSector->m_oElements[uiElem];

        // look if the element falls inside the sector
        int iXSector = (poElem->m_oPos.x - poOriginalSector->m_oBV.m_oIni.x) / SECTOR_WIDTH;
        int iYSector = (poElem->m_oPos.y - poOriginalSector->m_oBV.m_oIni.y) / SECTOR_HEIGHT;

		if (iXSector >= (int)uiNumXSectors)
			iXSector = uiNumXSectors-1;
		else
			if (iXSector<0)
				iXSector = 0;
		
		if (iYSector >= (int)uiNumYSectors)
			iYSector = uiNumYSectors-1;
		else
			if (iYSector<0)
				iYSector = 0;

        CFEMapSector* poSector = &poSectorList[iYSector*uiNumXSectors + iXSector];
        poSector->m_oElements.push_back(*poElem);
    }

	// Add sectors to the layers
    _poLayer->m_poSectors.clear();
    for (uint uiSector=0;uiSector<uiNumXSectors*uiNumYSectors;uiSector++)
    {
        CFEMapSector*poSector = &poSectorList[uiSector];

        if (poSector->m_oElements.size() > 0)
        {
            ProcessSector(poSector);
            _poLayer->m_poSectors.push_back(poSector);
        }
    }
}
// ------------------------------------------------------------------------------------------------------
void ProcessMap(CFEMap* _poMap)
{
	_poMap->m_oBV.m_oEnd = CFEVect2(-1e6f,-1e6f);
	_poMap->m_oBV.m_oIni = CFEVect2( 1e6f, 1e6f);

	// Compute Map Bounding Volume
    for (uint uiLayer=0;uiLayer<_poMap->m_poLayers.size();uiLayer++)
    {
        CFEMapLayer* poLayer = _poMap->m_poLayers[uiLayer];
        if (poLayer== NULL) continue;

		// Process layer
    	SectorizeLayer(poLayer);
		
		const bool bIsValidLayer = (poLayer->m_poSectors.size() > 0) && (poLayer->m_poSectors[0]->m_oElements.size() > 0);

		// Compute map bounding volume
		bool bXIsValid = (poLayer->m_oParallaxFact.x == 1.0);
		bool bYIsValid = (poLayer->m_oParallaxFact.y == 1.0);
    		
		if ((bXIsValid || bYIsValid) && bIsValidLayer)
		{
			for (uint uiSector=0;uiSector<poLayer->m_poSectors.size();uiSector++)
			{
				CFEMapSector* poSector = poLayer->m_poSectors[uiSector];
				if (poSector == NULL) continue;

				if (poSector->m_oBV.m_oIni.x < _poMap->m_oBV.m_oIni.x) _poMap->m_oBV.m_oIni.x = poSector->m_oBV.m_oIni.x;
				if (poSector->m_oBV.m_oEnd.x > _poMap->m_oBV.m_oEnd.x) _poMap->m_oBV.m_oEnd.x = poSector->m_oBV.m_oEnd.x;
				if (poSector->m_oBV.m_oIni.y < _poMap->m_oBV.m_oIni.y) _poMap->m_oBV.m_oIni.y = poSector->m_oBV.m_oIni.y;
				if (poSector->m_oBV.m_oEnd.y > _poMap->m_oBV.m_oEnd.y) _poMap->m_oBV.m_oEnd.y = poSector->m_oBV.m_oEnd.y;
			}
	    }
	}
}
// ------------------------------------------------------------------------------------------------------
// saving 
// ------------------------------------------------------------------------------------------------------
void SaveSpriteSet(CConfigFileWriter* _poFD)
{
	// ---------
	// SpriteSet
	// ---------
	_poFD->OpenSection("SpriteSet");

        _poFD->AddVar("NumSprites",goSprListFiles.size(),(uint)0);
    	for (uint uiSpr = 0;uiSpr<goSprListFiles.size();uiSpr++)
	    {
            CFEString sStr = CFEString("Sprite")+CFEString(uiSpr);
            _poFD->AddVar(sStr.szString(),goSprListFiles[uiSpr].szString(),"");
        }

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------6
void SaveBoundingVolume(CFERect* _poBV,CConfigFileWriter* _poFD)
{
    _poFD->OpenSection("BV");

        _poFD->OpenSection("Mins");
            _poFD->AddVar("x",_poBV->m_oIni.x,_0r,1);
            _poFD->AddVar("y",_poBV->m_oIni.y,_0r,1);
        _poFD->CloseSection();

        _poFD->OpenSection("Maxs");
            _poFD->AddVar("x",_poBV->m_oEnd.x,_0r,1);
            _poFD->AddVar("y",_poBV->m_oEnd.y,_0r,1);
        _poFD->CloseSection();

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------
void SaveMap(CFEMap* _poMap,const char* _szFilename)
{
	// m_bBinaryOutput
    CConfigFileWriter* poFD = NULL;
    if (m_bBinaryOutput)
		poFD = new CBinConfigFileWriter(_szFilename);
	else
		poFD = new CLibConfigFileWriter(_szFilename);


    if (! poFD->bCreate(_szFilename)) return;
    poFD->OpenSection("Map");

		// ---------
		// Bounding Volume
		// ---------
		SaveBoundingVolume(&_poMap->m_oBV,poFD);

		// ---------
		// SpriteSet
		// ---------
		SaveSpriteSet(poFD);

        // ---------
        // Layers
        // ---------
        poFD->OpenSection("Layers");
            
            poFD->AddVar("NumLayers",_poMap->m_poLayers.size(),(uint)0);
            FEReal rDepth = 0.0;

            for (uint uiLayer=0;uiLayer<_poMap->m_poLayers.size();uiLayer++)
            {
                CFEMapLayer* poLayer = _poMap->m_poLayers[uiLayer];
                if (poLayer == NULL) continue;

	            // ---------
	            // Layer X
	            // ---------
	            poFD->OpenSection("Layer",uiLayer);

                    poFD->AddVar("Name",poLayer->m_sName.szString(),"");
                    
                    // Layer Depth
                    poFD->AddVar("Depth",poLayer->m_rDepth,_0r,5);

		            // Layer speed
		            poFD->OpenSection("Parallax");
		                
		                poFD->AddVar("x",poLayer->m_oParallaxFact.x,_0r,5);
			            poFD->AddVar("y",poLayer->m_oParallaxFact.y,_0r,5);

		            poFD->CloseSection();

					// ---------
					// Sectors
					// ---------
					poFD->OpenSection("Sectors");

						poFD->AddVar("NumSectors",poLayer->m_poSectors.size(),(uint)0);

						for (uint uiSector=0;uiSector<poLayer->m_poSectors.size();uiSector++)
						{
							CFEMapSector* poSector = poLayer->m_poSectors[uiSector];
							if (poSector == NULL) continue;

							// ---------
							// Sector X
							// ---------
							poFD->OpenSection("Sector",uiSector);

								// ---------
								// Bounding Volume
								// ---------
								SaveBoundingVolume(&poSector->m_oBV,poFD);

								// ---------
								// Elements
								// ---------
								poFD->OpenSection("Elements");

									poFD->AddVar("NumElements",poSector->m_oElements.size(),(uint)0);

									for (uint uiElem=0;uiElem<poSector->m_oElements.size();uiElem++)
									{
										CFEMapElement* poElem = &poSector->m_oElements[uiElem];
							            
										poFD->OpenSection("Element",uiElem);
							            
											// write the sprite index
											poFD->AddVar("SpriteIdx",*((unsigned int*)&poElem->m_rDepth),(uint)0xffffffff);

											// Position
											poFD->OpenSection("Position");

												poFD->AddVar("x",poElem->m_oPos.x,_0r,5);
												poFD->AddVar("y",poElem->m_oPos.y,_0r,5);

											poFD->CloseSection();

											// Scale
											poFD->OpenSection("Scale");

												poFD->AddVar("x",poElem->m_oScale.x,_1r,5);
												poFD->AddVar("y",poElem->m_oScale.y,_1r,5);

											poFD->CloseSection();

											poFD->AddVar("Depth",rDepth, _0r, 5); rDepth += 1.0;
											poFD->AddVar("Angle",poElem->m_rAngle,_0r,5);

											// Color
											poFD->OpenSection("Color");

												poFD->AddVar("r",poElem->m_oColor.r,_1r,5);
												poFD->AddVar("g",poElem->m_oColor.g,_1r,5);
												poFD->AddVar("b",poElem->m_oColor.b,_1r,5);
												poFD->AddVar("a",poElem->m_oColor.a,_1r,5);

											poFD->CloseSection();

										poFD->CloseSection(); // Element X
									} // for each element

								poFD->CloseSection(); // Elements

							poFD->CloseSection(); // Sector X
						} // for each sector 

					poFD->CloseSection(); // Sectors
                
                poFD->CloseSection(); // Layer X
            } // for each layer

		poFD->CloseSection(); // Layers

    poFD->CloseSection(); // Map
    poFD->Write();
}
// ------------------------------------------------------------------------------------------------------
void SavePath(CFEArray<CFEVect2>& _oPntList,const char* _szFilename)
{
	// m_bBinaryOutput
    CConfigFileWriter* poFD = NULL;
    if (m_bBinaryOutput)
		poFD = new CBinConfigFileWriter(_szFilename);
	else
		poFD = new CLibConfigFileWriter(_szFilename);

    poFD->OpenSection("Path");

        poFD->AddVar("NumPoints",_oPntList.size(),(uint)0);
        for (uint i=0;i<_oPntList.size();i++)
        {
            poFD->OpenSection("Point",i);
                
                poFD->AddVar("x",_oPntList[i].x,_0r);
                poFD->AddVar("y",_oPntList[i].y,_0r);

            poFD->CloseSection();
        }

    poFD->CloseSection();
    poFD->Write();
}
// ------------------------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	ParseCommandLine(argc,argv);

	TiXmlDocument oDoc;
	if (argc < 1)
		errorLog("missing input filename");

	// Setup filename
	CFEString sFullPath = CFEString(m_sFilename);
	CFEString sFilename = CFEStringUtils::sGetFilename(sFullPath);
	CFEString sPath		= CFEStringUtils::sGetPath(sFullPath);

	// 
	if (sPath != "")
		chdir(sPath.szString());

	if (oDoc.LoadFile(sFilename.szString()) == false)
	{
	    errorLog("FileNotFound");
		return NULL;
	}

    TiXmlNode* poNode;
	if ( !( poNode = oDoc.FirstChild("Level") ))
	{
		errorLog("First tag not found");
		return NULL;
	}

	if ( !( poNode = poNode->FirstChild("Layers") ) )
	{
		errorLog("layers tag not found");
		return NULL;
	}

	CFESpriteInstMgr::Init(1024);
	CFEMap* poMap = new CFEMap;

    TiXmlNode* poLayerNode = poNode->FirstChild();
    while (poLayerNode)
    {
        CFEMapLayer* poMapLayer = poReadLayer(poLayerNode);
        poMap->m_poLayers.push_back(poMapLayer);

        poLayerNode = poNode->IterateChildren(poLayerNode);
    }

    // Process map ...
    ProcessMap(poMap);

    // Save map
	CFEString sOutputFilename = sPath + CFEString("/") + CFEStringUtils::sRemoveExtension(sFilename) + CFEString(".map");
    SaveMap(poMap,sOutputFilename.szString());

	return 0;
}
// ------------------------------------------------------------------------------------------------------
