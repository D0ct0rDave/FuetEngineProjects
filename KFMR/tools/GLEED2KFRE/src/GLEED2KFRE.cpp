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
typedef struct TKFRMGroundSegment
{
    uint        m_uiType;
    CFESegment  m_oSegment;  
};

typedef struct TKFRMPSys
{
    CFEString   m_sType;
    CFEVect2    m_oPos;
    FEReal      m_rDepth;
};

typedef struct TKFRMEntity
{
    CFEString   m_sType;
    CFEVect2    m_oPos;
    FEReal      m_rDepth;
};

typedef struct TKFRMControlPoint
{
    uint        m_uiType;
    CFEVect2    m_oPos;
};

CFEArray<TKFRMPSys>             m_oPSysList;
CFEArray<TKFRMEntity>           m_oEntityList;
CFEArray<TKFRMControlPoint>     m_oCPList;
CFEArray<TKFRMGroundSegment>    m_oSegList;

bool				    m_bBinaryOutput = false;
CFEString			    m_sFilename = "";

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
	    if (!stricmp(argv[i], "-binary"))
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
void ProcessElement(TiXmlNode* _poNode, const CFEString& _sItemType,FEReal _rDepth)
{
	TiXmlNode* poElementNode = _poNode->FirstChild();

	// Read position.
	CFEVect2 oPos;
	while (poElementNode)
    {
		CFEString sName(poElementNode->Value());

	    if (sName |= "Position")
		{
			oPos.x = readXmlFloat(poElementNode,"X",0.0,true);
			oPos.y = readXmlFloat(poElementNode,"Y",0.0,true);
		}

        poElementNode = _poNode->IterateChildren(poElementNode);
    }

    // Retrieve name
    TiXmlAttribute* poAttr = ((TiXmlElement*)_poNode)->FirstAttribute();
    while ((poAttr != NULL) && stricmp("Name",poAttr->Name()))
        poAttr = poAttr->Next();

    if (poAttr!=NULL)
    {
        CFEString sName = CFEString(poAttr->Value());

        // Control Point
        if (_sItemType |= "RectangleItem")
        {
            TKFRMControlPoint oCP;
            
            if (sName |= "StartPoint")
            {
                oCP.m_uiType = 1;
            }
       else if (sName |= "EndPoint")
            {
                oCP.m_uiType = 2;
            }
       else if (sName |= "Cinematic")
            {
                oCP.m_uiType = 3;
                // cinematic plane ???
            }
                        
            oCP.m_oPos = oPos;
            m_oCPList.push_back(oCP);
        }

        // Particle systems or Entity
   else if (_sItemType |= "CircleItem")
        {
            if (sName.SubString(0,3) |= CFEString("FX_"))
            {
                TKFRMPSys oPS;
                oPS.m_sType = sName.SubString(3,sName.uiLen() - 3);
                oPS.m_oPos = oPos;
                oPS.m_rDepth = _rDepth;
                m_oPSysList.push_back(oPS);
            }

       else if (sName.SubString(0,4) |= CFEString("ENT_"))
            {
                TKFRMEntity oEnt;
                oEnt.m_sType = sName.SubString(4,sName.uiLen() - 4);
                oEnt.m_oPos = oPos;
                oEnt.m_rDepth = _rDepth;
                m_oEntityList.push_back(oEnt);
            }
        }
    }
}
// ------------------------------------------------------------------------------------------------------
uint uiGetGroundType(TiXmlNode* _poNode)
{
    uint uiRType = 0;
	TiXmlNode* poElementNode = _poNode->FirstChild();

	// Read position.
	while (poElementNode)
    {
		CFEString sName(poElementNode->Value());

	    if (sName |= "LineColor")
			uiRType = readXmlInt(poElementNode,"G",0,true);

        poElementNode = _poNode->IterateChildren(poElementNode);
    }

    return(uiRType);
}
// ------------------------------------------------------------------------------------------------------
void ProcessPath(TiXmlNode* _poNode)
{
	// Read ground type encoded as the red element of the fill color
    uint uiGroundType = uiGetGroundType(_poNode);

    // read points
    TiXmlNode* poItems    = _poNode->FirstChild("WorldPoints");
    TiXmlNode* poItemNode = poItems->FirstChild();

    bool bFirstPoint = true;
    CFEVect2 oLastPoint = CFEVect2::ZERO();

    while (poItemNode)
    {
        CFEVect2 oPos;
		oPos.x = readXmlFloat(poItemNode,"X",0.0,true);
		oPos.y = readXmlFloat(poItemNode,"Y",0.0,true);
		
		if (bFirstPoint)
		{
		    bFirstPoint = false;
		}
		else
		{
		    TKFRMGroundSegment oSeg;
		    oSeg.m_oSegment.m_oIni = oPos;
		    oSeg.m_oSegment.m_oEnd = oLastPoint; // strange 
		    oSeg.m_uiType = uiGroundType;

            m_oSegList.push_back(oSeg);
		}

        oLastPoint = oPos;
        poItemNode = poItems->IterateChildren(poItemNode);
    }
}
// ------------------------------------------------------------------------------------------------------
void ProcessLayer(TiXmlNode* _poNode)
{ 
    TiXmlAttribute* poName = ((TiXmlElement*)_poNode)->FirstAttribute();
    CFEString sName = poName->Value(); // readXmlString(_poNode,"Name","",false);

    // Retrieve layer depth.
    FEReal rDepth           = 0.0f;
    CFEString sDepth        = sName.SubString(0,3);
    CFEString sLayerName    = sName.SubString(0,3);

    if (sscanf(sDepth.szString(),"%f",&rDepth))
    {
        sLayerName = sName.SubString(4,sName.uiLen()-4);
        rDepth    /= 100.0f;
    }
    else
    {
        sLayerName = sName;
        rDepth     = _1r;
    }

    // 
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
            if (
                    (!stricmp(poAttr->Value(),"CircleItem"))
                ||  (!stricmp(poAttr->Value(),"RectangleItem"))
                )
            {
                ProcessElement(poItemNode, CFEString(poAttr->Value()), rDepth);
            }
       else if (!stricmp(poAttr->Value(),"PathItem"))
            {
                ProcessPath(poItemNode);
            }
        }

        poItemNode = poItems->IterateChildren(poItemNode);
    }
}
// ------------------------------------------------------------------------------------------------------
// saving 
// ------------------------------------------------------------------------------------------------------
void SaveSpriteSet(CConfigFileWriter* _poFD)
{
    /*
	// ---------
	// SpriteSet
	// ---------
	_poFD->OpenSection("SpriteSet");

        _poFD->AddVar("NumSprites",goSprListFiles.size());
    	for (uint uiSpr = 0;uiSpr<goSprListFiles.size();uiSpr++)
	    {
            CFEString sStr = CFEString("Sprite")+CFEString(uiSpr);
            _poFD->AddVar(sStr.szString(),goSprListFiles[uiSpr].szString());
        }

    _poFD->CloseSection();
    */
}
// ------------------------------------------------------------------------------------------------------6
void SaveBoundingVolume(CFERect* _poBV,CConfigFileWriter* _poFD)
{
    /*
    _poFD->OpenSection("BV");

        _poFD->OpenSection("Mins");
            _poFD->AddVar("x",_poBV->m_oIni.x,1);
            _poFD->AddVar("y",_poBV->m_oIni.y,1);
        _poFD->CloseSection();

        _poFD->OpenSection("Maxs");
            _poFD->AddVar("x",_poBV->m_oEnd.x,1);
            _poFD->AddVar("y",_poBV->m_oEnd.y,1);
        _poFD->CloseSection();

    _poFD->CloseSection();
    */
}
// ------------------------------------------------------------------------------------------------------
void SaveControlPoints(CConfigFileWriter* _poFD)
{
	// m_bBinaryOutput
    _poFD->OpenSection("ControlPoints");

        _poFD->AddVar("NumCPs",m_oCPList.size());       
        for (uint i=0;i<m_oCPList.size();i++)
        {
            TKFRMControlPoint oCP = m_oCPList[i];

            _poFD->OpenSection("CP",i);

                _poFD->AddVar("X",oCP.m_oPos.x);
                _poFD->AddVar("Y",oCP.m_oPos.y);
                _poFD->AddVar("Type",oCP.m_uiType);

            _poFD->CloseSection();
        }

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------
void SaveParticleSystems(CConfigFileWriter* _poFD)
{
	// m_bBinaryOutput
    _poFD->OpenSection("ParticleSystems");

        _poFD->AddVar("NumPSs",m_oPSysList.size());       
        for (uint i=0;i<m_oPSysList.size();i++)
        {
            TKFRMPSys oPS = m_oPSysList[i];

            _poFD->OpenSection("PS",i);

                _poFD->AddVar("X",oPS.m_oPos.x);
                _poFD->AddVar("Y",oPS.m_oPos.y);
                _poFD->AddVar("Depth",oPS.m_rDepth);
                _poFD->AddVar("Type",oPS.m_sType.szString());

            _poFD->CloseSection();
        }

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------
void SaveEntities(CConfigFileWriter* _poFD)
{
	// m_bBinaryOutput
    _poFD->OpenSection("Entities");

        _poFD->AddVar("NumEntities",m_oEntityList.size());       
        for (uint i=0;i<m_oPSysList.size();i++)
        {
            TKFRMEntity oEnt = m_oEntityList[i];

            _poFD->OpenSection("Ent",i);

                _poFD->AddVar("X",oEnt.m_oPos.x);
                _poFD->AddVar("Y",oEnt.m_oPos.y);
                _poFD->AddVar("Depth",oEnt.m_rDepth);
                _poFD->AddVar("Type",oEnt.m_sType.szString());

            _poFD->CloseSection();
        }

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------
void SaveGround(CConfigFileWriter* _poFD)
{
	// m_bBinaryOutput
    _poFD->OpenSection("Ground");

        _poFD->AddVar("NumSegments",m_oSegList.size());       
        for (uint i=0;i<m_oSegList.size();i++)
        {
            TKFRMGroundSegment oSeg = m_oSegList[i];
            
            _poFD->OpenSection("Segment",i);

                _poFD->AddVar("IniX",oSeg.m_oSegment.m_oIni.x);
                _poFD->AddVar("IniY",oSeg.m_oSegment.m_oIni.y);
                _poFD->AddVar("EndX",oSeg.m_oSegment.m_oEnd.x);
                _poFD->AddVar("EndY",oSeg.m_oSegment.m_oEnd.y);
                _poFD->AddVar("Type",oSeg.m_uiType);

            _poFD->CloseSection();
        }

    _poFD->CloseSection();
}
// ------------------------------------------------------------------------------------------------------
void SaveMap(CFEMap* _poMap,const char* _szFilename)
{
	// m_bBinaryOutput
    CConfigFileWriter* _poFD = NULL;
    if (m_bBinaryOutput)
		_poFD = new CBinConfigFileWriter(_szFilename);
	else
		_poFD = new CLibConfigFileWriter(_szFilename);

    if (! _poFD->bCreate(_szFilename)) return;
    _poFD->OpenSection("KFRMMap");

		// ---------
		// Save Control Points
		// ---------
		SaveControlPoints(_poFD);

		// ---------
		// Save Entities
		// ---------
		SaveEntities(_poFD);

		// ---------
		// Save Particle FX
		// ---------
		SaveParticleSystems(_poFD);

		// ---------
		// Save Bounding Volume
		// ---------
		SaveGround(_poFD);

    _poFD->CloseSection(); // KFRMMap
    _poFD->Write();
}
// ------------------------------------------------------------------------------------------------------
// Extraer:
// Control points: Start Point (pos) / EndPoint/BossPoint (pos) / Cinematic Point (Position/PlaneNum). : QUADS
// particles: depth / position / type. : CIRCLES Green 0xff00ff00
// entities: depth / position / type.  : CIRCLES Rosa fucsia 0xffff00ff
// ground: height / type. : LINES
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
        ProcessLayer(poLayerNode);
        poLayerNode = poNode->IterateChildren(poLayerNode);
    }

    // Save map
	CFEString sOutputFilename = sPath + CFEString("/") + CFEStringUtils::sRemoveExtension(sFilename) + CFEString(".km");
    SaveMap(poMap,sOutputFilename.szString());

	return 0;
}
// ------------------------------------------------------------------------------------------------------
