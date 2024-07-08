// ----------------------------------------------------------------------------
#ifndef CFrameOptimizerH
#define CFrameOptimizerH
// ----------------------------------------------------------------------------
#include <FuetEngine.h>
#include <FreeImage.h>
#include <FreeImagePlus.h>
// ----------------------------------------------------------------------------
const uint DEFAULT_MASK = 0xff000000;
// ----------------------------------------------------------------------------
class CFrame
{
    public:
        
        CFrame(){};
        CFrame(const fipImage& _oImg,unsigned int _uiMask = DEFAULT_MASK,unsigned int _uiBorderPixels = 0)
        {
            Init(_oImg,_uiMask,_uiBorderPixels);
        };
		
		CFrame(unsigned int _uiWidth,unsigned int _uiHeight,unsigned int _uiColor)
        {
            Create(_uiWidth,_uiHeight,_uiColor);
        };

        /// Initializes the frame using the source image.
        void Init(const fipImage& _oImg,unsigned int _uiMask = DEFAULT_MASK,unsigned int _uiBorderPixels = 0);

        /// Creates a frame using the given information.
        void Create(unsigned int _uiWidth,unsigned int _uiHeight,unsigned int _uiColor);

        /// Retrieves the image of this frame.
        fipImage& oGetImage()
        {
            return(m_oFrame);
        }

        /// Retrieves the Rect of this frame.
        const CFERect& oGetRect()
        {
            return(m_oRect);
        }

    protected:

        /// Retrieves the minimum rectagle that enclosing the important portion od the image.        
        CFERect oComputeMinRect(const fipImage& oImg,unsigned int _uiMask = DEFAULT_MASK);

        /// The image that containing the minimum important information from the original source.
        fipImage m_oFrame;
        
        /// The rectangle from where was extracted the frame.
        CFERect  m_oRect;
};
// ----------------------------------------------------------------------------
class CFrameSequence
{
    public:

        CFrameSequence(const CFEString& _sSequenceName)
        {
            m_sSequenceName = _sSequenceName;
        }

        void AddFrame(CFrame* _poFrame)
        {
			m_oFrames.push_back(_poFrame);
        }

        // name of the action
        CFEString         m_sSequenceName;

        // frame coordinates
        CFEArray<CFrame*>  m_oFrames;
};
// ----------------------------------------------------------------------------
class COptiFrameSequence
{
    public:

		fipImage			m_oFrameSeq;

        // name of the action
        CFEString			m_sSequenceName;

        // frame coordinates
        CFEArray<CFERect>	m_oRects;
};
// ----------------------------------------------------------------------------
class CFrameOptimizer
{
    public:

        CFrameOptimizer();
    	virtual COptiFrameSequence* poProcess(const CFrameSequence& _oFS);

	protected:
		
		/// In case is needed
		virtual bool bInsertFrame(fipImage* _poFrame,COptiFrameSequence* _poOFS);
		
		/// Reinit process
		virtual void ReinitProcess() {};
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
