#include "CFontEditorGUI.h"

CFontEditorGUI::CFontEditorGUI( wxWindow* parent )
    :
    CFontEditorGUIBase( parent )
{
    // Initialize in the application directory
    if ( ( globals.m_sExecCmdLine != _T("") ) && ( !IsDebuggerPresent() ) ) // Windows API function call: requires Windows XP.
    {
        wxFileName sFilename(globals.m_sExecCmdLine);
        SetCurrentDirectory( sFilename.GetPath().c_str() );
    }

    globals.GUI = this;

    m_gdFontList->SetDefaultRenderer(new CFontListElemRenderer);

    // fontEnumerator.EnumerateEncodings();

    // m_gdFontList

    // m_gdFontList->SetCellHighlightColour(globals.m_oColorScheme.m_oGridLabelBase);
    // m_gdFontList->SetLabelBackgroundColour(globals.m_oColorScheme.m_oGridLabelBase);
    // m_gdFontList->DeleteCols(3,m_gdFontList->GetCols()-3);

    m_gdFontList->SetColSize(0,150);
    m_gdFontList->SetColSize(1,150);
    m_gdFontList->SetColSize(2,400);
    m_gdFontList->SetDefaultRowSize(48,true);
    m_gdFontList->SetColLabelValue( 0,_T("Font Face") );
    m_gdFontList->SetColLabelValue( 1,_T("Numbers") );
    m_gdFontList->SetColLabelValue( 2,_T("Letters") );

    // gdHUDElemLayers->SetColLabelSize(0);
    // m_gdFontList->SetRowLabelSize(0);
    // m_gdFontList->ClearGrid();

    m_gdFontList->DisableDragColSize();
    m_gdFontList->DisableDragRowSize();
    m_gdFontList->DisableCellEditControl();
    m_gdFontList->EnableEditing(false);

    m_gdSpecialChars->SetDefaultRenderer(new CSpecialCharElemRenderer);

    m_gdSpecialChars->SetDefaultRowSize(32,true);
    m_gdSpecialChars->DisableDragColSize();
    m_gdSpecialChars->DisableDragRowSize();
    m_gdSpecialChars->DisableCellEditControl();
    m_gdSpecialChars->EnableEditing(false);

    m_gdSpecialChars->SetDefaultRowSize(48,true);
    m_gdSpecialChars->DeleteRows( 0,m_gdSpecialChars->GetRows() );

    // Fill combo box with fonts.
    globals.m_sFontList.clear();
    CMyEncodingEnumerator fontEnumerator;
    fontEnumerator.EnumerateFacenames();

    // ordenar fuentes
    std::sort( globals.m_sFontList.begin(),globals.m_sFontList.end() );

    m_gdFontList->DeleteRows( 0,m_gdFontList->GetRows() );
    m_gdFontList->AppendRows(globals.m_sFontList.size(),false);

    globals.m_uiCurFont = 0;
    wxString sFontFace = globals.m_sFontList[ globals.m_uiCurFont ];
    wxFont oFont;
    oFont.SetFaceName(sFontFace);
    SetFontSize(12);

    SetTitle( APP_NAME  );

    if ( globals.m_sInputFilename != _T("") )
    {
        wxFileName sFilename(globals.m_sInputFilename);
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        bLoadIniFile(globals.m_sInputFilename);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::m_gdFontListOnGridSelectCell( wxGridEvent& event )
{
// insert your code here
    globals.m_uiCurFont = event.GetRow();

    wxFont oFont = mem_Chars->GetFont();
    wxString sFontFace = globals.m_sFontList[ globals.m_uiCurFont ];
    oFont.SetFaceName(sFontFace);
    mem_Chars->SetFont(oFont);

    m_gdFontList->Refresh();
}

void CFontEditorGUI::m_gdFontListOnKeyDown( wxKeyEvent& event )
{
    switch ( event.GetKeyCode() )
    {
        case WXK_UP:
        {
            if ( globals.m_uiCurFont > 0 )
            {
                globals.m_uiCurFont--;
                m_gdFontList->SelectRow( globals.m_uiCurFont);
                ViewFontRow(globals.m_uiCurFont);
            }
        }
        break;

        case WXK_DOWN:
        {
            if ( globals.m_uiCurFont < (globals.m_sFontList.size() - 1) )
            {
                globals.m_uiCurFont++;
                m_gdFontList->SelectRow(globals.m_uiCurFont);
                ViewFontRow(globals.m_uiCurFont);
            }
        }
        break;
    }
}

void CFontEditorGUI::m_chFontStyleOnChoice( wxCommandEvent& event )
{
    SetFontStyle( m_chFontStyle->GetSelection() );
}

void CFontEditorGUI::m_bpButton1OnButtonClick( wxCommandEvent& event )
{
// Descrease font
    SetFontSize(globals.m_uiFontSize - 1);
}

void CFontEditorGUI::m_lbFontSizeOnLeftDClick( wxMouseEvent& event )
{
// open dialog to select font size:
    bool bOk = false;
    do {
        wxString s;
        wxTextEntryDialog oInputBox(this,
                                    _T("Font Size:"),
                                    _T("Enter the font size: "),
                                    s.Format(_T("%d"),globals.m_uiFontSize),
                                    wxOK | wxCANCEL);

        if ( oInputBox.ShowModal() == wxID_OK )
        {
            int iNum = 0;
            if ( sscanf(oInputBox.GetValue().char_str(),"%d",&iNum) != 0 )
            {
                bOk = true;
                SetFontSize(iNum);
            }
            else
                bOk = wxMessageBox(   _T("Cannot read this number\n")
                                      _T("Rewrite it?"),
                                      _T("Did you wrote a correct number?"),
                                      wxYES_NO,this) == wxNO;
        }
        else
            bOk = true;
    } while ( !bOk );
}

void CFontEditorGUI::m_bpButton2OnButtonClick( wxCommandEvent& event )
{
// Increase font
    SetFontSize(globals.m_uiFontSize + 1);
}

void CFontEditorGUI::m_btLessStrokeOnButtonClick( wxCommandEvent& event )
{
// TODO: Implement m_btLessStrokeOnButtonClick
    if ( globals.m_uiStrokeSize > 0 )
        SetStrokeSize(globals.m_uiStrokeSize - 1);
}

void CFontEditorGUI::m_lbStrokeSizeOnLeftDClick( wxMouseEvent& event )
{
// TODO: Implement m_lbStrokeSizeOnLeftDClick
// open dialog to select font size:
    bool bOk = false;
    do {
        wxString s;
        wxTextEntryDialog oInputBox(this,
                                    _T("Stroke Size:"),
                                    _T("Enter the stroke size: "),
                                    s.Format(_T("%d"),globals.m_uiStrokeSize),
                                    wxOK | wxCANCEL);

        if ( oInputBox.ShowModal() == wxID_OK )
        {
            int iNum = 0;
            if ( sscanf(oInputBox.GetValue().char_str(),"%d",&iNum) != 0 )
            {
                bOk = true;
                SetStrokeSize(iNum);
            }
            else
                bOk = wxMessageBox( _T("Cannot read this number\n")
                                    _T("Rewrite it?"),
                                    _T("Did you wrote a correct number?"),
                                    wxYES_NO,this) == wxNO;
        }
        else
            bOk = true;
    } while ( !bOk );
}

void CFontEditorGUI::m_btMoreStrokeOnButtonClick( wxCommandEvent& event )
{
// TODO: Implement m_btMoreStrokeOnButtonClick
    SetStrokeSize(globals.m_uiStrokeSize + 1);
}

void CFontEditorGUI::m_gdSpecialCharsOnGridCmdCellLeftDClick( wxGridEvent& event )
{
// TODO: Implement m_gdSpecialCharsOnGridCmdCellLeftDClick
    m_btEdiSCOnButtonClick(event);
}

void CFontEditorGUI::m_gdSpecialCharsOnGridSelectCell( wxGridEvent& event )
{
// TODO: Implement m_gdSpecialCharsOnGridSelectCell
// insert your code here
    globals.m_uiCurSC = event.GetRow();
    m_gdSpecialChars->Refresh();
}

void CFontEditorGUI::m_btNewSCOnButtonClick( wxCommandEvent& event )
{
// TODO: Implement m_btNewSCOnButtonClick
// Set dialog values.
    CSpecialChar oSC;
    oSC.m_cID		= wxT('0');
    oSC.m_uiLeft	= 0;
    oSC.m_uiRight	= 0;
    oSC.m_uiTop		= 0;
    oSC.m_uiBottom	= 0;
    oSC.m_cKCEq		= wxT('0');
    oSC.m_uiColor	= 0xff0000ff;

    CSpecialCharGUI* poDlg = new CSpecialCharGUI(this,&oSC);
    if ( poDlg->ShowModal() == wxID_OK )
    {
        globals.m_oSpecialChars.push_back(oSC);

        if ( m_gdSpecialChars->GetRows() > 0 )
            m_gdSpecialChars->DeleteRows( 0,m_gdSpecialChars->GetRows() );

        m_gdSpecialChars->AppendRows(globals.m_oSpecialChars.size(),false);
    }

    delete poDlg;
    Refresh();
}

void CFontEditorGUI::m_btEdiSCOnButtonClick( wxCommandEvent& event )
{
// Set dialog values.
    CSpecialCharGUI* poDlg = new CSpecialCharGUI(this,&globals.m_oSpecialChars[globals.m_uiCurSC]);

    if ( poDlg->ShowModal() == wxID_OK )
    {

    }

    delete poDlg;
    Refresh();
}

void CFontEditorGUI::m_btDeleteSCOnButtonClick( wxCommandEvent& event )
{
// TODO: Implement m_btDeleteSCOnButtonClick
}

void CFontEditorGUI::ch_ConstraintsOnChoice( wxCommandEvent& event )
{
// TODO: Implement ch_ConstraintsOnChoice
}

void CFontEditorGUI::m_sldBorderPixelsOnCommandScroll( wxScrollEvent& event )
{
// TODO: Implement m_sldBorderPixelsOnCommandScroll
}

void CFontEditorGUI::m_sldForcedTrackingOnCommandScroll( wxScrollEvent& event )
{
// TODO: Implement m_sldForcedTrackingOnCommandScroll
}

void CFontEditorGUI::m_sldPointScaleOnCommandScroll( wxScrollEvent& event )
{
// TODO: Implement m_sldPointScaleOnCommandScroll
}

void CFontEditorGUI::m_sldYOffsetOnCommandScroll( wxScrollEvent& event )
{
// TODO: Implement m_sldYOffsetOnCommandScroll
}

void CFontEditorGUI::menuitem1OnMenuSelection( wxCommandEvent& event )
{
    wxFileName sFilename(globals.m_sInputFilename);
    wxFileDialog dlgLoadFile(this, wxT("Load font"), sFilename.GetPath(true), sFilename.GetName() + FE_FONT_FILE_EXT, wxT("*") + FE_FONT_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if ( dlgLoadFile.ShowModal() == wxID_OK )
    {
        sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        bLoadIniFile(globals.m_sInputFilename);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::menuitem2OnMenuSelection( wxCommandEvent& event )
{
// save complete font data, with fast texture compression
    wxFileName sFilename(globals.m_sInputFilename);
    wxFileDialog dlgSaveFile(this, wxT("Fast Save Font as ..."), sFilename.GetPath(true), sFilename.GetName() + FE_FONT_FILE_EXT, wxT("*") + FE_FONT_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if ( dlgSaveFile.ShowModal() == wxID_OK )
    {
        sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        if ( !bSaveIniFile( globals.m_sInputFilename ) )
        {
            ShowFileErrorMessage(globals.m_sInputFilename);
            return;
        }

        wxFont oFont = mem_Chars->GetFont();

        CFontGenerator::SetFastCompact(true);
        CFontGenerator::SetConstraintType( ch_Constraints->GetSelection() );
        CFontGenerator::SetAntialiasType( m_chAntialiasType->GetSelection() );
        CFontGenerator::SetBorderPixels( m_sldBorderPixels->GetValue() );
        CFontGenerator::SetForcedTracking( m_sldForcedTracking->GetValue() );
        CFontGenerator::SetPointScale( (float)m_sldPointScale->GetValue() / 100.0f );
        CFontGenerator::SetForcedYOffset( m_sldYOffset->GetValue() );
		CFontGenerator::SetTypefaceColor( WXCOLOR2UICOLOR( m_oTypefaceColor->GetColour() ) );
        CFontGenerator::SetStrokeSize(globals.m_uiStrokeSize);
        CFontGenerator::SetStrokeColor( WXCOLOR2UICOLOR( m_oStrokeColor->GetColour() ) );

        CFontGenerator::SetUTF8Font( m_cbUTF8->GetValue() );
        CFontGenerator::GenerateFontTexture(true);
        CFontGenerator::GenerateFontDefinition(true);
        CFontGenerator::Generate(oFont,sFullFilename,mem_Chars->GetValue(),globals.m_oSpecialChars);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::menuitem22OnMenuSelection( wxCommandEvent& event )
{
// save complete font data, with optimum texture compression (does not work always)
    wxFileName sFilename(globals.m_sInputFilename);
    wxFileDialog dlgSaveFile(this, wxT("Save Font as ..."), sFilename.GetPath(true), sFilename.GetName() + FE_FONT_FILE_EXT, wxT("*") + FE_FONT_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if ( dlgSaveFile.ShowModal() == wxID_OK )
    {
        sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        if ( !bSaveIniFile( globals.m_sInputFilename ) )
        {
            ShowFileErrorMessage(globals.m_sInputFilename);
            return;
        }

        wxFont oFont = mem_Chars->GetFont();

        CFontGenerator::SetFastCompact(false);
        CFontGenerator::SetConstraintType( ch_Constraints->GetSelection() );
        CFontGenerator::SetAntialiasType( m_chAntialiasType->GetSelection() );
        CFontGenerator::SetBorderPixels( m_sldBorderPixels->GetValue() );
        CFontGenerator::SetForcedTracking( m_sldForcedTracking->GetValue() );
        CFontGenerator::SetPointScale( (float)m_sldPointScale->GetValue() / 100.0f );
        CFontGenerator::SetForcedYOffset( m_sldYOffset->GetValue() );
		CFontGenerator::SetTypefaceColor( WXCOLOR2UICOLOR( m_oTypefaceColor->GetColour() ) );
        CFontGenerator::SetStrokeSize(globals.m_uiStrokeSize);
        CFontGenerator::SetStrokeColor( WXCOLOR2UICOLOR( m_oStrokeColor->GetColour() ) );
        CFontGenerator::SetUTF8Font( m_cbUTF8->GetValue() );
        CFontGenerator::GenerateFontTexture(true);
        CFontGenerator::GenerateFontDefinition(true);
        CFontGenerator::Generate(oFont,sFullFilename,mem_Chars->GetValue(),globals.m_oSpecialChars);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::menuitem21OnMenuSelection( wxCommandEvent& event )
{
// ---------------------------
// save only definition
// ---------------------------
    wxFileName sFilename(globals.m_sInputFilename);
    wxFileDialog dlgSaveFile(this, wxT("Save Font as ..."), sFilename.GetPath(true), sFilename.GetName() + FE_FONT_FILE_EXT, wxT("*") + FE_FONT_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if ( dlgSaveFile.ShowModal() == wxID_OK )
    {
        sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        if ( !bSaveIniFile( globals.m_sInputFilename ) )
        {
            ShowFileErrorMessage(globals.m_sInputFilename);
            return;
        }

        wxFont oFont = mem_Chars->GetFont();

        CFontGenerator::SetFastCompact(true);
        CFontGenerator::SetConstraintType( ch_Constraints->GetSelection() );
        CFontGenerator::SetAntialiasType( m_chAntialiasType->GetSelection() );
        CFontGenerator::SetBorderPixels( m_sldBorderPixels->GetValue() );
        CFontGenerator::SetForcedTracking( m_sldForcedTracking->GetValue() );
        CFontGenerator::SetPointScale( (float)m_sldPointScale->GetValue() / 100.0f );
        CFontGenerator::SetForcedYOffset( m_sldYOffset->GetValue() );
		CFontGenerator::SetTypefaceColor( WXCOLOR2UICOLOR( m_oTypefaceColor->GetColour() ) );
        CFontGenerator::SetStrokeSize(globals.m_uiStrokeSize);
        CFontGenerator::SetStrokeColor( WXCOLOR2UICOLOR( m_oStrokeColor->GetColour() ) );
        CFontGenerator::SetUTF8Font( m_cbUTF8->GetValue() );
        CFontGenerator::GenerateFontTexture(false);
        CFontGenerator::GenerateFontDefinition(true);
        CFontGenerator::Generate(oFont,sFullFilename,mem_Chars->GetValue(),globals.m_oSpecialChars);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::menuitem211OnMenuSelection( wxCommandEvent& event )
{
// ---------------------------
// save only texture
// ---------------------------
    wxFileName sFilename(globals.m_sInputFilename);
    wxFileDialog dlgSaveFile(this, wxT("Save Font as ..."), sFilename.GetPath(true), sFilename.GetName() + FE_FONT_FILE_EXT, wxT("*") + FE_FONT_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if ( dlgSaveFile.ShowModal() == wxID_OK )
    {
        sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
        wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        globals.m_sWorkingDir = sFilename.GetPath();
        globals.m_sInputFilename = sFullFilename + FE_FONT_FILE_EXT;

        if ( !bSaveIniFile( globals.m_sInputFilename ) )
        {
            ShowFileErrorMessage(globals.m_sInputFilename);
            return;
        }

        wxFont oFont = mem_Chars->GetFont();

        CFontGenerator::SetFastCompact(false);
        CFontGenerator::SetConstraintType( ch_Constraints->GetSelection() );
        CFontGenerator::SetAntialiasType( m_chAntialiasType->GetSelection() );
        CFontGenerator::SetBorderPixels( m_sldBorderPixels->GetValue() );
        CFontGenerator::SetForcedTracking( m_sldForcedTracking->GetValue() );
        CFontGenerator::SetPointScale( (float)m_sldPointScale->GetValue() / 100.0f );
        CFontGenerator::SetForcedYOffset( m_sldYOffset->GetValue() );
		CFontGenerator::SetTypefaceColor( WXCOLOR2UICOLOR( m_oTypefaceColor->GetColour() ) );
        CFontGenerator::SetStrokeSize(globals.m_uiStrokeSize);
        CFontGenerator::SetStrokeColor( WXCOLOR2UICOLOR( m_oStrokeColor->GetColour() ) );
        CFontGenerator::SetUTF8Font( m_cbUTF8->GetValue() );
        CFontGenerator::GenerateFontTexture(true);
        CFontGenerator::GenerateFontDefinition(false);
        CFontGenerator::Generate(oFont,sFullFilename,mem_Chars->GetValue(),globals.m_oSpecialChars);

        SetTitle( APP_NAME + _T(" - ") + globals.m_sInputFilename );
    }
}

void CFontEditorGUI::menuitemOnMenuSelection( wxCommandEvent& event )
{
// insert your code here
    Close();
}
