//-----------------------------------------------------------------------------
/*! \class CGameFlow
 *  \brief The class responsible of the game flow in the level.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameFlowH
#define CGameFlowH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemySpawner.h"
#include "Game/CStatedObject.h"
//-----------------------------------------------------------------------------
class CGameFlow : public CStatedObject
{
	public:

        /// Default constructor of the level game flow.
        CGameFlow() : m_poEnemySpawner(NULL) {};

		/// Destructor of the level game flow.
		virtual ~CGameFlow() {};

		/// Function to be called the first time a level is to be played.
		virtual void Start() = 0;

		/// Function to be called upon level restart.
		virtual void Restart() = 0;

		/// Resets the gameflow to its initial state.
		virtual void Reset() = 0;

		/// Performs an update step over the game flow logic.
		virtual void Update(FEReal _rDeltaT) = 0;

		/// Retrieves whether the gameflow is already done or not.
		virtual bool bFinished() = 0;

		/// Retrieves the enemy spawner associated with the current game flow.
		CEnemySpawner* poGetEnemySpawner()
		{
			return(m_poEnemySpawner);
		}

        /// If someone wants to communicate with this entity...
		virtual void ReceiveMessage(uint _uiMessage/*,CEntity* _poSrcEntity,FEPointer _pData*/) {}

        /// In case some specific gameflow needs to render some kind of information ....
		virtual void Render() {};

	protected:

        /// The enemy spawner used by this object
		CEnemySpawner*	m_poEnemySpawner;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
