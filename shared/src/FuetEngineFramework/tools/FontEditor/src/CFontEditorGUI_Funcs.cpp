//---------------------------------------------------------------------------
#include "CFontEditorGUI_Funcs.h"
#include "CAppGlobals.h"
#include "CFontEditorGUI.h"
#include "edutils.h"

#include <vector>
#include <wx/fontenum.h>
#include <wx/rawbmp.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/xml/xml.h>
#include <tinyxml.h>
//---------------------------------------------------------------------------
wxString sGetCharSet(char* _szCharSet)
{
	return( wxString::FromUTF8(_szCharSet) );

	/*
	wxString sRes = "";
	char* szBuffer = _szCharSet;
	while (*szBuffer != 0)
	{
		if (*szBuffer == '\\')
		{
			szBuffer++;
			if (*szBuffer == 'x')
			{
				szBuffer++;
				char ucChar[5];
				ucChar[0] = szBuffer[0];
				ucChar[1] = szBuffer[1];
				ucChar[2] = 0;
				
				unsigned int c;
				sscanf(ucChar,"%2x",&c);
				sRes += wxT(c);

				szBuffer+=2;
			}
			else
			{
				sRes += wxT(*szBuffer);
				szBuffer++;
			}
		}
		else
		{
			sRes += wxT(*szBuffer);
			szBuffer++;
		}
	}

	return(sRes);
	*/
}

//---------------------------------------------------------------------------
const char* szGetCharSet(const wxString& _sCharSet)
{
	static wxCharBuffer bu;
	bu = _sCharSet.ToUTF8();
	return(bu);

		/*
		static char* szBuffer = NULL;
	
		if (szBuffer != NULL)
			free(szBuffer);
	
		szBuffer = (char*)malloc(_sCharSet.size()*4 + 1);
		szBuffer[0] = 0;
	
		int j=0;
		for (int i=0;i<_sCharSet.size();i++)
		{
			wxString sChar = _sCharSet.SubString(i,i);
			const char* szChar = sChar.ToAscii();
			unsigned char cChar = *szChar;
	
			char szStr[8];
			sprintf(szStr,"\\x%2x",cChar);
			strcat(szBuffer,szStr);
		}
	
		return(szBuffer);
		*/
}
//---------------------------------------------------------------------------
void errorLog(char* str)
{
	printf("ERROR %s\n",str);
	exit(1);
}

