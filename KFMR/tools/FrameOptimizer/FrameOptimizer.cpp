// Standard Gen Includes

#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include <direct.h>

#include <vector>
#include <string>
#include <math.h>

#include "CImageFileProcessor.h"
#include "CLibConfigFileWriter.h"
#include "CBinConfigFileWriter.h"
#include "CFrameOptimizer.h"
#include <FuetEngine.h>
// ----------------------------------------------------------------------------
typedef struct TConfigOptions
{
	CFEString	m_sName;
	CFEString	m_sPlayMode;
	FEReal		m_rSpeedMult;
	FEReal		m_rAnimTime;
};
// ----------------------------------------------------------------------------
class CActorSPRGenerator : public CImageFileProcessor
{
	public:

		CActorSPRGenerator();

		virtual void Process(const fipImage& oImg);
		virtual void ParseCommandLine(int argc, char* argv[]);		
		virtual void EnterDir(const std::string& _sDirectory);
		virtual void EndProcess();

		void SaveOutput();

	protected:

		void SaveAction(COptiFrameSequence* _poAction);
		void SaveSPRFile();
    	void ParseConfigFile();
    	
		float fGetActionSpeedMult(const std::string& _sName);
		float fGetActionTime(const std::string& _sName);
		std::string sGetActionPlayMode(const std::string& _sName);
		std::string sGetActionFuncPlayMode(const std::string& _sName);
		
		void LimitDimensions(fipImage* _poImg,unsigned int _uiWidthDimsMode,unsigned int _uiHeightDimsMode);
		void AddNewAction(CFrameSequence* _poAction);
		
		CFEString				m_sWorkingDirectory;
    	CFEString				m_sOutputDirectory;
    	
    	CFEString				m_sActionSufix;			// overrides previous member

		unsigned int			m_uiImgNum;
		unsigned int			m_uiXPos;
		unsigned int			m_uiYPos;
		unsigned int			m_uiMaxHeight;

        std::vector<CFrameSequence*>		m_oActions;
        CFrameSequence*						m_poCurAction;

        std::vector<COptiFrameSequence*>	m_oOptAction;

        // config options
        std::vector<TConfigOptions> m_oActionCfgs;

		// --------------------------------------------------------
		// Parameterizable values
		// --------------------------------------------------------
		CFEConfigFile* m_poConfig;

        float		m_fFPS;

        float		m_fBlendFact;

        CFEVect2		m_oPivot;
		CFEVect2		m_oImagePivot;
        CFEVect2		m_oSpriteScale;		

