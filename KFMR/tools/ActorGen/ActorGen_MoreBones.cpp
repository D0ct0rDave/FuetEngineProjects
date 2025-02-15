// Standard Gen Includes

#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include <direct.h>
// #include <io.h>
#include <vector>
#include <string>
#include <math.h>

#include "CImageFileProcessor.h"
#include "CLibConfigFileWriter.h"
#include "CBinConfigFileWriter.h"
#include <FuetEngine.h>

// ----------------------------------------------------------------------------
const uint IMAGE_WIDTH  = 1024;
const uint IMAGE_HEIGHT = 512;
static int iKeyFrame = 0;
// ----------------------------------------------------------------------------
class CAnimActionInfo
{
    public:

        CAnimActionInfo(const std::string& _sActionName)
        {
            m_sActionName = _sActionName;
        }

        // name of the action
        std::string         m_sActionName;

        // head coords
        std::vector<CFEVect2> m_oHeadCoords;
        std::vector<CFEVect2> m_oNeckCoords;

        // left hand coords
        std::vector<CFEVect2> m_oLHCoords;
        std::vector<CFEVect2> m_oLElbowCoords;
        
        // right hand coords
        std::vector<CFEVect2> m_oRHCoords;
        std::vector<CFEVect2> m_oRElbowCoords;
        
        // left foot coords
        std::vector<CFEVect2> m_oLFCoords;
        // left foot tip coords
        std::vector<CFEVect2> m_oLFTCoords;
        
        // right foot coords
        std::vector<CFEVect2> m_oRFCoords;
        // right foot tip coords
        std::vector<CFEVect2> m_oRFTCoords;
        
        // stomach coords
        std::vector<CFEVect2> m_oBodyCoords;
};
// ----------------------------------------------------------------------------
typedef struct TConfigOptions
{
	CFEString	m_sName;
	CFEString	m_sPlayMode;
	FEReal		m_rSpeedMult;
	FEReal		m_rAnimTime;
};
// ----------------------------------------------------------------------------
class CActorGenerator : public CImageFileProcessor
{
	public:
		CActorGenerator();
		
		virtual void Process(const fipImage& oImg);
		virtual void ParseCommandLine(int argc, char* argv[]);		
		virtual void EnterDir(const std::string& _sDirectory);
		virtual void EndProcess();
		
		void SaveOutput();
		
	protected:
		void SaveSKAFile();
		void SaveSPRFile();
		void ParseConfigFile();
		
		void FinishAction(CAnimActionInfo* _poAnim);

		float fGetActionSpeedMult(const std::string& _sName);
		float fGetActionTime(const std::string& _sName);
		std::string sGetActionPlayMode(const std::string& _sName);
		std::string sGetActionFuncPlayMode(const std::string& _sName);
		
		CFEString				m_sWorkingDirectory;
    	CFEString				m_sOutputDirectory;

		unsigned int			m_uiImgNum;
		unsigned int			m_uiXPos;
		unsigned int			m_uiYPos;
		unsigned int			m_uiMaxHeight;

        std::vector<CAnimActionInfo*> m_oActions;
        CAnimActionInfo*        m_poCurAction;

        // config options
        std::vector<TConfigOptions> m_oActionCfgs;

        float m_fBlendTime;
        float m_fDelayTime;

		// --------------------------------------------------------
		// Parameterizable values
		// --------------------------------------------------------
		CFEConfigFile* m_poConfig;

        float		m_fFPS;

        float		m_fBlendFact;

        CFEVect2		m_oPivot;
        CFEVect2		m_oImagePivot;

        CFEVect2		m_oHeadGroupOffset;
        CFEVect2		m_oHeadOffset;
        CFEVect2		m_oFaceOffset;
        CFEVect2		m_oBodyOffset;
        CFEVect2		m_oMainNodeScale;
        CFEVect2		m_oMainNodeOffset;
        
