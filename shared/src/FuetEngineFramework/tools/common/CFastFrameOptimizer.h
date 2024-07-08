// ----------------------------------------------------------------------------
#ifndef CFastFrameOptimizerH
#define CFastFrameOptimizerH
// ----------------------------------------------------------------------------
#include "CFrameOptimizer.h"
// ----------------------------------------------------------------------------
class CFastFrameOptimizer : public CFrameOptimizer
{
    public:

        CFastFrameOptimizer();

	protected:

		/// In case is needed
		virtual bool bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS);
		
		/// Reinit process
		virtual void ReinitProcess();

	protected:
		
		float m_fCurX;		
		float m_fCurY;
		float m_fNextY;
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

