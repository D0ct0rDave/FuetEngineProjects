// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDLabel.h"
#include "core/CFECore.h"
#include "graphics/font/CFEFont.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "types/CFEArray.h"
//-----------------------------------------------------------------------------
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
//-----------------------------------------------------------------------------
CFEHUDLabel::~CFEHUDLabel()
{
	SetText("");
}
//-----------------------------------------------------------------------------
void CFEHUDLabel::SetText(const char *fmt,...)
{
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
	va_end  (argptr);

	SetText( CFEString(gsszLocalBuff) );
}
//-----------------------------------------------------------------------------
void CFEHUDLabel::AdjustText()
{
	if (m_poFont == NULL) return;
        /*
        /// Retrieves the length of the string in pixels.
        FEReal rStringLen(const CFEString& _sString,uint _uiFrom = 0,int _iLen = -1) const;
        */
      
    const char* szCurStr = sGetText().szString();
	if (szCurStr==NULL)
	{
		m_sAdjustedText=CFEString::sNULL();
		return;
	}
	
	CFEString sWord;

	// isolate words
	CFEArray<CFEString> m_sWords;
	do 
	{
		// Get palabra
		sWord = "";
		while ( 
					   (*szCurStr != 0) 
					&& (*szCurStr != ' ') 
					&& ( !((szCurStr[0] == '/') && (szCurStr[1] == 'n')) )
				)
		{
			sWord +=  *szCurStr;
			szCurStr++;
		};

		if ((szCurStr[0] == '/') && (szCurStr[1] == 'n'))
		{
			sWord += "/n";
			szCurStr+=2;
		}
		else if (*szCurStr == ' ')
		{
			szCurStr++;
		}

		m_sWords.push_back(sWord);

	}while (*szCurStr != 0);
	
	// Build new sentence with end of line (EOL) char seqs.
	m_sAdjustedText = "";
	CFEString sFrase = "";
	for (uint i=0;i<m_sWords.size();i++)
	{
		if (m_sWords[i] == "") continue;

		// Build sentence to test...
		CFEString sNewFrase;
		if (sFrase != "")
			sNewFrase = sFrase + CFEString(" ") + m_sWords[i];
		else
			sNewFrase = m_sWords[i];

		FEReal rLen = m_poFont->rStringLen(sNewFrase)*m_rPointSize + sNewFrase.uiLen()*m_rTracking*m_rPointSize;

		// this word has a newline char sequence?
		uint uiWordLen = m_sWords[i].uiLen();
		FEBool bEndOfLine = (uiWordLen>=2) && ((m_sWords[i][uiWordLen-2]=='/') && (m_sWords[i][uiWordLen-1]=='n'));

		
		if ((m_rAdjustmentWidth < rLen) || (bEndOfLine))
		{
			if (m_rAdjustmentWidth > rLen)
			{
				// we are here sure because we have an end of line char seq.
				if (bEndOfLine)
					m_sAdjustedText += sNewFrase;

				// word doest not contribute to the next sentence
				sFrase = "";
			}
			else
			{				
				// current word does not contribute to this sentence
				m_sAdjustedText += sFrase + "/n";
				
				// but if there is end of line, the current word should be added 
				// to the adjusted text (if not, it will be lost in the next iteration).
				if (bEndOfLine)
				{
					m_sAdjustedText += m_sWords[i]; // includes the EOL char seq.
					sFrase = "";
				}
				else
				{
					// The new word contributes to the next sentence.
					sFrase = m_sWords[i];
				}				
			}
		}

		// Frase pases test, save it
		else
		{
			sFrase = sNewFrase;

			/*
			if (sFrase != "")
				// una palabra válida más...
				sFrase += CFEString(" ") + m_sWords[i];
			else
				// una palabra válida más...
				sFrase = m_sWords[i];
			*/
		}
	}
	
	if (sFrase != "")
		m_sAdjustedText += sFrase;
}
//-----------------------------------------------------------------------------
