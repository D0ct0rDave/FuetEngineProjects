// ----------------------------------------------------------------------------
/*! \class CFESkelAnimUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimUtilsH
#define CFESkelAnimUtilsH
//-----------------------------------------------------------------------------
#include "FEEnums.h"
#include "CFESkelAnimNodeVisitor.h"
#include "CFESkelAnim.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNodeTypeGetter : public CFESkelAnimNodeVisitor
{
	public:
		CFESkelAnimNodeTypeGetter();

		static EFESkelAnimNodeType eGetType(CFESkelAnimNode* _poNode);

		virtual void Visit(CFESkelAnimGroup* _poNode);
		virtual void Visit(CFESkelAnimMeshModel* _poNode);
		virtual void Visit(CFESkelAnimMesh* _poNode);
		virtual void Visit(CFESkelAnimSpriteModel* _poNode);
		virtual void Visit(CFESkelAnimSprite* _poNode);
		virtual void Visit(CFESkelAnimBone* _poNode);
		virtual void Visit(CFESkelAnimNode* /*_poNode*/ ){};

	protected:

		EFESkelAnimNodeType m_eType;
};
//-----------------------------------------------------------------------------
namespace CFESkelAnimUtils
{
	/// Translate a string text representing the play mode into the proper enumerated value.
	ESAAPlayMode eGetActionPlayMode(const CFEString& _sPlayMode);
		
	/// Retrieves the animation time taken by a given action or -1 if infinite (loop/pingpong)
	FEReal rGetActionTime(CFESkelAnim* _poAnim,CFESkelAnimAction* _poAction);

	/// Retrieves the maximum animation time taken by a given action without taking into account the playmode
	FEReal rGetMaxActionTime(CFESkelAnim* _poAnim,CFESkelAnimAction* _poAction);

	/// Retrieves the animation time taken by a given node action or -1 if infinite (loop/pingpong)
	FEReal rGetNodeActionTime(CFESkelAnim* _poAnim,CFESkelAnimNodeAction* _poAction);

	/// Retrieves the maximum animation time taken by a given node action without taking into account the playmode
	FEReal rGetNodeMaxActionTime(CFESkelAnim* _poAnim,CFESkelAnimNodeAction* _poAction);
}
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
