// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFontLoader.h"
#include "Support/Graphics/CFEMaterialMgr.h"
#include "Support/Misc/CFELogger.h"
#include "Support/File/CFEFile.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Config/CFEConfigFile.h"
#include "core/CFECore.h"

#include "CFEFont.h"
#include "CFEFontANSI.h"
#include "CFEFontUTF8.h"
// ----------------------------------------------------------------------------
/// Build a basic font around a given material.
CFEFont* CFEFontLoader::poBuildBasicFont(FEHandler _hMat)
{
    uint uiWidth,uiHeight;
    CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiWidth);
    CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiHeight);

	CFEFontANSI* poFont = new CFEFontANSI;
	poFont->Init(_hMat);

	const FEReal rStep = _1r / _16r;
    for (uint i=0;i<MAX_CHAR;i++)
	{
		int iX = (i % 16);
		int iY = (i / 16);

		CCharInfo oCI;
		oCI.m_oUV.m_oIni.x = (FEReal)(iX  ) * rStep;
		oCI.m_oUV.m_oIni.y = (FEReal)(iY  ) * rStep;
		oCI.m_oUV.m_oEnd.x = (FEReal)(iX+1) * rStep;
		oCI.m_oUV.m_oEnd.y = (FEReal)(iY+1) * rStep;
		oCI.m_usCW = uiWidth  / 16;
		oCI.m_usCH = uiHeight / 16;
	    oCI.m_sYOfs= 0;
		oCI.m_cLC = 0;
		oCI.m_cRC = 0;
		
		poFont->AddChar(i,oCI);
	}

	return(poFont);
}

