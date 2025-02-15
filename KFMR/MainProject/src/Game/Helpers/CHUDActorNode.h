// -----------------------------------------------------------------------------
/*! \class CEntityMgr
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CHUDActorNodeH
#define CHUDActorNodeH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CHUDActorNode : public CFEHUDObject
{
	public:
				CHUDActorNode(const CFEString& _sName) : CFEHUDObject(_sName)
				{
				}

				virtual void Render(CFEHUDVisitor* _poVisitor);

				/// Sets an actor node of for this hud object.
				void SetActorNode(CFESkelAnimNode* _poNode)
				{
					m_poNode = _poNode;
				}

				/// Sets an actor node of for this hud object.
				CFESkelAnimNode* poGetActorNode()
				{
					return( m_poNode );
				}

                /// Perform processing over the object
        		virtual void Accept(CFEHUDVisitor* _poVisitor)
        		{
        		    Render(_poVisitor);
        		}

	private:	
				CFESkelAnimNode*    m_poNode;
				uint		        m_uiAction;
};

//-----------------------------------------------------------------------------
#endif // CHUDActionsH
//-----------------------------------------------------------------------------
