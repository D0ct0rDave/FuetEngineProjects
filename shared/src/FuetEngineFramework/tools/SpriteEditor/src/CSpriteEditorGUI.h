#ifndef __CSpriteEditorGUI__
#define __CSpriteEditorGUI__

/**
@file
Subclass of CSpriteEditorGUIBase, which is generated by wxFormBuilder.
*/

#include "CSpriteEditorGUIBase.h"

//// end generated include

/** Implementing CSpriteEditorGUIBase */
class CSpriteEditorGUI : public CSpriteEditorGUIBase
{
	protected:
		// Handlers for CSpriteEditorGUIBase events.
		void CSpriteEditorGUIBaseOnClose( wxCloseEvent& event );
		void CSpriteEditorGUIBaseOnIdle( wxIdleEvent& event );
		void m_gdSpriteActionsOnGridCellLeftClick( wxGridEvent& event );
		void m_gdSpriteActionsOnGridCellLeftDClick( wxGridEvent& event );
		void m_gdSpriteActionsOnGridCellRightClick( wxGridEvent& event );
		void m_btNewActionOnButtonClick( wxCommandEvent& event );
		void m_btActionUpOnButtonClick( wxCommandEvent& event );
		void m_btActionDownOnButtonClick( wxCommandEvent& event );
		void m_btDeleteActionOnButtonClick( wxCommandEvent& event );
		void OnPlayAnimationClick( wxCommandEvent& event );
		void m_btReloadMaterialsOnButtonClick( wxCommandEvent& event );
		void m_btCopyFrameOnButtonClick( wxCommandEvent& event );
		void m_btPasteFrameOnButtonClick( wxCommandEvent& event );
		void sb_KeyFrameOnScroll( wxScrollEvent& event );
		void m_btNewFrameOnButtonClick( wxCommandEvent& event );
		void m_btDeleteFrameOnButtonClick( wxCommandEvent& event );
		void pn_DisplayOnMouseWheel( wxMouseEvent& event );
		void pn_DisplayOnSize( wxSizeEvent& event );
		void m_cbSnapFactorOnChoice( wxCommandEvent& event );
		void m_btLoadMaterialOnButtonClick( wxCommandEvent& event );
		void m_btClearMaterialOnButtonClick( wxCommandEvent& event );
		void m_cbSWrapModeOnCombobox( wxCommandEvent& event );
		void m_cbTWrapModeOnCombobox( wxCommandEvent& event );
		void m_cbFilterOnCombobox( wxCommandEvent& event );
		void m_btAffectAllFramesOnToggleButton( wxCommandEvent& event );
		void m_sldDelayBlendFactOnCommandScroll( wxScrollEvent& event );
		void m_sldFrameTimeOnCommandScroll( wxScrollEvent& event );
		void m_btAutoAdjustTopLeftOnButtonClick( wxCommandEvent& event );
		void m_btAutoAdjustCenterOnButtonClick( wxCommandEvent& event );
		void m_btAutoAdjustBottomCenterOnButtonClick( wxCommandEvent& event );
		void m_btAutoAdjustSizeOnButtonClick( wxCommandEvent& event );
		void m_btAdjustSizeManuallyOnButtonClick( wxCommandEvent& event );
		void m_cbScaleXUEquallyOnCheckBox( wxCommandEvent& event );
		void m_cbUWorldCoordsOnCheckBox( wxCommandEvent& event );
		void m_cbScaleYVEquallyOnCheckBox( wxCommandEvent& event );
		void m_cbVWorldCoordsOnCheckBox( wxCommandEvent& event );
		void m_edPivotImageXCoordOnKeyDown( wxKeyEvent& event );
		void m_edPivotImageXCoordOnSpinCtrl( wxSpinEvent& event );
		void m_edPivotImageXCoordOnSpinCtrlText( wxCommandEvent& event );
		void m_edPivotImageYCoordOnKeyDown( wxKeyEvent& event );
		void m_edPivotImageYCoordOnSpinCtrl( wxSpinEvent& event );
		void m_edPivotImageYCoordOnSpinCtrlText( wxCommandEvent& event );
		void m_btAdjustPivotVerticalTopOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotTopLeftOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotTopCenterOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotTopRightOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotVerticalCenterOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotCenterLeftOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotCenterCenterOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotCenterRightOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotVerticalBottomOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotBottomLeftOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotBottomCenterOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotBottomRightOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotHorizontalLeftOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotHorizontalCenterOnButtonClick( wxCommandEvent& event );
		void m_btAdjustPivotHorizontalRightOnButtonClick( wxCommandEvent& event );
		void menuitem0OnMenuSelection( wxCommandEvent& event );
		void menuitem1OnMenuSelection( wxCommandEvent& event );
		void menuitem11OnMenuSelection( wxCommandEvent& event );
		void menuitem3OnMenuSelection( wxCommandEvent& event );
		void mo_GenerateHScrollOnMenuSelection( wxCommandEvent& event );
		void mo_GenerateVScrollOnMenuSelection( wxCommandEvent& event );
		void mo_AboutOnMenuSelection( wxCommandEvent& event );
	public:
		/** Constructor */
		CSpriteEditorGUI( wxWindow* parent );
	//// end generated class members














};

#endif // __CSpriteEditorGUI__
