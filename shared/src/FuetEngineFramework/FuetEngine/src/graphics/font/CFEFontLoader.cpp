// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFontLoader.h"
#include "Support/Graphics/CFEMaterialMgr.h"
#include "Support/Misc/CFELogger.h"
#include "Support/File/CFEFile.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Config/CFEConfigFile.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
/// Build a basic font around a given material.
CFEFont* CFEFontLoader::poBuildBasicFont(FEHandler _hMat)
{
	TCharInfo* poCharTable = (TCharInfo*)CFEMem::pAlloc(sizeof(TCharInfo) * MAX_CHAR);
    
    uint uiWidth,uiHeight;
    CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiWidth);
    CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiHeight);

	const FEReal rStep = _1r / _16r;
    for (uint i=0;i<MAX_CHAR;i++)
	{
		int iX = (i % 16);
		int iY = (i / 16);

		poCharTable[i].m_oUV.m_oIni.x = (FEReal)(iX  ) * rStep;
		poCharTable[i].m_oUV.m_oIni.y = (FEReal)(iY  ) * rStep;
		poCharTable[i].m_oUV.m_oEnd.x = (FEReal)(iX+1) * rStep;
		poCharTable[i].m_oUV.m_oEnd.y = (FEReal)(iY+1) * rStep;
		poCharTable[i].m_usCW = uiWidth  / 16;
		poCharTable[i].m_usCH = uiHeight / 16;
	    poCharTable[i].m_usOfs= 0;
	}
	
	CFEFont* poFont = new CFEFont;
	poFont->Init(_hMat,poCharTable);

	return(poFont);
}

