///-----------------------------------------------------------------
///
/// @file      CHUDLayerEditor.h
/// @author    pepe
/// Created:   24/04/2010 0:39:19
/// @section   DESCRIPTION
///            CHUDLayerEditor class declaration
///
///------------------------------------------------------------------
#ifndef CActorEditorGUI_TreeBuilderH
#define CActorEditorGUI_TreeBuilderH
///------------------------------------------------------------------
#include "FuetEngine.h"
#include <wx/TreeCtrl.h>
#include "wx/treelistctrl.h"
//---------------------------------------------------------------------------
class CBoneSkelNode : public wxTreeItemData
{
	public:

		CBoneSkelNode(CFESkelAnimNode* _poParentRoot, CFESkelAnimNode* _poParent,CFESkelAnimNode* _poObj) : 
			m_poRootParent(_poParentRoot), 
			m_poParent(_poParent),
			m_poObj(_poObj)
		{
		}; 

	
	public:
	    
		EFESkelAnimNodeType eGetType()
		{
			return( CFESkelAnimNodeTypeGetter::eGetType(m_poObj) );
		}
		CFEString sGetExternalFile()
		{
			if (eGetType() == SANT_SPRITE)
			{
				return( sGetNodeWithSpriteExternalFile((CFESkelAnimSprite*)m_poObj) );
			}
		else if (eGetType() == SANT_MESH)
			{
				return( sGetNodeWithSpriteExternalFile((CFESkelAnimMesh*)m_poObj) );
			}
		else
			 return("");
		}

	public:	

		CFESkelAnimNode*	    m_poRootParent;
		CFESkelAnimNode*	    m_poParent;
		CFESkelAnimNode*	    m_poObj;
	
	protected:
		
		template <class T>
		CFEString sGetNodeWithSpriteExternalFile(T* _poNode)
		{
			return ( CFESpriteMgr::I()->sGetResourceName( CFESpriteInstMgr::I()->poGetSprite( _poNode->hGetSprite()) ) );
		}
};
//-----------------------------------------------------------------
typedef enum ETreeElementIcon{

		IMG_UNKNOWN = -1,
		
		IMG_GROUP = 0,
		IMG_SPRITE,
		IMG_BONE,
		IMG_MESH,

		IMG_VISIBLE,
		IMG_INVISIBLE,

		IMG_LOCKED,
		IMG_UNLOCKED,

} ETreeElementIcon;
///------------------------------------------------------------------
class CActorEditorGUI_TreeBuilder : public CFESkelAnimNodeVisitor
{
	public:

		static void Build(wxTreeListCtrl* _poTreeCtrl,CFESkelAnimNode* _poNode);
		static void SetTreeCtrl(wxTreeListCtrl* _poTreeCtrl);
		static void SetParentItemID(wxTreeItemId _oItemID);
		static void SetParent(CFESkelAnimNode* _poParent);
		static void SetRootParent(CFESkelAnimNode* _poElem);
		static void SetInsertPos(int _iInsertPos);

	protected:

		/// 
		virtual void Visit(CFESkelAnimNode* _poObj) ;
		/// 
		virtual void Visit(CFESkelAnimSprite* _poObj) ;
		/// 
		virtual void Visit(CFESkelAnimBone* _poObj) ;
		/// 
		virtual void Visit(CFESkelAnimMesh* _poObj) ;
		/// 
		virtual void Visit(CFESkelAnimGroup* _poObj) ;

	public:

		/// Retrieves the type of visitor (useful when needed to extend the basic class hierarchy).
		virtual const CFEString sGetType() { return("CActorEditorGUI_TreeBuilder"); }
		wxTreeItemId oCreateNode(wxTreeItemId _oParentID,CFESkelAnimNode* _poParentRoot, CFESkelAnimNode* _poParent,CFESkelAnimNode* _poObj,int _iInsertPos);	
};
///------------------------------------------------------------------
#endif
///------------------------------------------------------------------