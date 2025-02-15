//-----------------------------------------------------------------------------
/*! \class CThrownObject
 *  \brief The class that manages objects thrown by ThrowerEnemy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CThrownObjectH
#define CThrownObjectH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CEntity.h"
#include "Game/Entity/EntityAttacks.h"
//-----------------------------------------------------------------------------
class CThrownObject : public CEntity
{ 
	public:

		CThrownObject();
		~CThrownObject();

		/// Initializes the enemy.
		void Init();

		/// Spawns the enemy.
		void Spawn(const CFEVect2& _oPos,bool _bLeft);

		// Change the default speed
		void SetSpeed(FEReal _rNewSpeed);

		// 
		virtual void Reset();

		// 
		virtual void Update(FEReal _rDeltaT);

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData);

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

		/// Returns the type of entity
		virtual EGameEntity eGetType()
		{
			return(GE_THROWN_OBJECT);
		}

	protected:

		virtual void Kill();		
		
		FEHandler	            m_hAnimInst;				
		FEReal		            m_rDir;
		FEHandler	            m_hSnd;
		FEReal		            OBJ_SPEED;
		static TEntityAttack    m_oAttackInfo;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
