//-----------------------------------------------------------------------------
/*! \class CEnemyExiter
 *  \brief This class makes all the living enemies exit from the scene.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEnemyExiterH
#define CEnemyExiterH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/CGameVisitor.h"
//-----------------------------------------------------------------------------
class CEnemyExiter : public CGameVisitor
{
	public:

		// Makes all the existing enemies exit from the scene.
		static void DoExit();

	public:

        /// 
        virtual void Visit(CEntity* _poEnt)
        {
            // do nothing
        }

        /// 
        virtual void Visit(CSimpleEnemy* _poEnt);

        /// 
        virtual void Visit(CThrowerEnemy* _poEnt);

        /// Fill with the rest of enemies ...
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