// ----------------------------------------------------------------------------
CFEFont* CFEFontLoader::poLoadFNTFile(const CFEString& _sFilename, FEHandler _hMat)
{
	CFEFile oFile;
	if (oFile.bOpen(_sFilename,FOM_READ))
	{
	    uint uiMatWidth,uiMatHeight;
        CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiMatWidth);
        CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiMatHeight);

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

		CFEFontANSI* poFont = new CFEFontANSI;
		poFont->Init(_hMat);

        // Read character info from file.
		for (uint i=0;i<uiNumChars;i++)
		{
		    // Read char information.
			unsigned short usCharInfo[6];
            oFile.uiReadArray16((FEPointer)usCharInfo,6);

			CCharInfo oCI;
			unsigned short usChar = usCharInfo[0];			
			oCI.m_oUV.m_oIni.x = (FEReal)(usCharInfo[1]) / (FEReal)(usBaseTexWidth);
			oCI.m_oUV.m_oIni.y = (FEReal)(usCharInfo[2]) / (FEReal)(usBaseTexHeight);
			oCI.m_oUV.m_oEnd.x = (FEReal)(usCharInfo[1]+usCharInfo[3]) / (FEReal)(usBaseTexWidth);
			oCI.m_oUV.m_oEnd.y = (FEReal)(usCharInfo[2]+usCharInfo[4]) / (FEReal)(usBaseTexHeight);

            oCI.m_usCW  = usCharInfo[3];
            oCI.m_usCH  = usCharInfo[4];
            oCI.m_sYOfs = usCharInfo[5];

            oCI.m_cLC = 0;
            oCI.m_cRC = 0;
            
            poFont->AddChar(usChar,oCI);
		}

		// finally close the file
		oFile.Close();

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
			FEBool bFilter = oFile.bGetBool("Font.Regular.Filter",true);
			CFEMaterialMgr::I()->bSetMaterialProperty(_hMat,"DiffuseMap.Filter",(FEPointer)bFilter);

			CFEFontANSI* poFont = new CFEFontANSI;
			poFont->Init(_hMat);

			// Read number of rows.
			uint uiRows = oFile.iGetInteger("Font.Regular.NumRows",16);

			// Read number of cols.
			uint uiCols = oFile.iGetInteger("Font.Regular.NumCols",16);

			// Read character set.
			CFEString sCharSet = oFile.sGetString("Font.Regular.CharSet","");

			// Read the base texture width
			uint uiTexWidth = oFile.iGetInteger("Font.Regular.TexWidth",16);

			// Read the base texture height 
			uint uiTexHeight = oFile.iGetInteger("Font.Regular.TexHeight",16);

			// Read char width.
			uint uiCellWidth = oFile.iGetInteger("Font.Regular.CellWidth",16);

			// Read cell height.
			uint uiCellHeight = oFile.iGetInteger("Font.Regular.CellHeight",16);

			// Read char width.
			uint uiCharWidth = oFile.iGetInteger("Font.Regular.CharWidth",16);

			// Read char height.
			uint uiCharHeight = oFile.iGetInteger("Font.Regular.CharHeight",16);

			// Create the char table.
	
			// Read the forced tracking
			int iForcedTracking = oFile.iGetInteger("Font.Regular.ForcedTracking",0);
			CCharInfo      oCI  = *poFont->poGetCharInfo(0);
			oCI.m_usCW  = uiCharWidth;
			oCI.m_usCH 	= uiCharHeight;
			oCI.m_sYOfs	= iForcedTracking;
			poFont->AddChar(0,oCI);

			uint uiWidth,uiHeight;
		    CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiWidth);
			CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiHeight);

			FEReal rHalfHPix = _0r; // _05r / (FEReal)uiWidth;
			FEReal rHalfVPix = _0r; // _05r / (FEReal)uiHeight;

			FEReal rURange = (FEReal)(uiCellWidth*uiCols)  / (FEReal)uiTexWidth;
			FEReal rVRange = (FEReal)(uiCellHeight*uiRows) / (FEReal)uiTexHeight;

			FEReal rUStep = (FEReal)uiCellWidth  / (FEReal)uiTexWidth;
			FEReal rVStep = (FEReal)uiCellHeight / (FEReal)uiTexHeight;

			FEReal rUWidth  = (FEReal)uiCharWidth  / (FEReal)uiTexWidth;
			FEReal rVHeight = (FEReal)uiCharHeight / (FEReal)uiTexHeight;

				const unsigned char* szChar = (unsigned char*)sCharSet.szString();
				uint uiElem  = 0;
				while (*szChar)
				{
					uint i = *szChar;
						
						CCharInfo oCI;
						oCI.m_usCH = uiCharHeight;
						oCI.m_usCW = uiCharWidth;
						oCI.m_sYOfs= 0;

						uint iX = uiElem % uiCols;
						uint iY = uiElem / uiCols;

						oCI.m_oUV.m_oIni.x = (FEReal)(iX*rUStep) + rHalfHPix;
						oCI.m_oUV.m_oIni.y = (FEReal)(iY*rVStep) + rHalfVPix;
						oCI.m_oUV.m_oEnd.x = (FEReal)(iX*rUStep) + rUWidth  + rHalfHPix;
						oCI.m_oUV.m_oEnd.y = (FEReal)(iY*rVStep) + rVHeight + rHalfVPix;

						poFont->AddChar(i,oCI);

					uiElem++;
					szChar++;
				}

			// Read space width
			uint uiSpaceCharWidth = oFile.iGetInteger("Font.Regular.SpaceCharWidth",uiCharWidth);
			oCI  = *poFont->poGetCharInfo(' ');
			oCI.m_usCW	= uiSpaceCharWidth;
			poFont->AddChar(' ',oCI);

			return (poFont);
		}
	else if (sType |= "OpenType")
		{
			FEBool bFilter = oFile.bGetBool("Font.OpenType.Filter",true);
			CFEMaterialMgr::I()->bSetMaterialProperty(_hMat,"DiffuseMap.Filter",(FEPointer)bFilter);

			FEBool bUTF8 = oFile.bGetBool("Font.OpenType.UTF8",false);
			FEBool bANSI = !bUTF8;
			CFEFont* poFont			= NULL;
			
			if (bANSI)
				poFont = new CFEFontANSI;
			else
				poFont = new CFEFontUTF8;

			uint uiMatWidth,uiMatHeight;
			CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Width",(FEPointer)&uiMatWidth);
			CFEMaterialMgr::I()->bGetMaterialProperty(_hMat,"DiffuseMap.Height",(FEPointer)&uiMatHeight);

			uint uiBaseTexWidth,uiBaseTexHeight;
			uint uiDefCharWidth,uiDefCharHeight;
			uint uiSpaceCharWidth;


			uiBaseTexWidth  = oFile.iGetInteger("Font.OpenType.BaseWidth",0);
			uiBaseTexHeight = oFile.iGetInteger("Font.OpenType.BaseHeight",0);
			uiDefCharWidth  = oFile.iGetInteger("Font.OpenType.DefCharWidth",0);
			uiDefCharHeight = oFile.iGetInteger("Font.OpenType.DefCharHeight",0);
			uiSpaceCharWidth = oFile.iGetInteger("Font.OpenType.SpaceCharWidth",uiDefCharWidth);

			// Read the forced tracking
			CCharInfo      oCI  = *poFont->poGetCharInfo(0);
			oCI.m_usCW  = uiDefCharWidth;
			oCI.m_usCH 	= uiDefCharHeight;
			oCI.m_sYOfs	= uiDefCharWidth/6;
			poFont->AddChar(0,oCI);

			// Read the kern table
			uint iLCs = oFile.iGetInteger("Font.OpenType.KerningTable.NumLClasses",0);
			uint iRCs = oFile.iGetInteger("Font.OpenType.KerningTable.NumRClasses",0);

			CFEKernTable* poKT = NULL;
			if ((iLCs*iRCs)>0)
			{
				poKT = new CFEKernTable(iLCs,iRCs);

				for (uint r=0;r<iRCs;r++)
					for (uint l=0;l<iLCs;l++)
					{
						CFEString sSection = CFEString("Font.OpenType.KerningTable.m");
						poKT->SetKern(l, r,	oFile.iGetInteger(sSection + CFEString((int)l) + CFEString("_") + CFEString((int)r),0));
					}
			}

			// Initialize the font with the given information
			poFont->Init(_hMat,poKT);

			// Read character info from file
			unsigned int uiNumChars = oFile.iGetInteger("Font.OpenType.NumChars",0);

			for (uint i=0;i<uiNumChars;i++)
			{
				CFEString sSection = CFEString("Font.OpenType.C")+CFEString((int)i);

				// Read char information.
				uint uiID	 = oFile.iGetInteger(sSection+".ID",0);
				uint uiCharX = oFile.iGetInteger(sSection+".S",0);
				uint uiCharY = oFile.iGetInteger(sSection+".T",0);
				uint uiCharW = oFile.iGetInteger(sSection+".W",0);
				uint uiCharH = oFile.iGetInteger(sSection+".H",0);

				CCharInfo oCI;
				oCI.m_oUV.m_oIni.x = (FEReal)(uiCharX) / (FEReal)(uiBaseTexWidth);
				oCI.m_oUV.m_oIni.y = (FEReal)(uiCharY) / (FEReal)(uiBaseTexHeight);
				oCI.m_oUV.m_oEnd.x = (FEReal)(uiCharX+uiCharW) / (FEReal)(uiBaseTexWidth);
				oCI.m_oUV.m_oEnd.y = (FEReal)(uiCharY+uiCharH) / (FEReal)(uiBaseTexHeight);

				oCI.m_usCW  = uiCharW;
				oCI.m_usCH  = uiCharH;
				oCI.m_sYOfs = oFile.iGetInteger(sSection+".YOfs",0);

				oCI.m_cLC   = oFile.iGetInteger(sSection+".LClass",0);
				oCI.m_cRC   = oFile.iGetInteger(sSection+".RClass",0);

				if (uiID != 0)	// character 0 is special and should not be read
					poFont->AddChar(uiID,oCI);
			}
			
			const CCharInfo* poCI = poFont->poGetCharInfo(' ');
			if (poCI != NULL)
			{
				// modify space character
				CCharInfo oCI = *poCI;
				oCI.m_usCW	= uiSpaceCharWidth;
				poFont->AddChar(' ',oCI);
			}

			return (poFont);
		}
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
CFEFont* CFEFontLoader::poLoad(const CFEString& _sFilename)
{
    FEHandler hFontTex = CFEMaterialMgr::I()->hLoad(_sFilename);
    if (hFontTex == NULL)
    {
        CFECore::Log::Print("WARNING: Unable to open font material %s\n",_sFilename.szString());
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

