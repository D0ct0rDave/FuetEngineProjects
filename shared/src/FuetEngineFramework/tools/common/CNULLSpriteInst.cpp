//---------------------------------------------------------------------------
//
// Name:        CHUDEditorApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "CNULLSpriteInst.h"
//---------------------------------------------------------------------------
#define     NULLTEX_SIZE         128
#define     NULLTEX_FRAMESIZE    8
//---------------------------------------------------------------------------
inline bool InRange(int Min,int Max,int Value)
{
    return ((Value>=Min) && (Value <= Max));
}
//---------------------------------------------------------------------------
const uint COLOR_FC = 0xff00ff00;
const uint COLOR_BC = 0xff008000;

CNULLSpriteInst::CNULLSpriteInst()
{
	// create texture
	m_uiWidth  = NULLTEX_SIZE;
	m_uiHeight = NULLTEX_SIZE;

	// Create the texture where will dump frame image data
	m_pImgData = (FEPointer)CFEMem::pAlloc(m_uiWidth*m_uiHeight*4);
	
	// Fill with pixels
	for (uint cJ=0;cJ<m_uiHeight;cJ++)
		for (uint cI=0;cI<m_uiWidth;cI++)
		{
			if (
        		CFEMath::bBetween(int(cI - NULLTEX_FRAMESIZE),int(cI + NULLTEX_FRAMESIZE),cJ) ||
                CFEMath::bBetween(int((NULLTEX_SIZE - cI) - NULLTEX_FRAMESIZE),int((NULLTEX_SIZE - cI) + NULLTEX_FRAMESIZE),cJ)
	           )
				*(uint*)(&m_pImgData[((cJ*NULLTEX_SIZE)+cI)*4]) = COLOR_FC;
			else
        		*(uint*)(&m_pImgData[((cJ*NULLTEX_SIZE)+cI)*4]) = COLOR_BC;

			/*
         	if (
            	(cI >= NULLTEX_SIZE - NULLTEX_FRAMESIZE)  ||
                (cJ >= NULLTEX_SIZE - NULLTEX_FRAMESIZE)
	           )
				*(uint*)(&m_pImgData[((cJ*NULLTEX_SIZE)+cI)*4]) = COLOR_FC;
	        else
				*(uint*)(&m_pImgData[((cJ*NULLTEX_SIZE)+cI)*4]) = COLOR_BC;
			*/
		}

	// register texture material
	CFEString sMemFile;
	sMemFile.Format("MEM://0x%x/%d/%d",m_pImgData,m_uiWidth,m_uiHeight);

	m_hTexMat = CFEMaterialMgr::I()->hLoad( sMemFile );
}
///-----------------------------------------------------------------
FEHandler CNULLSpriteInst::hGetInstance(const CFEString& _sSpriteModel)
{
	// create sprite
	CFESprite* poSpr = CFESpriteLoader::poBuildBasicSprite(m_hTexMat,_sSpriteModel);
	CFESpriteMgr::I()->Register(poSpr,_sSpriteModel);
	return( CFESpriteInstMgr::I()->hGetInstance(poSpr) );
}
///-----------------------------------------------------------------
