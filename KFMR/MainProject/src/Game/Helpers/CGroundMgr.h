//-----------------------------------------------------------------------------
/*! \class CGroundMgr
 *  \brief Helper class to retrieve the ground level at a certain map place.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGroundMgrH
#define CGroundMgrH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CGroundMgr
{
	public:

		/// 
		static void Init(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);

		/// 
		static void Finish();

		/// Retrieves the ground height of the given position.
		static FEReal rGetHeight(FEReal _rPos);
		
		/// Retrieves the ground type of the given position.
		static uint uiGetGroundType(FEReal _rPos);

    protected:
        
        typedef struct TGroundSegment
        {
            CFESegment  m_oSeg;
            uint        m_uiType;
        };

        static CFEArray<TGroundSegment> m_oSegLst;
        static TGroundSegment* m_poLastSegment;
        static uint            m_uiLastSegment;

        /// Retrieves the segment belonging to the given position.
        static TGroundSegment* poGetSegment(FEReal _rPos);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