// ----------------------------------------------------------------------------
CFEFont* CFEFontLoader::poLoadFNTFile(const CFEString& _sFilename, FEHandler _hMat)
{
	CFEFile oFile;
	if (oFile.bOpen(_sFilename,FOM_READ))
	{
	    uint uiMatWidth,uiMatHeight;
        CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiMatWidth);
        CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiMatHeight);
        FEReal r1OverW = _1r / (FEReal)uiMatWidth;        
        FEReal r1OverH = _1r / (FEReal)uiMatHeight;

		// Read base dimensions
	    unsigned short usBaseTexWidth;
	    unsigned short usBaseTexHeight;
        oFile.uiReadArray16((FEPointer)&usBaseTexWidth,1);
	  	oFile.uiReadArray16((FEPointer)&usBaseTexHeight,1);
        
        // Read default char dimensions
	    unsigned short usDefCharWidth;
	    unsigned short usDefCharHeight;
        oFile.uiReadArray16((FEPointer)&usDefCharWidth,1);
	  	oFile.uiReadArray16((FEPointer)&usDefCharHeight,1);

        // Read number of chars in the file.
        uint uiNumChars;
        oFile.uiReadArray32((FEPointer)&uiNumChars,1);

		TCharInfo* poCharTable = (TCharInfo*)CFEMem::pAlloc(sizeof(TCharInfo) * MAX_CHAR);
        uint i;

        // Setup default values for all the characters.
        for (i=0;i<MAX_CHAR;i++)
        {
			poCharTable[i].m_oUV.m_oIni.x = 0;
			poCharTable[i].m_oUV.m_oIni.y = 0;
			poCharTable[i].m_oUV.m_oEnd.x = 0;
			poCharTable[i].m_oUV.m_oEnd.y = 0;

            poCharTable[i].m_usCW  = usDefCharWidth;
            poCharTable[i].m_usCH  = usDefCharHeight;
            poCharTable[i].m_usOfs = usDefCharWidth / 6;
        }

        // Read character info from file.
		for (i=0;i<uiNumChars;i++)
		{
		    // Read char information.
			unsigned short usCharInfo[6];
            oFile.uiReadArray16((FEPointer)usCharInfo,6);

			unsigned short usChar = usCharInfo[0];
			poCharTable[usChar].m_oUV.m_oIni.x = (FEReal)(usCharInfo[1]) / (FEReal)(usBaseTexWidth);
			poCharTable[usChar].m_oUV.m_oIni.y = (FEReal)(usCharInfo[2]) / (FEReal)(usBaseTexHeight);
			poCharTable[usChar].m_oUV.m_oEnd.x = (FEReal)(usCharInfo[1]+usCharInfo[3]) / (FEReal)(usBaseTexWidth);
			poCharTable[usChar].m_oUV.m_oEnd.y = (FEReal)(usCharInfo[2]+usCharInfo[4]) / (FEReal)(usBaseTexHeight);

			/*
			poCharTable[usChar].m_oUV.m_oIni.x += r1OverW;
			poCharTable[usChar].m_oUV.m_oIni.y += r1OverH;
			poCharTable[usChar].m_oUV.m_oEnd.x += r1OverW;
			poCharTable[usChar].m_oUV.m_oEnd.y += r1OverH;
			*/

            poCharTable[usChar].m_usCW  = usCharInfo[3];
            poCharTable[usChar].m_usCH  = usCharInfo[4];
            poCharTable[usChar].m_usOfs = usCharInfo[5];

			// oFile.uiRead((FEPointer)&poCharTable[i],18);

			/*
			if ((poCharTable[i].m_oUV.m_oIni.x!=poCharTable[i].m_oUV.m_oIni.y)
				&& (poCharTable[i].m_oUV.m_oEnd.x!=poCharTable[i].m_oUV.m_oEnd.y))
			
			{
				OS_Printf("%3d - (%d,%d)",i,poCharTable[i].m_ucCW,poCharTable[i].m_ucCH);
				
				OS_Printf("(%f,%f)-(%f,%f)\n",
				poCharTable[i].m_oUV.m_oIni.x,
				poCharTable[i].m_oUV.m_oIni.y,
				poCharTable[i].m_oUV.m_oEnd.x,
				poCharTable[i].m_oUV.m_oEnd.y);
			}
			*/
		}

		CFEFont* poFont = new CFEFont;
		poFont->Init(_hMat,poCharTable);

		return(poFont);
	}

    return(NULL);	
}
// ----------------------------------------------------------------------------
CFEFont* CFEFontLoader::poLoadFONFile(const CFEString& _sFilename, FEHandler _hMat)
{
    CFEConfigFile oFile(_sFilename);
    if (oFile.bInitialized())
    {
        // Read number of rows.
        uint uiRows = oFile.iGetInteger("Font.NumRows",16);

        // Read number of cols.
        uint uiCols = oFile.iGetInteger("Font.NumCols",16);

        // Read first char.
        uint uiFirstChar = oFile.iGetInteger("Font.FirstChar",0);

        // Read last char.
        uint uiLastChar = oFile.iGetInteger("Font.LastChar",255);

        // Read char width.
        uint uiCharWidth = oFile.iGetInteger("Font.CharWidth",16);

        // Read char height.
        uint uiCharHeight = oFile.iGetInteger("Font.CharHeight",16);
    
    	TCharInfo* poCharTable = (TCharInfo*)CFEMem::pAlloc(sizeof(TCharInfo) * MAX_CHAR);
		
		FEReal rUStep = _1r / uiCols;
		FEReal rVStep = _1r / uiRows;
		
		for (uint i=0;i<MAX_CHAR;i++)
		{
		    if ((i>=uiFirstChar) && (i<=uiLastChar))
		    {
		        poCharTable[i].m_usCH = uiCharHeight;
		        poCharTable[i].m_usCW = uiCharWidth;
                poCharTable[i].m_usOfs= 0;

                uint uiElem = (i - uiFirstChar);
		        uint iX = uiElem % uiCols;
		        uint iY = uiElem / uiCols;

		        poCharTable[i].m_oUV.m_oIni.x = (FEReal)(iX  ) * rUStep;
                poCharTable[i].m_oUV.m_oIni.y = (FEReal)(iY  ) * rVStep;
                poCharTable[i].m_oUV.m_oEnd.x = (FEReal)(iX+1) * rUStep;
                poCharTable[i].m_oUV.m_oEnd.y = (FEReal)(iY+1) * rVStep;
            }
            else
            {
		        poCharTable[i].m_usCH = uiCharHeight;
		        poCharTable[i].m_usCW = uiCharWidth;
                poCharTable[i].m_usOfs= 0;

		        poCharTable[i].m_oUV.m_oIni.x = 0;
                poCharTable[i].m_oUV.m_oIni.y = 0;
                poCharTable[i].m_oUV.m_oEnd.x = 0;
                poCharTable[i].m_oUV.m_oEnd.y = 0;                
            } 
		}

		CFEFont* poFont = new CFEFont;
		poFont->Init(_hMat,poCharTable);

		return (poFont);
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
CFEFont* CFEFontLoader::poLoad(const CFEString& _sFilename)
{
    FEHandler hFontTex = CFEMaterialMgr::hLoad(_sFilename);
    if (hFontTex == NULL)
    {
        CFESystem::Log::Print("WARNING: Unable to open font material %s\n",_sFilename.szString());
        return(NULL);
    }

    CFEFont* poFont;

    // Try loading a FNT file first.
    poFont = poLoadFNTFile(_sFilename + CFEString(".fnt"),hFontTex);
    if (poFont != NULL) return(poFont);

    // Try loading a FON file then.
    poFont = poLoadFONFile(_sFilename + CFEString(".fon"),hFontTex);
    if (poFont != NULL) return(poFont);

    // Try to build a basic font
    return( poBuildBasicFont(hFontTex) );
}
// ----------------------------------------------------------------------------

