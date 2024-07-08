//---------------------------------------------------------------------------
//
// Name:        CClipBoard.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#ifndef CNULLSpriteInstH
#define CNULLSpriteInstH
//---------------------------------------------------------------------------
#include "FuetEngine.h"
//---------------------------------------------------------------------------
DECLARE_SINGLETON(CNULLSpriteInst)
{
	public:
		
		friend CFESingleton;
		FEHandler hGetInstance(const CFEString& _sSpriteModel);
	
	protected:

		CNULLSpriteInst();

		FEPointer	m_pImgData;
		uint		m_uiWidth;
		uint		m_uiHeight;
		FEHandler	m_hTexMat;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
