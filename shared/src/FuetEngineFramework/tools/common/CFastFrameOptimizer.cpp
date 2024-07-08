// Standard Gen Includes

#include <windows.h>
#include <math.h>
#include <direct.h>
// #include <io.h>
#include <vector>
#include <string>
#include <math.h>

#include "CFastFrameOptimizer.h"
#include <FuetEngine.h>
// ----------------------------------------------------------------------------
CFastFrameOptimizer::CFastFrameOptimizer()
{

}
// ----------------------------------------------------------------------------
bool CFastFrameOptimizer::bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS)
{
	if ((_poFrame->getWidth() == 0) || (_poFrame->getHeight() == 0)) return(true);
	
	uint uiImgWidth		= _poFrame->getWidth(); 
	uint uiImgHeight	= _poFrame->getHeight();
	uint uiFrmSeqWidth	= _poOFS->m_oFrameSeq.getWidth();
	uint uiFrmSeqHeight	= _poOFS->m_oFrameSeq.getHeight();

	// cabe verticalmente?
	if ((m_fCurY+_poFrame->getHeight()) <= _poOFS->m_oFrameSeq.getHeight())
	{
		// cabe horizontalmente también.
		if ((m_fCurX+_poFrame->getWidth()) <= _poOFS->m_oFrameSeq.getWidth())
		{
			// we've found a hole: put the frame there.
			_poOFS->m_oFrameSeq.pasteSubImage(*_poFrame,m_fCurX,m_fCurY);
			
			CFERect oSRect;
			oSRect.m_oIni.x = m_fCurX;
			oSRect.m_oIni.y = m_fCurY;
			oSRect.m_oEnd.x = m_fCurX + _poFrame->getWidth();
			oSRect.m_oEnd.y = m_fCurY + _poFrame->getHeight();

			_poOFS->m_oRects.push_back(oSRect);
			
			// Update X cursor pos.
			m_fCurX = m_fCurX+_poFrame->getWidth();

			if (m_fNextY < (m_fCurY+_poFrame->getHeight()))
				m_fNextY = (m_fCurY+_poFrame->getHeight());

			return(true);
		}
		// no cabe horizontalmente, ir al siguiente nivel
		else if (m_fCurY<m_fNextY)
		{
			m_fCurX = 0.0f;
			m_fCurY = m_fNextY;
			return(bInsertFrame(_poFrame,_poOFS));
		}
		else
		{
			// hay que ensanchar...
			return(false);
		}
	}
	else
	{
		return(false);
	}
}
// ----------------------------------------------------------------------------
void CFastFrameOptimizer::ReinitProcess()
{
	m_fCurX = 0.0f;
	m_fCurY = 0.0f;
	m_fNextY = 0.0f;
}
// ----------------------------------------------------------------------------
