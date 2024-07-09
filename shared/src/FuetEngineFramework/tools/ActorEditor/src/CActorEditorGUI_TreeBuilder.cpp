///-----------------------------------------------------------------
///
//file      CActorEditorGUI_TreeBuilder.cpp
//author    pepe
//Created:   24/04/2010 0:39:19
//section   DESCRIPTION
//CHUDLayerEditor class implementation
///
///------------------------------------------------------------------
#include "CActorEditorGUI_TreeBuilder.h"
// #include "CHUDElement.h"
// #include "CAppGlobals.h"

static wxTreeListCtrl* m_poTreeCtrl = NULL;
static int m_iInsertPos = -1;
static wxTreeItemId     m_oParentItemId;
static CFESkelAnimNode*	m_poRootParent = NULL;
static CFESkelAnimNode*	m_poParent     = NULL;
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::SetTreeCtrl(wxTreeListCtrl* _poTreeCtrl)
{
	m_poTreeCtrl = _poTreeCtrl;
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::SetParentItemID(wxTreeItemId _oItemID)
{
	m_oParentItemId = _oItemID;
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::SetRootParent(CFESkelAnimNode* _poElem)
{
	m_poRootParent = _poElem;
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::SetParent(CFESkelAnimNode* _poParent)
{
	m_poParent = _poParent;
}
//-----------------------------------------------------------------
wxTreeItemId CActorEditorGUI_TreeBuilder::oCreateNode(wxTreeItemId _oParentID,CFESkelAnimNode* _poParentRoot, CFESkelAnimNode* _poParent,CFESkelAnimNode* _poObj,int _iInsertPos)
{
	wxTreeItemId oItem;

	if (_iInsertPos == -1)
		oItem = m_poTreeCtrl->AppendItem(_oParentID,wxString(_poObj->sGetName().szString(),wxConvUTF8));
	else
		oItem = m_poTreeCtrl->InsertItem(_oParentID,_iInsertPos,wxString(_poObj->sGetName().szString(),wxConvUTF8));

	CBoneSkelNode* poElement = new CBoneSkelNode(m_poRootParent,m_poParent,_poObj);
	
	// Setup item properties
	m_poTreeCtrl->SetItemData(oItem,poElement);

/*
			if (i&0x01)
				m_oItemColor = globals.m_oColorScheme.m_oGenObjBack1;
			else
				m_oItemColor = globals.m_oColorScheme.m_oGenObjBack0;

*/
	switch (poElement->eGetType())
	{	
		case SANT_GROUP:
		m_poTreeCtrl->SetItemImage(oItem,1,IMG_GROUP);
		// m_poTreeCtrl->SetItemBackgroundColour(oItem,globals.m_oColorScheme.m_oElemBack);
		break;

		case SANT_SPRITE:
		m_poTreeCtrl->SetItemImage(oItem,1,IMG_SPRITE);
		// m_poTreeCtrl->SetItemBackgroundColour(oItem,globals.m_oColorScheme.m_oGenObjBack0);
		break;

		case SANT_MESH:
		m_poTreeCtrl->SetItemImage(oItem,1,IMG_MESH);
		// m_poTreeCtrl->SetItemBackgroundColour(oItem,globals.m_oColorScheme.m_oGenObjBack0);
		break;

		case SANT_BONE:
		m_poTreeCtrl->SetItemImage(oItem,1,IMG_BONE);
		// m_poTreeCtrl->SetItemBackgroundColour(oItem,globals.m_oColorScheme.m_oGenObjBack0);
		break;
	}

	if (_poObj->bIsVisible())
		m_poTreeCtrl->SetItemImage(oItem,0,IMG_VISIBLE);
	else
		m_poTreeCtrl->SetItemImage(oItem,0,IMG_INVISIBLE);
	
	return(oItem);
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Build(wxTreeListCtrl* _poTreeCtrl,CFESkelAnimNode* _poNode)
{
	m_poTreeCtrl = _poTreeCtrl;
	// m_poTreeCtrl-DeleteAllItems();

	m_poTreeCtrl->DeleteRoot();
    m_iInsertPos = -1;
    wxTreeItemId rootId = m_poTreeCtrl->AddRoot(wxT("Root"));
	m_oParentItemId = m_poTreeCtrl->GetRootItem();

	if (_poNode != NULL)
	{	
		CActorEditorGUI_TreeBuilder oBuilder;
		_poNode->Accept(&oBuilder);
	}
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Visit(CFESkelAnimNode* _poNode)
{
	// unknown
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Visit(CFESkelAnimGroup* _poObj)
{
	// Create the group node.
	wxTreeItemId oID = oCreateNode(m_oParentItemId,m_poRootParent,m_poParent,_poObj,m_iInsertPos);

	// backup things
	wxTreeItemId oBackItem	  = m_oParentItemId;
	CFESkelAnimNode* poOldParent = m_poParent;

	m_oParentItemId = oID;
	m_iInsertPos	= -1;
	m_poParent		= _poObj;

		for (uint i=0;i<_poObj->uiNumChildren();i++)
			_poObj->poGetChild(i)->Accept(this);

	// restore things
	m_poParent		= poOldParent;
	m_oParentItemId = oBackItem;
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Visit(CFESkelAnimBone* _poObj)
{
	// Create the group node.
	wxTreeItemId oID = oCreateNode(m_oParentItemId,m_poRootParent,m_poParent,_poObj,m_iInsertPos);

	// backup things
	wxTreeItemId oBackItem	     = m_oParentItemId;
	CFESkelAnimNode* poOldParent = m_poParent;

	m_oParentItemId = oID;
	m_iInsertPos	= -1;
	m_poParent		= _poObj;

	if (_poObj->poGetChild(0) != NULL)
		_poObj->poGetChild(0)->Accept(this);

	// restore things
	m_poParent		= poOldParent;
	m_oParentItemId = oBackItem;
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Visit(CFESkelAnimSprite* _poObj)
{
	wxTreeItemId oID = oCreateNode(m_oParentItemId,m_poRootParent,m_poParent,_poObj,m_iInsertPos);
}
//-----------------------------------------------------------------
void CActorEditorGUI_TreeBuilder::Visit(CFESkelAnimMesh* _poObj)
{
	wxTreeItemId oID = oCreateNode(m_oParentItemId,m_poRootParent,m_poParent,_poObj,m_iInsertPos);
}
//-----------------------------------------------------------------