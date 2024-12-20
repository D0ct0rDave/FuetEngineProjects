#ifndef __CActorEditorGUIInst__
#define __CActorEditorGUIInst__

/**
@file
Subclass of CActorEditorGUI, which is generated by wxFormBuilder.
*/

#include "CActorEditorGUIBase.h"

//// end generated include

/** Implementing CActorEditorGUI */
class CActorEditorGUIInst : public CActorEditorGUI
{
	protected:
		// Handlers for CActorEditorGUI events.
		void sb_KeyFrameOnScroll( wxScrollEvent& event );
		void ChangeHorizontalSkeletionScale( wxScrollEvent& event );
		void ChangeVerticalSkeletionScale( wxScrollEvent& event );
		void ShowBonesClick( wxCommandEvent& event );
		void ReloadMaterialsClick( wxCommandEvent& event );
		void OnPlayAnimationClick( wxCommandEvent& event );
		void OnLoopAnimationClick( wxCommandEvent& event );
		void OnChangeHorizontalSkeletionScaleChangeAnimationSpeed( wxScrollEvent& event );
		void bt_LoadSpriteOnButtonClick( wxCommandEvent& event );
		void cbSpriteActionOnCombobox( wxCommandEvent& event );
		void sld_OfsOnScroll( wxScrollEvent& event );
		void cb_ShowLabelsOnCheckBox( wxCommandEvent& event );
		void cb_ShowSpritesOnCheckBox( wxCommandEvent& event );
	public:
		/** Constructor */
		CActorEditorGUIInst( wxWindow* parent );
	//// end generated class members
	
};

#endif // __CActorEditorGUIInst__