        CFEVect2		m_oHeadGroupOffset;
        CFEVect2		m_oHeadOffset;
        CFEVect2		m_oFaceOffset;
        CFEVect2		m_oBodyOffset;
		unsigned int	m_uiWidthDimsMode;
		unsigned int	m_uiHeightDimsMode;
        bool			m_bBinaryOutput;
};
// ----------------------------------------------------------------------------
CActorSPRGenerator::CActorSPRGenerator()
{
    m_sActionSufix = "";
    m_sWorkingDirectory = "";
    m_sOutputDirectory = "";
    m_sOutputFilename = "";

	m_uiImgNum = 0;
	m_uiXPos = 0;
	m_uiYPos = 0;
	m_uiMaxHeight = 0;
	m_poCurAction = NULL;
	
	// parameterizables
	m_poConfig = NULL;	

	m_fFPS = 12.0;
    m_fBlendFact = 1.0; 
    m_oPivot.x = 0.5;
    m_oPivot.y = 0.5;
    m_oImagePivot.x = -1.0;
    m_oImagePivot.y = -1.0;
    m_oHeadGroupOffset.x = 0.0;
	m_oHeadGroupOffset.y = 0.0;
	m_oHeadOffset.x = 0.0;
    m_oHeadOffset.y = 0.0;  
	m_oFaceOffset.x = 0.0;
    m_oFaceOffset.y = 0.0;  
    m_oBodyOffset.x = 0.0;
    m_oBodyOffset.y = 0.0;

	m_oSpriteScale.x = 1.0;
	m_oSpriteScale.y = 1.0;

    m_bBinaryOutput = false;
    m_uiWidthDimsMode =3;
    m_uiHeightDimsMode =3;
}
// ----------------------------------------------------------------------------
float CActorSPRGenerator::fGetActionSpeedMult(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( m_oActionCfgs[i].m_rSpeedMult );

	return(1.0);
}
// ----------------------------------------------------------------------------
float CActorSPRGenerator::fGetActionTime(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( m_oActionCfgs[i].m_rAnimTime );
			
	return(-1.0);			
}
// ----------------------------------------------------------------------------
std::string CActorSPRGenerator::sGetActionPlayMode(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( std::string( m_oActionCfgs[i].m_sPlayMode.szString() ) );

	return("ONE_SHOT");
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::SaveSPRFile()
{
    CFEString sFilename = std::string(m_sOutputFilename + ".spr").c_str();

    CConfigFileWriter* poFWriter = NULL;   

    if (m_bBinaryOutput)
		poFWriter = new CBinConfigFileWriter();
	else
		poFWriter = new CLibConfigFileWriter();

    if (! poFWriter->bCreate(sFilename.szString() )) return;

    poFWriter->OpenSection("Sprite");

        poFWriter->AddVar("Name",m_sOutputFilename);
        poFWriter->AddVar("BlendMode","ALPHA");
        poFWriter->AddVar("NumActions",m_oOptAction.size());
        poFWriter->NewLine();

        for (uint uiAction=0;uiAction<m_oOptAction.size();uiAction++)
        {
            poFWriter->OpenSection("Action",uiAction);
				
				if (m_oOptAction[uiAction] != NULL)
                {
					poFWriter->AddVar("Name",m_oOptAction[uiAction]->m_sSequenceName.szString());
                	poFWriter->AddVar("PlayMode",sGetActionPlayMode(m_oOptAction[uiAction]->m_sSequenceName.szString()));

					float fActionTime = fGetActionTime(m_oOptAction[uiAction]->m_sSequenceName.szString());
					float fSpeed	  = (fActionTime<=0.0) ? fGetActionSpeedMult(m_oOptAction[uiAction]->m_sSequenceName.szString()) : 1.0;
					float fFrameTime  = (fActionTime<=0.0) ? (1.0/m_fFPS) : fActionTime / (float)m_oOptAction[uiAction]->m_oRects.size();
					float fBlendTime  = (fFrameTime * m_fBlendFact);
					float fDelayTime  = (fFrameTime * (1.0-m_fBlendFact));

					poFWriter->OpenSection("Frames");

						poFWriter->AddVar("Type","Complete");
						poFWriter->OpenSection("Complete");    

							poFWriter->AddVar("NumFrames",m_oOptAction[uiAction]->m_oRects.size());
							for (uint uiFrame=0;uiFrame<m_oOptAction[uiAction]->m_oRects.size();uiFrame++)
							{  
								CFERect* poOptFrame = &m_oOptAction[uiAction]->m_oRects[uiFrame];
								CFERect  oFrame     = m_oActions[uiAction]->m_oFrames[uiFrame]->oGetRect();

								float fFrameWidth = (poOptFrame->m_oEnd.x - poOptFrame->m_oIni.x) * m_oSpriteScale.y;
								float fFrameHeight= (poOptFrame->m_oEnd.y - poOptFrame->m_oIni.y) * m_oSpriteScale.y;
	                        
								poFWriter->OpenSection("Frame",uiFrame);
	                                
									CFEString sMat = m_oOptAction[uiAction]->m_sSequenceName + m_sActionSufix;
									poFWriter->AddVar("Material",sMat.szString());
									poFWriter->AddVar("DelayTime",fDelayTime/fSpeed,6);
									poFWriter->AddVar("BlendTime",fBlendTime/fSpeed,6);

									// WARNING: this should be corrected
									// if ((m_oPivot.x != 0.5) || (m_oPivot.y != 0.5))
									{
										CFEVect2 oPivotFrame;
										oPivotFrame.x =(m_oImagePivot.x < 0.0)?m_oPivot.x: ((m_oImagePivot.x - oFrame.m_oIni.x) / (oFrame.m_oEnd.x - oFrame.m_oIni.x));
										oPivotFrame.y =(m_oImagePivot.y < 0.0)?m_oPivot.y: ((m_oImagePivot.y - oFrame.m_oIni.y) / (oFrame.m_oEnd.y - oFrame.m_oIni.y));									

										poFWriter->OpenSection("Pivot");
										poFWriter->AddVar("x",oPivotFrame.x);
										poFWriter->AddVar("y",oPivotFrame.y);
										poFWriter->CloseSection();
									}

									poFWriter->OpenSection("Size");
										poFWriter->AddVar("Width",fFrameWidth,1);
										poFWriter->AddVar("Height",fFrameHeight,1);
									poFWriter->CloseSection();

									poFWriter->OpenSection("TexCoords");
									
										float fHalfXPixel = 0.5f / (float)m_oOptAction[uiAction]->m_oFrameSeq.getWidth();
										float fHalfYPixel = 0.5f / (float)m_oOptAction[uiAction]->m_oFrameSeq.getHeight();
										
										float fIU,fIV,fFU,fFV;
										fIU = (float)poOptFrame->m_oIni.x / (float)m_oOptAction[uiAction]->m_oFrameSeq.getWidth();										
										fIV = (float)poOptFrame->m_oIni.y / (float)m_oOptAction[uiAction]->m_oFrameSeq.getHeight();
										fFU = (float)poOptFrame->m_oEnd.x / (float)m_oOptAction[uiAction]->m_oFrameSeq.getWidth();
										fFV = (float)poOptFrame->m_oEnd.y / (float)m_oOptAction[uiAction]->m_oFrameSeq.getHeight();
										if (fIU < 0.0) fIU = 0.0;
										if (fIV < 0.0) fIV = 0.0;
										if (fFU > 1.0) fFU = 1.0;
										if (fFV > 1.0) fFV = 1.0;
    
										poFWriter->AddVar("IU",fIU);
										poFWriter->AddVar("IV",fIV);
										poFWriter->AddVar("FU",fFU);
										poFWriter->AddVar("FV",fFV);

									poFWriter->CloseSection();

								poFWriter->CloseSection(); // Frame
							}
	                    
						poFWriter->CloseSection(); // Complete

					poFWriter->CloseSection();    // Frames

				}	// if (m_oOptAction[uiAction] != NULL)

            poFWriter->CloseSection(); // Action
        }

    poFWriter->CloseSection(); // Sprite
    poFWriter->Write();
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::SaveAction(COptiFrameSequence* _poAction)
{
    CFEString sTexFilename;
    
    if (m_sActionSufix != "")
		sTexFilename.Format("%s%s.png",_poAction->m_sSequenceName,m_sActionSufix.szString() );
	else
		sTexFilename.Format("%s.png",_poAction->m_sSequenceName);
	
    _poAction->m_oFrameSeq.save(sTexFilename.szString());
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::SaveOutput()
{   
	printf("Saving to %s directory\n",m_sOutputDirectory.szString());

    if (m_sOutputDirectory != "")
        _chdir(m_sOutputDirectory.szString());
	
	// Optimize frame actions
    for (uint i=0;i<m_oActions.size();i++)
		if (m_oOptAction[i] != NULL)
			SaveAction( m_oOptAction[i] );

    // Save spr file
    SaveSPRFile();
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::ParseCommandLine(int argc, char* argv[])
{	
    CImageFileProcessor::ParseCommandLine(argc,argv);

	for (int i=1; i<argc; i++)
	{
		if (!stricmp(argv[i], "-wd"))
		{
			if ((i+1) < argc)
				m_sWorkingDirectory = CFEString( argv[i+1] );

			i++;
		}

  else if (!stricmp(argv[i], "-od"))
		{
			if ((i+1) < argc)
				m_sOutputDirectory = CFEString( argv[i+1] );

			i++;
		}

  else if (!stricmp(argv[i], "-as"))
		{
			if ((i+1) < argc)
				m_sActionSufix = CFEString( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-py"))
		{
			if ((i+1) < argc)
				m_oPivot.y = atof( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-px"))
		{
			if ((i+1) < argc)
				m_oPivot.x = atof( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-ssx"))
		{
			if ((i+1) < argc)
				m_oSpriteScale.x = atof( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-ssy"))
		{
			if ((i+1) < argc)
				m_oSpriteScale.y = atof( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-fps"))
		{
			if ((i+1) < argc)
				m_fFPS = atof( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-binary"))
		{
			m_bBinaryOutput = true;
		}

   else if (!stricmp(argv[i], "-wdm"))
		{
			if ((i+1) < argc)
				m_uiWidthDimsMode = atoi( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-hdm"))
		{
			if ((i+1) < argc)
				m_uiHeightDimsMode = atoi( argv[i+1] );

			i++;
		}

   else if (!stricmp(argv[i], "-config"))
		{
			if ((i+1) < argc)
			{
				m_poConfig = new CFEConfigFile( CFEString(argv[i+1]) );
			}

			i++;
		}
	}

	// Process command line
	if ( (m_poConfig != NULL) && (m_poConfig->bInitialized()) )
		ParseConfigFile();

	if (m_sWorkingDirectory != "")
		_chdir(m_sWorkingDirectory.szString());
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::ParseConfigFile()
{
	m_sWorkingDirectory = m_poConfig->sGetString("Config.WorkingDirectory",m_sWorkingDirectory);
	m_sOutputDirectory  = m_poConfig->sGetString("Config.OutputDirectory",m_sOutputDirectory);

	CFEString sStr = m_poConfig->sGetString("Config.OutputFilename", m_sOutputFilename.c_str() );
	m_sOutputFilename = sStr.szString();

	m_sActionSufix		= m_poConfig->sGetString("Config.ActionSufix", m_sActionSufix );

	m_uiWidthDimsMode	= m_poConfig->iGetInteger("Config.WidthDimsMode", m_uiWidthDimsMode);
	m_uiHeightDimsMode	= m_poConfig->iGetInteger("Config.WidthDimsMode", m_uiHeightDimsMode);

	//
	m_fFPS = m_poConfig->rGetReal("Config.FPS",m_fFPS);
	
	// 
	m_bBinaryOutput = m_poConfig->bGetBool("Config.BinaryOutput",m_bBinaryOutput);
	
	// 
	m_fBlendFact = m_poConfig->rGetReal("Config.BlendFact",m_fBlendFact);

	m_oPivot.x = m_poConfig->rGetReal("Config.Pivot.x",m_oPivot.x);
	m_oPivot.y = m_poConfig->rGetReal("Config.Pivot.y",m_oPivot.y);

	m_oSpriteScale.x = m_poConfig->rGetReal("Config.SpriteScale.x",m_oSpriteScale.x);
	m_oSpriteScale.y = m_poConfig->rGetReal("Config.SpriteScale.y",m_oSpriteScale.y);

	m_oImagePivot.x = m_poConfig->rGetReal("Config.ImagePivot.x",m_oImagePivot.x);
	m_oImagePivot.y = m_poConfig->rGetReal("Config.ImagePivot.y",m_oImagePivot.y);

	m_oHeadGroupOffset.x = m_poConfig->rGetReal("Config.HeadGroupOffset.x",0.0);
	m_oHeadGroupOffset.y = m_poConfig->rGetReal("Config.HeadGroupOffset.y",0.0);

	m_oFaceOffset.x = m_poConfig->rGetReal("Config.FaceOffset.x",0.0);
	m_oFaceOffset.y = m_poConfig->rGetReal("Config.FaceOffset.y",0.0);

	m_oHeadOffset.x = m_poConfig->rGetReal("Config.HeadOffset.x",0.0);
	m_oHeadOffset.y = m_poConfig->rGetReal("Config.HeadOffset.y",0.0);

	m_oBodyOffset.x = m_poConfig->rGetReal("Config.BodyOffset.x",0.0);
	m_oBodyOffset.y = m_poConfig->rGetReal("Config.BodyOffset.y",0.0);

	uint uiActions = m_poConfig->iGetInteger("Config.ActionConfig.NumActions",0);
	for (uint i=0;i<uiActions;i++)
	{
		TConfigOptions m_oOpt;

		CFEString sVar		= CFEString ("Config.ActionConfig.Action") + CFEString(i);
		m_oOpt.m_sName		= m_poConfig->sGetString(sVar + ".ActionName","none");
		m_oOpt.m_sPlayMode	= m_poConfig->sGetString(sVar + ".PlayMode","ONESHOT");
		m_oOpt.m_rSpeedMult = m_poConfig->rGetReal(sVar + ".SpeedMult",1.0);
		m_oOpt.m_rAnimTime  = m_poConfig->rGetReal(sVar + ".AnimTime",-1.0);

		m_oActionCfgs.push_back(m_oOpt);
	}
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::AddNewAction(CFrameSequence* _poAction)
{
	m_oActions.push_back(_poAction);

	CFrameOptimizer oOptimizer;
	COptiFrameSequence* poOptCurAction = oOptimizer.poProcess( *_poAction );
	m_oOptAction.push_back( poOptCurAction );

	// Limit output to specified dimensions (powerOf2, mult8, mult4, none)
	LimitDimensions(&poOptCurAction->m_oFrameSeq, m_uiWidthDimsMode,m_uiHeightDimsMode);
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::EnterDir(const std::string& _sDirectory)
{
    if (m_poCurAction)
		AddNewAction(m_poCurAction);

    m_poCurAction = new CFrameSequence( _sDirectory.c_str() );

	m_uiImgNum = 0;
	m_uiXPos = 0;
	m_uiYPos = 0;
	m_uiMaxHeight = 0;

    printf("%s \n",_sDirectory.c_str());
}
// ----------------------------------------------------------------------------
static unsigned int ROUNDUP(unsigned int _uiSize,unsigned int _uiBase)
{
	return ((_uiSize+(_uiBase-1)) & (~(_uiBase-1)) );
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::LimitDimensions(fipImage* _poImg,unsigned int _uiWidthDimsMode,unsigned int _uiHeightDimsMode)
{
	CFrame oFrame(*_poImg);
	unsigned int uiNewWidth = 0;
	unsigned int uiNewHeight = 0;

	// Width dims
	switch (_uiWidthDimsMode)
	{
		// no limitation
		case 0:
		{
			uiNewWidth  = oFrame.oGetImage().getWidth();
		}
		break;
		
		// multiple of 4
		case 1:
		{
			uiNewWidth  = ROUNDUP(oFrame.oGetImage().getWidth(),4);
		}
		break;
		
		// multiple of 8
		case 2:
		{
			uiNewWidth  = ROUNDUP(oFrame.oGetImage().getWidth(),8);
		}
		break;

		// power of 2: nothing to do
		case 3:
		{
			uiNewWidth  = _poImg->getWidth();
		}
		break;
	}

	// Height dims
	switch (_uiHeightDimsMode)
	{
		// no limitation
		case 0:
		{
			uiNewHeight  = oFrame.oGetImage().getHeight();
		}
		break;
		
		// multiple of 4
		case 1:
		{
			uiNewHeight  = ROUNDUP(oFrame.oGetImage().getHeight(),4);
		}
		break;
		
		// multiple of 8
		case 2:
		{
			uiNewHeight  = ROUNDUP(oFrame.oGetImage().getHeight(),8);
		}
		break;

		// power of 2: nothing to do
		case 3:
		{
			uiNewHeight  = _poImg->getHeight();
		}
		break;
	}

	_poImg->crop(0,0,uiNewWidth,uiNewHeight);
}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::EndProcess()
{
    if (m_poCurAction)
		AddNewAction(m_poCurAction);
	}
// ----------------------------------------------------------------------------
void CActorSPRGenerator::Process(const fipImage& oImg)
{
    CFrame* poFrame = new CFrame(oImg);
    if (m_poCurAction == NULL)
		m_poCurAction = new CFrameSequence( "anim" );

    m_poCurAction->AddFrame(poFrame);
}
// ----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	CActorSPRGenerator oAG;

	oAG.ParseCommandLine(argc,argv);
	oAG.ProcessFiles("");
	oAG.SaveOutput();

	return(0);
}
// ----------------------------------------------------------------------------