        bool			m_bBinaryOutput;
};
// ----------------------------------------------------------------------------
void WriteKeyFrames(const std::vector<CFEVect2>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode,float _fFrameTime)
{
    _poFWriter->OpenSection("PosFunc");

		_poFWriter->AddVar("WrapMode",_sWrapMode);
        _poFWriter->AddVar("NumKeyFrames",_oCoords.size());

        float fKeyFrameTime = 0.0;
        for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.size();uiKeyFrame++)
        {
            _poFWriter->OpenSection("KeyFrame",uiKeyFrame);
                
                _poFWriter->AddVar("x", _oCoords[uiKeyFrame].x);
                _poFWriter->AddVar("y", _oCoords[uiKeyFrame].y);
                _poFWriter->AddVar("Time",fKeyFrameTime);
                _poFWriter->AddVar("LerpFunc", "linear" );

            _poFWriter->CloseSection();
            fKeyFrameTime += _fFrameTime;
        };

    _poFWriter->CloseSection();
}
// ----------------------------------------------------------------------------
void WriteKeyFrames(const std::vector<CFEVect2>& _oCoords,const std::vector<CFEVect2>& _oCoords2,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode,float _fFrameTime)
{
    WriteKeyFrames(_oCoords,_poFWriter,_sWrapMode,_fFrameTime);

    _poFWriter->OpenSection("AngleFunc");

		_poFWriter->AddVar("WrapMode",_sWrapMode);
        _poFWriter->AddVar("NumKeyFrames",_oCoords.size());

        float fKeyFrameTime = 0.0;
        float fLastAngle = 0.0;
        for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.size();uiKeyFrame++)
        {
            _poFWriter->OpenSection("KeyFrame",uiKeyFrame);
                
                CFEVect2 oD = _oCoords2[uiKeyFrame] - _oCoords[uiKeyFrame];
                oD.Normalize();

                float fAngle = atan2(oD.x,oD.y) + _PIr_;
                if ((uiKeyFrame>0) && (fabs(fAngle - fLastAngle)>_PIr_))
                {
                    if (fAngle > fLastAngle)
                        fAngle -= _2PIr_;
                    else
                        fAngle += _2PIr_;
                }

                fLastAngle = fAngle;

                _poFWriter->AddVar("Value",fAngle);
                _poFWriter->AddVar("Time",fKeyFrameTime);
                _poFWriter->AddVar("LerpFunc", "linear" );

            _poFWriter->CloseSection();
            fKeyFrameTime += _fFrameTime;
        };

    _poFWriter->CloseSection();
}
// ----------------------------------------------------------------------------
CActorGenerator::CActorGenerator()
{
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

    m_oHeadOffset.x = 0.0;
    m_oHeadOffset.y = 0.0;  

    m_oImagePivot.x = 0.0;
    m_oImagePivot.y = 0.0;
    
    m_bBinaryOutput = false;
    
    m_oMainNodeScale.x = 1.0;
    m_oMainNodeScale.y = 1.0;
    
    m_oMainNodeOffset.x = 0.0;
    m_oMainNodeOffset.y = 0.0;
}
// ----------------------------------------------------------------------------
float CActorGenerator::fGetActionSpeedMult(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( m_oActionCfgs[i].m_rSpeedMult );

	return(1.0);
}
// ----------------------------------------------------------------------------
float CActorGenerator::fGetActionTime(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( m_oActionCfgs[i].m_rAnimTime );
	
	return(-1.0);
}
// ----------------------------------------------------------------------------
std::string CActorGenerator::sGetActionPlayMode(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
			return( std::string( m_oActionCfgs[i].m_sPlayMode.szString() ) );

	return("ONE_SHOT");
}
// ----------------------------------------------------------------------------
std::string CActorGenerator::sGetActionFuncPlayMode(const std::string& _sName)
{
	for (uint i=0;i<m_oActionCfgs.size();i++)
		if (m_oActionCfgs[i].m_sName |= CFEString(_sName.c_str()))
		{
			if (m_oActionCfgs[i].m_sPlayMode |= "ONESHOT")
				return("FinalValue");
	   else if (m_oActionCfgs[i].m_sPlayMode |= "LOOP")
				return("Loop");
	   else if (m_oActionCfgs[i].m_sPlayMode |= "PINGPONG")
				return("PingPong");
		}

	return("FinalValue");
}
// ----------------------------------------------------------------------------
void CActorGenerator::SaveSKAFile()
{
    CFEString sFilename = std::string (m_sOutputFilename + ".ska").c_str();

    CConfigFileWriter* poFWriter = NULL;   
    if (m_bBinaryOutput)
		poFWriter = new CBinConfigFileWriter();
	else
		poFWriter = new CLibConfigFileWriter();

    if (! poFWriter->bCreate(sFilename.szString() )) return;

    /*
    float fFrameTime = (1.0/m_fFPS);
    float fBlendTime = fFrameTime * m_fBlendFact;
    float fDelayTime = fFrameTime * (1.0-m_fBlendFact);
    */

    poFWriter->OpenSection("SkelAnim");
        poFWriter->AddVar("Name",m_sOutputFilename.c_str());
        poFWriter->NewLine();
        poFWriter->OpenSection("AnimNode");

            poFWriter->AddVar("Type","group");
		    poFWriter->AddVar("Name","Actor");

		    if ((m_oMainNodeScale.x != 1.0) || (m_oMainNodeScale.y != 1.0))
		    {
		        poFWriter->OpenSection("Scale");
                    poFWriter->AddVar("x",m_oMainNodeScale.x);
	    	        poFWriter->AddVar("y",m_oMainNodeScale.y);
	    	    poFWriter->CloseSection();
		    }

		    if ((m_oMainNodeOffset.x != 0.0) || (m_oMainNodeOffset.y != 0.0))
		    {
		        poFWriter->OpenSection("Position");
                    poFWriter->AddVar("x",m_oMainNodeOffset.x);
	    	        poFWriter->AddVar("y",m_oMainNodeOffset.y);
	    	    poFWriter->CloseSection();
		    }

		    poFWriter->AddVar("NumNodes",8);
		    poFWriter->NewLine();

			// Left part of the body
			std::string sSpriteFilename;
			sSpriteFilename = m_sOutputFilename + "_l";
		    poFWriter->OpenSection("Node",0);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","extremities_l");
                poFWriter->AddVar("Sprite",sSpriteFilename);
            poFWriter->CloseSection();

			// Left hand
		    poFWriter->OpenSection("Node",1);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","hand_left");
                poFWriter->AddVar("Sprite","hand_left");
            poFWriter->CloseSection();

			// Left foot
		    poFWriter->OpenSection("Node",2);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","foot_left");
                poFWriter->AddVar("Sprite","foot_left");
            poFWriter->CloseSection();

			// Body
		    poFWriter->OpenSection("Node",3);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","body");
                poFWriter->AddVar("Sprite","body");

                if ((m_oBodyOffset.x != 0.0) || (m_oBodyOffset.y != 0.0))
                {
					poFWriter->OpenSection("Position");

						if (m_oBodyOffset.x != 0.0)
							poFWriter->AddVar("x",m_oBodyOffset.x);
						if (m_oBodyOffset.y != 0.0)
							poFWriter->AddVar("y",m_oBodyOffset.y);

					poFWriter->CloseSection();
                }
            poFWriter->CloseSection();
			
			// Head group
            poFWriter->OpenSection("Node",4);

				poFWriter->AddVar("Type","group");
				poFWriter->AddVar("Name","HeadGroup");

                if ((m_oHeadGroupOffset.x != 0.0) || (m_oHeadGroupOffset.y != 0.0))
                {
					poFWriter->OpenSection("Position");

						if (m_oHeadGroupOffset.x != 0.0)
							poFWriter->AddVar("x",m_oHeadGroupOffset.x);
						if (m_oHeadGroupOffset.y != 0.0)
							poFWriter->AddVar("y",m_oHeadGroupOffset.y);

					poFWriter->CloseSection();
                }

				poFWriter->AddVar("NumNodes",2);
				poFWriter->NewLine();    
				
				poFWriter->OpenSection("Node",0);
					poFWriter->AddVar("Type","sprite");
					poFWriter->AddVar("Name","head");
					poFWriter->AddVar("Sprite","head");
					
					if ((m_oHeadOffset.x != 0.0) || (m_oHeadOffset.y != 0.0))
					{
						poFWriter->OpenSection("Position");

							if (m_oHeadOffset.x != 0.0)
								poFWriter->AddVar("x",m_oHeadOffset.x);
							if (m_oHeadOffset.y != 0.0)
								poFWriter->AddVar("y",m_oHeadOffset.y);

						poFWriter->CloseSection();
					}

				poFWriter->CloseSection();

				poFWriter->OpenSection("Node",1);
					poFWriter->AddVar("Type","sprite");
					poFWriter->AddVar("Name","face");
					poFWriter->AddVar("Sprite","face");
					
					if ((m_oFaceOffset.x != 0.0) || (m_oFaceOffset.y != 0.0))
					{
						poFWriter->OpenSection("Position");

							if (m_oFaceOffset.x != 0.0)
								poFWriter->AddVar("x",m_oFaceOffset.x);
							if (m_oFaceOffset.y != 0.0)
								poFWriter->AddVar("y",m_oFaceOffset.y);

						poFWriter->CloseSection();
					}

				poFWriter->CloseSection();

			poFWriter->CloseSection();
		
			// Right part of the body
			sSpriteFilename = m_sOutputFilename+"_r";
		    poFWriter->OpenSection("Node",5);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","extremities_r");
                poFWriter->AddVar("Sprite",sSpriteFilename);
            poFWriter->CloseSection();

			// Right hand
		    poFWriter->OpenSection("Node",6);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","hand_right");
                poFWriter->AddVar("Sprite","hand_right");
            poFWriter->CloseSection();

			// Right foot
		    poFWriter->OpenSection("Node",7);
                poFWriter->AddVar("Type","sprite");
                poFWriter->AddVar("Name","foot_right");
                poFWriter->AddVar("Sprite","foot_right");
            poFWriter->CloseSection();

        poFWriter->CloseSection();
        poFWriter->NewLine();

        poFWriter->OpenSection("ActionSet");    
        poFWriter->AddVar("NumActions",m_oActions.size());

        for (uint uiAction=0;uiAction<m_oActions.size();uiAction++)
        {
			float fActionTime = fGetActionTime(m_oActions[uiAction]->m_sActionName);
			float fSpeed     = fGetActionSpeedMult(m_oActions[uiAction]->m_sActionName);
			float fAnimTime  = (fActionTime > 0.0)?fActionTime:((float)m_oActions[uiAction]->m_oHeadCoords.size() / m_fFPS) / fSpeed;
			float fFrameTime = (fAnimTime / (m_oActions[uiAction]->m_oHeadCoords.size()));

            poFWriter->OpenSection("Action",uiAction);
    
                poFWriter->AddVar("Name",m_oActions[uiAction]->m_sActionName);
				poFWriter->AddVar("PlayMode",sGetActionPlayMode(m_oActions[uiAction]->m_sActionName));
                poFWriter->AddVar("NumNodeActions",8);
            
                // Right Extremities 
                poFWriter->OpenSection("NodeAction",0);
                
                    poFWriter->AddVar("NodeName","extremities_r");
                    poFWriter->AddVar("NodeAction",m_oActions[uiAction]->m_sActionName);

                poFWriter->CloseSection();
                
                // Left Extremities 
                poFWriter->OpenSection("NodeAction",1);
                
                    poFWriter->AddVar("NodeName","extremities_l");
                    poFWriter->AddVar("NodeAction",m_oActions[uiAction]->m_sActionName);

                poFWriter->CloseSection();
               
                std::string sAPM = sGetActionFuncPlayMode(m_oActions[uiAction]->m_sActionName);
                
                // Head node
                poFWriter->OpenSection("NodeAction",2);

                    poFWriter->AddVar("NodeName","HeadGroup");
                    poFWriter->AddVar("NodeAction","HeadGroup");

                    WriteKeyFrames(m_oActions[uiAction]->m_oHeadCoords,m_oActions[uiAction]->m_oNeckCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();
                
                // Body node
                poFWriter->OpenSection("NodeAction",3);

                    poFWriter->AddVar("NodeName","body");
                    poFWriter->AddVar("NodeAction","body");

                    WriteKeyFrames(m_oActions[uiAction]->m_oBodyCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();
                
                // Left hand node
                poFWriter->OpenSection("NodeAction",4);

                    poFWriter->AddVar("NodeName","hand_left");
                    poFWriter->AddVar("NodeAction","hand_left");
                    
                    WriteKeyFrames(m_oActions[uiAction]->m_oLHCoords,m_oActions[uiAction]->m_oLElbowCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();
                
                // Right hand node
                poFWriter->OpenSection("NodeAction",5);

                    poFWriter->AddVar("NodeName","hand_right");
                    poFWriter->AddVar("NodeAction","hand_right");

                    WriteKeyFrames(m_oActions[uiAction]->m_oRHCoords,m_oActions[uiAction]->m_oRElbowCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();

                // Left foot node
                poFWriter->OpenSection("NodeAction",6);

                    poFWriter->AddVar("NodeName","foot_left");
                    poFWriter->AddVar("NodeAction","foot_left");

                    WriteKeyFrames(m_oActions[uiAction]->m_oLFCoords,m_oActions[uiAction]->m_oLFTCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();
                
                // Right foot node
                poFWriter->OpenSection("NodeAction",7);

                    poFWriter->AddVar("NodeName","foot_right");
                    poFWriter->AddVar("NodeAction","foot_right");

                    WriteKeyFrames(m_oActions[uiAction]->m_oRFCoords,m_oActions[uiAction]->m_oRFTCoords,poFWriter,sAPM,fFrameTime);

                poFWriter->CloseSection();
                
                // Right foot node
            poFWriter->CloseSection();
        }

        poFWriter->CloseSection();

    poFWriter->CloseSection();
    poFWriter->Write();
}
// ----------------------------------------------------------------------------
void CActorGenerator::SaveOutput()
{   
    if (m_sOutputDirectory != "")
        _chdir(m_sOutputDirectory.szString());

    // Save ska file
    SaveSKAFile(); 
}
// ----------------------------------------------------------------------------
void CActorGenerator::ParseCommandLine(int argc, char* argv[])
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

   else if (!stricmp(argv[i], "-binary"))
		{
			m_bBinaryOutput = true;
		}

   else if (!stricmp(argv[i], "-fps"))
		{
			if ((i+1) < argc)
				m_fFPS = atof( argv[i+1] );

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
void CActorGenerator::ParseConfigFile()
{
	m_sWorkingDirectory = m_poConfig->sGetString("Config.WorkingDirectory",m_sWorkingDirectory);
	m_sOutputDirectory  = m_poConfig->sGetString("Config.OutputDirectory",m_sOutputDirectory);
	CFEString sStr = m_poConfig->sGetString("Config.OutputFilename", m_sOutputFilename.c_str() );
	m_sOutputFilename   = sStr.szString();

	//
	m_fFPS = m_poConfig->rGetReal("Config.FPS",m_fFPS);

	//
	m_bBinaryOutput = m_poConfig->bGetBool("Config.BinaryOutput",m_bBinaryOutput);
	
	// 
	m_fBlendFact = m_poConfig->rGetReal("Config.BlendFact",m_fBlendFact);

	m_oPivot.x = m_poConfig->rGetReal("Config.Pivot.x",m_oPivot.x);
	m_oPivot.y = m_poConfig->rGetReal("Config.Pivot.y",m_oPivot.y);

	m_oMainNodeScale.x = m_poConfig->rGetReal("Config.MainNodeScale.x",m_oMainNodeScale.x);
	m_oMainNodeScale.y = m_poConfig->rGetReal("Config.MainNodeScale.y",m_oMainNodeScale.y);
	m_oMainNodeOffset.x = m_poConfig->rGetReal("Config.MainNodeOffset.x",m_oMainNodeOffset.x);
	m_oMainNodeOffset.y = m_poConfig->rGetReal("Config.MainNodeOffset.y",m_oMainNodeOffset.y);

	m_oImagePivot.x = m_poConfig->rGetReal("Config.HeadProcess.ImagePivot.x",m_oImagePivot.x);
	m_oImagePivot.y = m_poConfig->rGetReal("Config.HeadProcess.ImagePivot.y",m_oImagePivot.y);

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
void CActorGenerator::EnterDir(const std::string& _sDirectory)
{
    if (m_poCurAction)
    {
		FinishAction(m_poCurAction);
        m_oActions.push_back(m_poCurAction);
	}

    m_poCurAction = new CAnimActionInfo(_sDirectory);

	m_uiImgNum = 0;
	m_uiXPos = 0;
	m_uiYPos = 0;
	m_uiMaxHeight = 0;

    printf("%s \n",_sDirectory.c_str());
}
// ----------------------------------------------------------------------------
void CActorGenerator::FinishAction(CAnimActionInfo* _poAnim)
{
    if (sGetActionFuncPlayMode(_poAnim->m_sActionName) == "Loop") 
    {
		// append the first key frame to the end ....
		if (_poAnim->m_oHeadCoords.size() > 0)
			_poAnim->m_oHeadCoords.push_back( _poAnim->m_oHeadCoords[0] );
		if (_poAnim->m_oNeckCoords.size() > 0)
			_poAnim->m_oNeckCoords.push_back( _poAnim->m_oNeckCoords[0] );
		if (_poAnim->m_oBodyCoords.size() > 0)
			_poAnim->m_oBodyCoords.push_back( _poAnim->m_oBodyCoords[0] );
		
		if (_poAnim->m_oLHCoords.size() > 0)
			_poAnim->m_oLHCoords.push_back( _poAnim->m_oLHCoords[0] );
		if (_poAnim->m_oLElbowCoords.size() > 0)
			_poAnim->m_oLElbowCoords.push_back( _poAnim->m_oLElbowCoords[0] );

		if (_poAnim->m_oRHCoords.size() > 0)
			_poAnim->m_oRHCoords.push_back( _poAnim->m_oRHCoords[0] );
		if (_poAnim->m_oRElbowCoords.size() > 0)
			_poAnim->m_oRElbowCoords.push_back( _poAnim->m_oRElbowCoords[0] );

		if (_poAnim->m_oLFCoords.size() > 0)
			_poAnim->m_oLFCoords.push_back( _poAnim->m_oLFCoords[0] );
		if (_poAnim->m_oLFTCoords.size() > 0)
			_poAnim->m_oLFTCoords.push_back( _poAnim->m_oLFTCoords[0] );

		if (_poAnim->m_oRFCoords.size() > 0)
			_poAnim->m_oRFCoords.push_back( _poAnim->m_oRFCoords[0] );
		if (_poAnim->m_oRFTCoords.size() > 0)
			_poAnim->m_oRFTCoords.push_back( _poAnim->m_oRFTCoords[0] );
    }
    else
    {
		// append the last key frame to the end ....
		/*
		if (_poAnim->m_oHeadCoords.size() > 0)
			_poAnim->m_oHeadCoords.push_back( _poAnim->m_oHeadCoords[_poAnim->m_oHeadCoords.size()-1] );
		if (_poAnim->m_oBodyCoords.size() > 0)
			_poAnim->m_oBodyCoords.push_back( _poAnim->m_oBodyCoords[_poAnim->m_oBodyCoords.size()-1] );
        */
    }
}
// ----------------------------------------------------------------------------
void CActorGenerator::EndProcess()
{
    if (m_poCurAction)
    {
		FinishAction(m_poCurAction);
        m_oActions.push_back(m_poCurAction);
	}
}
// ----------------------------------------------------------------------------
static float fColorDistance(const CFEColor& oA,const CFEColor& oB)
{
	float fRD = (float)(oA.r - oB.r);
	float fGD = (float)(oA.g - oB.g);
	float fBD = (float)(oA.b - oB.b);
	float fAD = (float)(oA.a - oB.a);

	return ( sqrtf(fRD*fRD + fGD*fGD + fBD*fBD + fAD*fAD) );
}
// ----------------------------------------------------------------------------
void CActorGenerator::Process(const fipImage& oImg)
{
	// 
	fipImage oAlphaImg = oImg;
    oImg.getChannel(oAlphaImg,FICC_ALPHA);
    oAlphaImg.convertTo32Bits();

	CFEColor oKeyColor;
	for (int iPass=1;iPass<12;iPass++)
	{
		switch (iPass)
		{
			case 0:
				// First pass is for image pivot.
				oKeyColor = CFEColor(0,0,0,1);
			break;

			case 1:
				// head pos.
				oKeyColor = CFEColor(1,0,0,1);
			break;

			case 2:
				// neck pos.
				oKeyColor = CFEColor(1,0,0,0.5);
			break;

			case 3:
				// Left hand pos.
				oKeyColor = CFEColor(0,0,1,1);
			break;

			case 4:
				// Left Elbow pos
				oKeyColor = CFEColor(0,0,1,0.5);
			break;

			case 5:
				// Right hand pos.
				oKeyColor = CFEColor(1,0,1,1);
			break;

			case 6:
				// Right Elbow pos
				oKeyColor = CFEColor(1,0,1,0.5);
			break;

			case 7:
				// Stomach hand pos.
				oKeyColor = CFEColor(0,1,0,1);
			break;

			case 8:
				// Left foot pos.
				oKeyColor = CFEColor(0,1,1,1);
			break;

			case 9:
				// Left foot tip pos
				oKeyColor = CFEColor(0,1,1,0.5);
			break;

			case 10:
				// Right foot pos.
				oKeyColor = CFEColor(1,1,0,1);
			break;

			case 11:
				// Right foot tip pos
				oKeyColor = CFEColor(1,1,0,0.5);
			break;
	    }
		
		// Scan the image for the body part centroid
		double dX = 0.0;
		double dY = 0.0;
		double dPassPixels = 0.0;

		for (int j=0;j<oImg.getHeight();j++)
			for (int i=0;i<oImg.getWidth();i++)
			{
				RGBQUAD oRGBColor;						
				RGBQUAD oAlphaColor;
				
				oImg.getPixelColor(i, j, &oRGBColor);				
				oAlphaImg.getPixelColor(i, j, &oAlphaColor);

				CFEColor oColor((FEReal)oRGBColor.rgbRed/255.0,
								(FEReal)oRGBColor.rgbGreen/255.0,
								(FEReal)oRGBColor.rgbBlue/255.0,
								(FEReal)oAlphaColor.rgbRed/255.0);

				const float DIST_THRESHOLD = 0.1;
				float fD = fColorDistance(oColor,oKeyColor);

				if (fD < DIST_THRESHOLD)
				{
					// TEX_fn_SetFColor(_poTex,i,j,oKeyColor);
					dX = dX + (double(i) / (1.0-fD));
					dY = dY + (double(j) / (1.0-fD));
					dPassPixels += 1.0;
				}
			}

		if (dPassPixels > 0.0)
		{
			dX = (dX / dPassPixels) /* / (double)(oImg.getWidth()) */;
			dY = (dY / dPassPixels) /* / (double)(oImg.getHeight())*/;

			// head pivot is at (0.5,0.5) so ...
			// dY += (oImg.getHeight()/2);
			/*
			dX -= m_oPivot.x;
			dY -= m_oPivot.y;

			// ... and coords are upside-down
			dY *= -1.0;
			
			// coords in pixels
			dX *=oImg.getWidth();
			dY *=oImg.getHeight();
			*/

			// ... and coords are upside-down
			dY = oImg.getHeight() - dY - 1;

			if (iPass == 0)
			{
			    m_oImagePivot.x = dX;
			    m_oImagePivot.y = dY;			
            }
            else
            {
			    dX -= m_oImagePivot.x;
			    dY -= m_oImagePivot.y;
            }


			switch (iPass)
			{
				case 1:
					m_poCurAction->m_oHeadCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 2:
					m_poCurAction->m_oNeckCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 3:
					m_poCurAction->m_oLHCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 4:
					m_poCurAction->m_oLElbowCoords.push_back( CFEVect2(dX,dY) );
				break;
				
				case 5:
					m_poCurAction->m_oRHCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 6:
					m_poCurAction->m_oRElbowCoords.push_back( CFEVect2(dX,dY) );
				break;
				
				case 7:
					m_poCurAction->m_oBodyCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 8:
					m_poCurAction->m_oLFCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 9:
					m_poCurAction->m_oLFTCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 10:
					m_poCurAction->m_oRFCoords.push_back( CFEVect2(dX,dY) );
				break;

				case 11:
					m_poCurAction->m_oRFTCoords.push_back( CFEVect2(dX,dY) );
				break;
			}
		}
	}

	m_uiImgNum++;
}
// ----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	CActorGenerator oAG;

	oAG.ParseCommandLine(argc,argv);
	oAG.ProcessFiles("");
	oAG.SaveOutput();

	return(0);
}
// ----------------------------------------------------------------------------
