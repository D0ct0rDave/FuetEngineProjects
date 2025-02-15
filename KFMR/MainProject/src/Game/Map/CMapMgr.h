//-----------------------------------------------------------------------------
/*! \class CMapMgr
 *  \brief KFRM Map Loader & Manager
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CMapMgrH
#define CMapMgrH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CEntity;
//-----------------------------------------------------------------------------
class CMapMgr
{
	public:

		/// Initializes the map manager with the given map.
		static void Init(const CFEString& _sMapName);

		/// Finalizes
		static void Finish();
		
		/// Performs an updates over the map manager.
		static void Update(FEReal _rDeltaT);
        
        /// Reset all the map entities.
		static void Reset();
    
    protected:

        static void Load(const CFEString& _sMapName);

        static CFEArray<CEntity*> m_poMapEntities;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
