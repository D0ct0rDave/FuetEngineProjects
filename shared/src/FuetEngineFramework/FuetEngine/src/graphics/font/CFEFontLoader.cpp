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
TCharInfo* CFEFontLoader::poCreateCharTable(uint _uiDefCharWidth,uint _uiDefCharHeight)
{
	TCharInfo* poCharTable = (TCharInfo*)CFEMem::pAlloc(sizeof(TCharInfo) * MAX_CHAR);

    // Setup default values for all the characters.
    for (uint i=0;i<MAX_CHAR;i++)
    {
		poCharTable[i].m_oUV.m_oIni.x = 0;
		poCharTable[i].m_oUV.m_oIni.y = 0;
		poCharTable[i].m_oUV.m_oEnd.x = 0;
		poCharTable[i].m_oUV.m_oEnd.y = 0;
		
        poCharTable[i].m_usCW  = _uiDefCharWidth;
        poCharTable[i].m_usCH  = _uiDefCharHeight;
        poCharTable[i].m_sYOfs = _uiDefCharWidth / 6;
        poCharTable[i].m_cLC   = 0;
        poCharTable[i].m_cRC   = 0;
    }

	return(	poCharTable);
}
// ----------------------------------------------------------------------------
/// Build a basic font around a given material.
CFEFont* CFEFontLoader::poBuildBasicFont(FEHandler _hMat)
{
    uint uiWidth,uiHeight;
    CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiWidth);
    CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiHeight);
	TCharInfo* poCharTable = poCreateCharTable(uiWidth /16,uiHeight/16);

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
	    poCharTable[i].m_sYOfs= 0;
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

		TCharInfo* poCharTable = poCreateCharTable(usDefCharWidth,usDefCharHeight);

        // Read character info from file.
		for (uint i=0;i<uiNumChars;i++)
		{
		    // Read char information.
			unsigned short usCharInfo[6];
            oFile.uiReadArray16((FEPointer)usCharInfo,6);

			unsigned short usChar = usCharInfo[0];
			poCharTable[usChar].m_oUV.m_oIni.x = (FEReal)(usCharInfo[1]) / (FEReal)(usBaseTexWidth);
			poCharTable[usChar].m_oUV.m_oIni.y = (FEReal)(usCharInfo[2]) / (FEReal)(usBaseTexHeight);
			poCharTable[usChar].m_oUV.m_oEnd.x = (FEReal)(usCharInfo[1]+usCharInfo[3]) / (FEReal)(usBaseTexWidth);
			poCharTable[usChar].m_oUV.m_oEnd.y = (FEReal)(usCharInfo[2]+usCharInfo[4]) / (FEReal)(usBaseTexHeight);

            poCharTable[usChar].m_usCW  = usCharInfo[3];
            poCharTable[usChar].m_usCH  = usCharInfo[4];
            poCharTable[usChar].m_sYOfs = usCharInfo[5];
		}

		// finally close the file
		oFile.Close();

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
		CFEString sType = oFile.sGetString("Font.Type","Regular");
		if (sType |= "Regular")
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

			TCharInfo* poCharTable = poCreateCharTable(uiCharWidth,uiCharHeight);

			FEReal rUStep = _1r / uiCols;
			FEReal rVStep = _1r / uiRows;

			for (uint i=0;i<MAX_CHAR;i++)
			{
				if ((i>=uiFirstChar) && (i<=uiLastChar))
				{
					poCharTable[i].m_usCH = uiCharHeight;
					poCharTable[i].m_usCW = uiCharWidth;
					poCharTable[i].m_sYOfs= 0;

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
					poCharTable[i].m_sYOfs= 0;

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
	else if (sType |= "OpenType")
		{
			uint uiMatWidth,uiMatHeight;
			CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiMatWidth);
			CFEMaterialMgr::bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiMatHeight);
			
			uint uiBaseTexWidth,uiBaseTexHeight;
			uint uiDefCharWidth,uiDefCharHeight;

			uiBaseTexWidth  = oFile.iGetInteger("Font.OpenType.BaseWidth",0);
			uiBaseTexHeight = oFile.iGetInteger("Font.OpenType.BaseHeight",0);
			uiDefCharWidth  = oFile.iGetInteger("Font.OpenType.DefCharWidth",0);
			uiDefCharHeight = oFile.iGetInteger("Font.OpenType.DefCharHeight",0);

			TCharInfo* poCharTable = poCreateCharTable(uiDefCharWidth,uiDefCharHeight);
			
			unsigned int uiNumChars = oFile.iGetInteger("Font.OpenType.NumChars",0);
			
			// Read character info from file.
			for (uint i=0;i<uiNumChars;i++)
			{			
				CFEString sSection = CFEString("Font.OpenType.C")+CFEString(i);

				// Read char information.
				uint uiID	 = oFile.iGetInteger(sSection+".ID",0);
				uint uiCharX = oFile.iGetInteger(sSection+".S",0);
				uint uiCharY = oFile.iGetInteger(sSection+".T",0);
				uint uiCharW = oFile.iGetInteger(sSection+".W",0);
				uint uiCharH = oFile.iGetInteger(sSection+".H",0);

				if (uiID>255) continue;

				poCharTable[uiID].m_oUV.m_oIni.x = (FEReal)(uiCharX) / (FEReal)(uiBaseTexWidth);
				poCharTable[uiID].m_oUV.m_oIni.y = (FEReal)(uiCharY) / (FEReal)(uiBaseTexHeight);
				poCharTable[uiID].m_oUV.m_oEnd.x = (FEReal)(uiCharX+uiCharW) / (FEReal)(uiBaseTexWidth);
				poCharTable[uiID].m_oUV.m_oEnd.y = (FEReal)(uiCharY+uiCharH) / (FEReal)(uiBaseTexHeight);

				poCharTable[uiID].m_usCW  = uiCharW;
				poCharTable[uiID].m_usCH  = uiCharH;
				poCharTable[uiID].m_sYOfs = oFile.iGetInteger(sSection+".YOfs",0);

				poCharTable[uiID].m_cLC   = oFile.iGetInteger(sSection+".LClass",0);
				poCharTable[uiID].m_cRC   = oFile.iGetInteger(sSection+".RClass",0);
			}
	
			// Read the kern table
			int iLCs = oFile.iGetInteger("Font.OpenType.KerningTable.NumLClasses",0);
			int iRCs = oFile.iGetInteger("Font.OpenType.KerningTable.NumRClasses",0);
			CFEKernTable* poKT = NULL;

			if ((iLCs*iRCs)>0)
			{
				poKT = new CFEKernTable(iLCs,iRCs);

				for (uint r=0;r<iRCs;r++)
					for (uint l=0;l<iLCs;l++)
					{
						CFEString sSection = CFEString("Font.OpenType.KerningTable.m");
						poKT->SetKern(l, r,	oFile.iGetInteger(sSection + CFEString(l) + CFEString("_") + CFEString(r),0));
					}
			}

			CFEFont* poFont = new CFEFont;
			poFont->Init(_hMat,poCharTable,poKT);
			return (poFont);
		}
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

    // Try loading a FON file first.
    poFont = poLoadFONFile(_sFilename + CFEString(".fon"),hFontTex);
    if (poFont != NULL) return(poFont);

    // Try loading a FNT file then.
    poFont = poLoadFNTFile(_sFilename + CFEString(".fnt"),hFontTex);
    if (poFont != NULL) return(poFont);

    // Try to build a basic font
    return( poBuildBasicFont(hFontTex) );
}
// ----------------------------------------------------------------------------

