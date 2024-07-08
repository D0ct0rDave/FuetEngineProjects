// ----------------------------------------------------------------------------
#ifndef CBSPFrameOptimizerH
#define CBSPFrameOptimizerH
// ----------------------------------------------------------------------------
#include "CFrameOptimizer.h"
// ----------------------------------------------------------------------------
class CBSPNode;
// ----------------------------------------------------------------------------
class CBSPFrameOptimizer : public CFrameOptimizer
{
    public:

        CBSPFrameOptimizer();

	protected:
		
		/// In case is needed
		virtual bool bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS);
		
		/// Reinit process
		virtual void ReinitProcess();
	
	protected:

		CBSPNode* m_poBSP;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
