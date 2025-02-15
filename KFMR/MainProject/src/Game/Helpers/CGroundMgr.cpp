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
#include "CGroundMgr.h"
#include "Game/CGameGlobals.h"
//-----------------------------------------------------------------------------
const FEReal GROUND_HEIGHT	= 370.0f;

//-----------------------------------------------------------------------------

CFEArray<CGroundMgr::TGroundSegment>    CGroundMgr::m_oSegLst;
CGroundMgr::TGroundSegment*             CGroundMgr::m_poLastSegment = NULL;
uint                                    CGroundMgr::m_uiLastSegment = 0;

//-----------------------------------------------------------------------------
// Retrieve the height of a position inside a segment.
//-----------------------------------------------------------------------------
FEReal rGetHeightFromSegment(const CFESegment& _oSeg,FEReal _rPos)
{
    FEReal rFact = (_rPos - _oSeg.m_oIni.x) / (_oSeg.m_oEnd.x - _oSeg.m_oIni.x);
    return( CFEMath::rLerp(_oSeg.m_oIni.y,_oSeg.m_oEnd.y,rFact) );
}
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
inline CGroundMgr::TGroundSegment* CGroundMgr::poGetSegment(FEReal _rPos)
{
    // Little speedup: try first the last segment
    if (
           (m_poLastSegment != NULL)
        && (CFEMath::bBetween(  m_poLastSegment->m_oSeg.m_oIni.x,
                                m_poLastSegment->m_oSeg.m_oEnd.x,
                                _rPos))
        )
    {
        return( m_poLastSegment );
    }

    // Find the proper segment
    
	#pragma message(__FUNCTION__"/"__FILE__)
	#pragma message("#################")
	#pragma message("Please find a more optimized algorithm!!!")
	#pragma message("#################")

    // Por ejemplo recorrer en zigzag last segment +1 -1 +2 -2 +3 -3 .... para encontrar el adecuado.
    for (uint i=0;i<m_oSegLst.size();i++)
    {
        if ( CFEMath::bBetween( m_oSegLst[i].m_oSeg.m_oIni.x, 
                                m_oSegLst[i].m_oSeg.m_oEnd.x,
                                _rPos)
            )
        {
            m_uiLastSegment = i;
            m_poLastSegment = &m_oSegLst[i];
            return( m_poLastSegment );
        }
    }
    
    return(NULL);
}
//-----------------------------------------------------------------------------
void CGroundMgr::Init(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
    // ------------------------------------
    // Read Ground Segments
    // ------------------------------------
    CFEString sVar = _sPrefix + ".Ground";
    uint uiNumSegments = _oCfg.iGetInteger(sVar + ".NumSegments",0);
    for (uint i=0;i<uiNumSegments;i++)
    {
        CFEString sIVar = sVar + CFEString(".Segment") + CFEString(i);
        TGroundSegment oSeg;

        oSeg.m_oSeg.m_oIni.x = _oCfg.rGetReal(sIVar + ".IniX",_0r);
        oSeg.m_oSeg.m_oIni.y = _oCfg.rGetReal(sIVar + ".IniY",_0r);
        oSeg.m_oSeg.m_oEnd.x = _oCfg.rGetReal(sIVar + ".EndX",_0r);
        oSeg.m_oSeg.m_oEnd.y = _oCfg.rGetReal(sIVar + ".EndY",_0r);

        oSeg.m_uiType = _oCfg.iGetInteger(sIVar + ".Type",0);

        m_oSegLst.push_back(oSeg);
    }        
    
    m_poLastSegment = NULL;
}
//-----------------------------------------------------------------------------
void CGroundMgr::Finish()
{
    m_oSegLst.clear();
    m_poLastSegment = NULL;
    m_uiLastSegment = 0;
}
//-----------------------------------------------------------------------------
uint CGroundMgr::uiGetGroundType(FEReal _rPos)
{
    if (m_oSegLst.size() == 0) return(0);

    TGroundSegment* poSeg = poGetSegment(_rPos);
    if (poSeg != NULL)
        return( poSeg->m_uiType );
    else

        return(0);
}
//-----------------------------------------------------------------------------
FEReal CGroundMgr::rGetHeight(FEReal _rPos)
{
    if (m_oSegLst.size() == 0) return(GROUND_HEIGHT);

    TGroundSegment* poSeg = poGetSegment(_rPos);
    if (poSeg != NULL)
        return( rGetHeightFromSegment(poSeg->m_oSeg,_rPos) );
    else

        return(_0r);
}
//-----------------------------------------------------------------------------
