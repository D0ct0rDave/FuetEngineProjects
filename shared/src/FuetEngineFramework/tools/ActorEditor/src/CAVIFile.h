//---------------------------------------------------------------------------
#ifndef CAVIFileH
#define CAVIFileH
//---------------------------------------------------------------------------
class CAVIFile
{
	public:
		static bool bOpen(const char* _szFilename);
		static bool Close();

		static void GotoFrame(unsigned int _uiFrame);
		static void GotoTime(float _fTime);

		static float fLength();
		static unsigned int uiNumFrames();

		static unsigned int uiFrame();
		static float fTime();

		static float fFPS();

		static void* pGetFrame();
		static unsigned int uiGetWidth();
		static unsigned int uiGetHeight();

	protected:
			
		static void FlipImage();
		static void SwapRBChannels();

		static float			m_fFPS;

		static unsigned int 	m_uiNumFrames;
		static float			m_fLength;

		static unsigned int 	m_uiCurFrame;
		static float			m_fTime;
		static void* 			m_pImgData;
		static unsigned int		m_uiWidth;
		static unsigned int		m_uiHeight;
};
//---------------------------------------------------------------------------
inline unsigned int CAVIFile::uiNumFrames()
{
	return (m_uiNumFrames);
}
//---------------------------------------------------------------------------
inline float CAVIFile::fLength()
{
	return(m_fLength);
}
//---------------------------------------------------------------------------
inline float CAVIFile::fTime()
{
	return(m_fTime);
}
//---------------------------------------------------------------------------
inline unsigned int CAVIFile::uiFrame()
{
	return (m_uiCurFrame);
}
//---------------------------------------------------------------------------
inline void CAVIFile::GotoTime(float _fTime)
{
	GotoFrame((int)(_fTime*m_fFPS));
}
//---------------------------------------------------------------------------
inline void* CAVIFile::pGetFrame()
{
	return(m_pImgData);
}

inline unsigned int CAVIFile::uiGetWidth()
{
	return(m_uiWidth);
}

inline unsigned int CAVIFile::uiGetHeight()
{
	return(m_uiHeight);
}
//---------------------------------------------------------------------------
inline float CAVIFile::fFPS()
{
	return(m_fFPS);
}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