//---------------------------------------------------------------------------
bool bLoadIniFile(const wxString& _sFilename)
{
	TiXmlDocument oDoc;
	if (oDoc.LoadFile(_sFilename.char_str()) == false)
	{
	    errorLog("FileNotFound");
		return(false);
	}

    TiXmlNode* poNode = oDoc.FirstChild();
	if ( !( poNode = oDoc.FirstChild("Props") ))
	{
		errorLog("First tag not found");
		return(false);
	}
	
	wxString sFontFace = wxT("Arial");
	
	wxString sVarValue;
	long lFontStyle = 0;
	long lFontSize = 10;
	long lConstraints = 0;
	long lAntialias = 1;
	long lBorderPixels = 0;
	long lForcedTracking = 0;
	long lPointScale = 1;
	long lYOffset = 0;
	long lToRGB = 0;
	long lPhotoshopDims = 0;
	long lUTF8 = 0;
	long lStrokeSize = 0;
	long lStrokeColor = 0xffffffff;
	long lTypefaceColor = 0xffffffff;

	wxString sCharsSet = wxT("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	TiXmlAttribute* poAttr = ((TiXmlElement*)poNode)->FirstAttribute();
	while (poAttr != NULL)
	{
		const char* szValue = poAttr->Value();
		sVarValue = wxString::FromUTF8(szValue);

		if (! stricmp("FontFace",poAttr->Name()))
		{
			// get font face
			sFontFace = sVarValue;
		}
   else if (! stricmp("FontStyle",poAttr->Name()))
        {
			sVarValue.ToLong(&lFontStyle);
        }   
   else if (! stricmp("FontSize",poAttr->Name()))
        {
			sVarValue.ToLong(&lFontSize);
        }   
   else if (! stricmp("Antialias",poAttr->Name()))
        {
			sVarValue.ToLong(&lAntialias);
        }   
   else if (! stricmp("TypefaceColor",poAttr->Name()))
        {
			sVarValue.ToLong(&lTypefaceColor);
        }
   else if (! stricmp("StrokeSize",poAttr->Name()))
        {
			sVarValue.ToLong(&lStrokeSize);
        }   
   else if (! stricmp("StrokeColor",poAttr->Name()))
        {
			sVarValue.ToLong(&lStrokeColor);
        }
   else if (! stricmp("Constraints",poAttr->Name()))
        {
			sVarValue.ToLong(&lConstraints);
        }   
   else if (! stricmp("BorderPixels",poAttr->Name()))
        {
			sVarValue.ToLong(&lBorderPixels);
        }   
   else if (! stricmp("ForcedTracking",poAttr->Name()))
        {
			sVarValue.ToLong(&lForcedTracking);
        }   
   else if (! stricmp("PointScale",poAttr->Name()))
        {
			sVarValue.ToLong(&lPointScale);
        }
   else if (! stricmp("YOffset",poAttr->Name()))
        {
			sVarValue.ToLong(&lYOffset);
        }             
   else if (! stricmp("ToRGB",poAttr->Name()))
        {
			sVarValue.ToLong(&lToRGB);
        }   
   else if (! stricmp("PhotoshopDims",poAttr->Name()))
        {
			sVarValue.ToLong(&lPhotoshopDims);
        }
   else if (! stricmp("UTF8",poAttr->Name()))
        {
			sVarValue.ToLong(&lUTF8);
        }   
   else if (! stricmp("CharSet",poAttr->Name()))
        {
			sCharsSet = sVarValue;
        }

        poAttr = poAttr->Next();
	}

	globals.m_oSpecialChars.clear();
	if ( poNode = oDoc.FirstChild("SpecialChars"))
	{
		int iNumSCs;
		((TiXmlElement*)poNode)->QueryIntAttribute("Num",&iNumSCs);

		for (int i=0;i<iNumSCs;i++)
		{
			char szStr[16];
			sprintf(szStr,"SC%d",i);

			TiXmlNode* poSpecialCharNode = poNode->FirstChild(szStr);
			
			CSpecialChar oSC;		
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("Left",	(int*)&oSC.m_uiLeft);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("Right",	(int*)&oSC.m_uiRight);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("Top",	(int*)&oSC.m_uiTop);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("Bottom",	(int*)&oSC.m_uiBottom);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("ID",		(int*)&oSC.m_cID);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("KCEq",	(int*)&oSC.m_cKCEq);
			((TiXmlElement*)poSpecialCharNode)->QueryIntAttribute("Color",	(int*)&oSC.m_uiColor);
			
			globals.m_oSpecialChars.push_back(oSC);
		}
	}
	
	if (globals.GUI->m_gdSpecialChars->GetRows()>0)
		globals.GUI->m_gdSpecialChars->DeleteRows(0,globals.GUI->m_gdSpecialChars->GetRows());

	globals.GUI->m_gdSpecialChars->AppendRows(globals.m_oSpecialChars.size(),false);

	/*
    wxXmlDocument doc;
    if (!doc.Load(_sFilename))
        return;

    wxXmlNode *child = doc.GetRoot();
	wxString sVarValue;

    // fontname
    sVarValue = child->GetPropVal(wxT("FontFace"), wxT("Arial"));
    wxString sFontFace = sVarValue;

    // font style
    sVarValue = child->GetPropVal(wxT("FontStyle"), wxT("0"));
    long lFontStyle = 0;
    sVarValue.ToLong(&lFontStyle);

    // font size
    sVarValue = child->GetPropVal(wxT("FontSize"), wxT("10"));
    long lFontSize = 0;
    sVarValue.ToLong(&lFontSize);

    // constraints
    sVarValue = child->GetPropVal(wxT("Constraints"), wxT("0"));
    long lConstraints = 0;
    sVarValue.ToLong(&lConstraints);

    // border pixels
    sVarValue = child->GetPropVal(wxT("BorderPixels"), wxT("0"));
    long lBorderPixels = 0;
    sVarValue.ToLong(&lBorderPixels);

    // Forced tracking
    sVarValue = child->GetPropVal(wxT("ForcedTracking"), wxT("0"));
    long lForcedTracking = 0;
    sVarValue.ToLong(&lForcedTracking);

    // Point scale
    sVarValue = child->GetPropVal(wxT("PointScale"), wxT("1"));
    long lPointScale = 0;
    sVarValue.ToLong(&lPointScale);

    // to rgb
    sVarValue= child->GetPropVal(wxT("ToRGB"), wxT("0"));
    long lToRGB = 0;
    sVarValue.ToLong(&lToRGB);

    // to rgb
    sVarValue= child->GetPropVal(wxT("PhotoshopDims"), wxT("0"));
    long lPhotoshopDims = 0;
    sVarValue.ToLong(&lPhotoshopDims);

    // char set
    wxString sCharsSet = child->GetPropVal(wxT("CharSet"), wxT("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	*/

    // Set font face and size
    SetFontSize(lFontSize);

	// Set font size label
	wxString s;
	globals.GUI->m_lbFontSize->SetLabel( s.Format(_T("%d"),lFontSize) );

	// Set font style
    globals.GUI->m_chFontStyle->Select(lFontStyle);

    // Set antialias
    globals.GUI->m_chAntialiasType->Select(lAntialias);

    // Set the typeface color
    globals.GUI->m_oTypefaceColor->SetColour(UICOLOR2WXCOLOR(lTypefaceColor));

    // Set stroke size
    SetStrokeSize(lStrokeSize);

    // Set the stroke color
    globals.GUI->m_oStrokeColor->SetColour(UICOLOR2WXCOLOR(lStrokeColor));

    // Set constraints
    globals.GUI->ch_Constraints->Select(lConstraints);

	// Set border pixels
	globals.GUI->m_sldBorderPixels->SetValue(lBorderPixels);

	// Set forced tracking
	globals.GUI->m_sldForcedTracking->SetValue(lForcedTracking);

	// Set point scale
	globals.GUI->m_sldPointScale->SetValue(lPointScale);

	// Set Y Offset
	globals.GUI->m_sldYOffset->SetValue(lYOffset);

    // Set UTF8
    globals.GUI->m_cbUTF8->SetValue(lUTF8);

	//
    wxFont oFont = globals.GUI->mem_Chars->GetFont();    
    oFont.SetFaceName(sFontFace);
    globals.GUI->mem_Chars->SetFont(oFont);
    SetFontStyle(lFontStyle);

    // Set char set
    globals.GUI->mem_Chars->SetValue(sCharsSet);

    globals.m_uiCurFont = 0;
    for (unsigned int i=0;i<globals.m_sFontList.size();i++)
    {
		if (sFontFace == globals.m_sFontList[i])
		{
			globals.m_uiCurFont = i;
			break;
		}
    }
	
	ViewFontRow(globals.m_uiCurFont);
	globals.GUI->Refresh();
	
	return(true);
}
//---------------------------------------------------------------------------
bool bSaveIniFile(const wxString& _sFilename)
{
	wxString sVarValue;
    wxFont oFont = globals.GUI->mem_Chars->GetFont();

	// Make xml: <?xml ..><Hello>World</Hello>
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	TiXmlElement* element = new TiXmlElement( "Props" );
	
	element->SetAttribute("FontFace",		oFont.GetFaceName().char_str());
	element->SetAttribute("FontSize",		globals.m_uiFontSize);
	element->SetAttribute("Constraints",	globals.GUI->ch_Constraints->GetSelection());
	element->SetAttribute("Antialias",		globals.GUI->m_chAntialiasType->GetSelection());
	element->SetAttribute("FontStyle",		globals.GUI->m_chFontStyle->GetSelection());
	element->SetAttribute("TypefaceColor",	WXCOLOR2UICOLOR(globals.GUI->m_oTypefaceColor->GetColour()) );
	element->SetAttribute("StrokeSize",		globals.m_uiStrokeSize);
	element->SetAttribute("StrokeColor",	WXCOLOR2UICOLOR(globals.GUI->m_oStrokeColor->GetColour()) );
	element->SetAttribute("BorderPixels",	globals.GUI->m_sldBorderPixels->GetValue());
	element->SetAttribute("ForcedTracking",	globals.GUI->m_sldForcedTracking->GetValue());
	element->SetAttribute("PointScale",		globals.GUI->m_sldPointScale->GetValue());
	element->SetAttribute("YOffset",		globals.GUI->m_sldYOffset->GetValue());
	element->SetAttribute("UTF8",			globals.GUI->m_cbUTF8->GetValue());
	element->SetAttribute("CharSet",		szGetCharSet(globals.GUI->mem_Chars->GetValue()));
	doc.LinkEndChild( decl );
	doc.LinkEndChild( element );

	element = new TiXmlElement( "SpecialChars" );
	element->SetAttribute("Num",globals.m_oSpecialChars.size());

		for (int i=0;i<globals.m_oSpecialChars.size();i++)
		{
			char szStr[16];
			sprintf(szStr,"SC%d",i);

			TiXmlElement* poSpecialCharNode = new TiXmlElement( szStr );

			poSpecialCharNode->SetAttribute("Left", globals.m_oSpecialChars[i].m_uiLeft );
			poSpecialCharNode->SetAttribute("Right", globals.m_oSpecialChars[i].m_uiRight);
			poSpecialCharNode->SetAttribute("Top", globals.m_oSpecialChars[i].m_uiTop );
			poSpecialCharNode->SetAttribute("Bottom", globals.m_oSpecialChars[i].m_uiBottom );
			poSpecialCharNode->SetAttribute("ID", globals.m_oSpecialChars[i].m_cID );
			poSpecialCharNode->SetAttribute("KCEq", globals.m_oSpecialChars[i].m_cKCEq);
			poSpecialCharNode->SetAttribute("Color", globals.m_oSpecialChars[i].m_uiColor );

			element->LinkEndChild(poSpecialCharNode);
		}

	doc.LinkEndChild( element );
	return( doc.SaveFile( _sFilename.char_str() ) );

	/*
	std::vector<wxXmlProperty*> oProperties;

    // fontname
    oProperties.push_back( new wxXmlProperty(wxT("FontFace"), oFont.GetFaceName()) );

    // font size
    wxString sFontSize = wxString::Format(wxT("%d"),oFont.GetPointSize() );
    oProperties.push_back( new wxXmlProperty(wxT("FontSize"), sFontSize ));

    // constraints
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->ch_Constraints->GetSelection() );
    oProperties.push_back( new wxXmlProperty(wxT("Constraints"), sVarValue));

    // Font Style
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_chFontStyle->GetSelection() );
    oProperties.push_back( new wxXmlProperty(wxT("FontStyle"), sVarValue));

    // border pixels
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_sldBorderPixels->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("BorderPixels"), sVarValue));

    // forced tracking
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_sldForcedTracking->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("ForcedTracking"), sVarValue));

    // point scale
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_sldPointScale->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("PointScale"), sVarValue));
    
    // Y Offset
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_sldYOffset->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("YOffset"), sVarValue));

    // to rgb
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_cbToRGB->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("ToRGB"), sVarValue));

    // Photoshop Dims
    sVarValue = wxString::Format(wxT("%d"),globals.GUI->m_cbPhotoshopDims->GetValue() );
    oProperties.push_back( new wxXmlProperty(wxT("PhotoshopDims"), sVarValue));

    // char set
    oProperties.push_back( new wxXmlProperty(wxT("CharSet"), globals.GUI->mem_Chars->GetValue()) );

    // chain props
    for (int i=1;i<oProperties.size();i++)
		oProperties[i-1]->SetNext( oProperties[i] );

    // set root node for document
    wxXmlNode* poNode = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("Props"),wxEmptyString, oProperties[0] );

    wxXmlDocument* poDoc = new wxXmlDocument;
    poDoc->SetRoot(poNode);
    if (! poDoc->Save(_sFilename))
    {
        delete poDoc;
        return;
    }

    delete poDoc;
    */
    
}
//---------------------------------------------------------------------------
/*
 * Mnuquit1004Click0
 */
///-----------------------------------------------------------------
void CFontListElemRenderer::Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected)
{
    wxString sFontFace = globals.m_sFontList[row];
    wxRect oCell = rect;
    wxBrush oBrush;
    wxPen oPen;

	if (row == globals.m_uiCurFont)
	{
		oPen.SetColour( globals.m_oColorScheme.m_oSelCellBack );
		oBrush.SetColour( globals.m_oColorScheme.m_oSelCellBack );

		dc.SetTextForeground( globals.m_oColorScheme.m_oSelCellText );

		dc.SetBrush(oBrush);
		dc.SetPen(oPen);

		dc.DrawRectangle(rect);
	}
	else
	{
		if (row % 2)
		{
			oPen.SetColour( globals.m_oColorScheme.m_oGenObjBack1);
			oBrush.SetColour( globals.m_oColorScheme.m_oGenObjBack1);
		}
		else
		{
			oPen.SetColour( globals.m_oColorScheme.m_oGenObjBack0);
			oBrush.SetColour( globals.m_oColorScheme.m_oGenObjBack0);
			
		};


		dc.SetTextForeground( globals.m_oColorScheme.m_oGridCellText );

		dc.SetBrush(oBrush);
		dc.SetPen(oPen);

		dc.DrawRectangle(rect);
	}

	// isSelected


    //oCell.GetLeft() + 40, oCell.GetBottom()-16,
    switch (col)
    {
		// first font face
        case 0:
        {
			wxFont oFont;
			oFont.SetFaceName(_T("Arial"));
			oFont.SetPointSize(12);
			dc.SetFont(oFont);
			wxSize uiLen;
			wxString sFontFaceName = sFontFace;
			wxSize strSize;

			do{
				strSize = dc.GetTextExtent(sFontFaceName);

				if (strSize.x > 150)
				{
					sFontFace = sFontFace.SubString(0,sFontFace.Len()-2);
					sFontFaceName = sFontFace + _T("...");
				}

			}while (strSize.x > 150);

			dc.DrawText( sFontFaceName,oCell.GetLeft(),oCell.GetTop());
        }
        break;

		// numbers and letters
        default:
        {
			wxString sFontFace = globals.m_sFontList[row];
			wxFont oFont;
			oFont.SetFaceName(sFontFace);
			oFont.SetPointSize(16);
			dc.SetFont(oFont);

			if (col == 1)
				dc.DrawText( wxT("0123456789"),oCell.GetLeft(),oCell.GetTop());
			else
				dc.DrawText( wxT("This is a sample text (!?)."),oCell.GetLeft(),oCell.GetTop());
        }
        break;
    }
}
//---------------------------------------------------------------------------
/*
 * Mnuquit1004Click0
 */
