//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CAppGlobalsH
#define CAppGlobalsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
class CAppGlobals
{
    public:
		CAppGlobals()
		{
			m_poScreen = &m_oRenderer;
		};

		CFERenderer		m_oRenderer;
        CFERenderer*	m_poScreen;
        CFEInput		m_oInput;
};

extern CAppGlobals* globals;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------