///-----------------------------------------------------------------
void CSpecialCharElemRenderer::Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected)
{
	if (row>=globals.m_oSpecialChars.size())
	{
		return;
	}

	CSpecialChar* poSC = &globals.m_oSpecialChars[row];
    wxRect oCell = rect;
    wxBrush oBrush;
    wxPen oPen;

	if (row == globals.m_uiCurSC)
	{
		oPen.SetColour( globals.m_oColorScheme.m_oSelCellBack );
		oBrush.SetColour( globals.m_oColorScheme.m_oSelCellBack );

		dc.SetTextForeground( globals.m_oColorScheme.m_oSelCellText );

		dc.SetBrush(oBrush);
		dc.SetPen(oPen);

		dc.DrawRectangle(rect);
	}
	else
	{
		if (row % 2)
		{
			oPen.SetColour( globals.m_oColorScheme.m_oGenObjBack1);
			oBrush.SetColour( globals.m_oColorScheme.m_oGenObjBack1);
		}
		else
		{
			oPen.SetColour( globals.m_oColorScheme.m_oGenObjBack0);
			oBrush.SetColour( globals.m_oColorScheme.m_oGenObjBack0);
		};

		dc.SetTextForeground( globals.m_oColorScheme.m_oGridCellText );

		dc.SetBrush(oBrush);
		dc.SetPen(oPen);

		dc.DrawRectangle(rect);
	}

	// isSelected
    //oCell.GetLeft() + 40, oCell.GetBottom()-16,
    switch (col)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        {
			wxFont oFont;
			oFont.SetFaceName(_T("Arial"));
			oFont.SetPointSize(12);
			dc.SetFont(oFont);
			wxSize uiLen;		
			wxSize strSize;
			
			wxString sStr;

			switch (col)
			{
				case 0: sStr = wxString::Format(wxT("%c"),poSC->m_cID);		break;
				case 1: sStr = wxString::Format(wxT("%d"),poSC->m_uiLeft);	break;
				case 2: sStr = wxString::Format(wxT("%d"),poSC->m_uiRight);	break;
				case 3: sStr = wxString::Format(wxT("%d"),poSC->m_uiTop);	break;
				case 4: sStr = wxString::Format(wxT("%d"),poSC->m_uiBottom); break;
				case 5: sStr = wxString::Format(wxT("%c"),poSC->m_cKCEq);	break;
			};

			dc.DrawText( sStr,oCell.GetLeft(),oCell.GetTop());
        }
        break;

        // color class
        case 6:
        {
			wxColor oColor(0xff000000 | poSC->m_uiColor);
			oPen.SetColour( oColor);
			oBrush.SetColour( oColor);

			dc.SetTextForeground( globals.m_oColorScheme.m_oGridCellText );

			dc.SetBrush(oBrush);
			dc.SetPen(oPen);

			dc.DrawRectangle(rect);


			wxFont oFont;
			oFont.SetFaceName(wxT("Arial"));
			oFont.SetPointSize(12);
			dc.SetFont(oFont);
			wxSize uiLen;		
			wxSize strSize;
        }
        break;

        default:
        {
        }
        break;
    }
}
//---------------------------------------------------------------------------
void SetFontSize(int _iFontSize)
{
	globals.m_uiFontSize = _iFontSize;

    wxFont oFont = globals.GUI->mem_Chars->GetFont();
    oFont.SetPointSize(_iFontSize*75/100);
	globals.GUI->mem_Chars->SetFont(oFont);

	// Set font size label
	wxString s;
	globals.GUI->m_lbFontSize->SetLabel( s.Format(_T("%d"),_iFontSize) );
}
//---------------------------------------------------------------------------
void SetStrokeSize(int _iStrokeSize)
{
	globals.m_uiStrokeSize = _iStrokeSize;

	// Set stroke size label
	wxString s;
	globals.GUI->m_lbStrokeSize->SetLabel( s.Format(_T("%d"),_iStrokeSize) );
}
//---------------------------------------------------------------------------
void SetFontStyle(int _iFontStyle)
{
    wxFont oFont = globals.GUI->mem_Chars->GetFont();

   	switch (_iFontStyle)
	{
		case 0:
		{
			oFont.SetStyle(wxFONTSTYLE_NORMAL);
			oFont.SetWeight(wxFONTWEIGHT_NORMAL);
		}
		break;
		
		// bold
		case 1:
		{
			oFont.SetStyle(wxFONTSTYLE_NORMAL);
			oFont.SetWeight(wxFONTWEIGHT_BOLD);			
		}
		break;

		case 2:
		{
			oFont.SetStyle(wxFONTSTYLE_ITALIC);
			oFont.SetWeight(wxFONTWEIGHT_NORMAL);
		}
		break;

		case 3:
		{
			oFont.SetStyle(wxFONTSTYLE_ITALIC);
			oFont.SetWeight(wxFONTWEIGHT_BOLD);
		}
		break;
	}

	globals.GUI->mem_Chars->SetFont(oFont);
}
//---------------------------------------------------------------------------
void ViewFontRow(unsigned int _uiRow)
{
	// set cursor at pos.
	globals.GUI->m_gdFontList->MakeCellVisible(_uiRow,0);
	globals.GUI->m_gdFontList->Refresh();
	/*
	unsigned int i=0;
	globals.GUI->m_gdFontList->GetRowBottom(
	while (! globals.GUI->m_gdFontList->IsVisible(_uiRow,0))
	{
		globals.GUI->m_gdFontList->Scroll(0,i);
		i++;
	}

	globals.GUI->m_gdFontList->Refresh();
	*/
}
//---------------------------------------------------------------------------
