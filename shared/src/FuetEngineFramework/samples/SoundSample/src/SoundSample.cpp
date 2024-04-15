//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <FuetEngine.h>

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
CFERenderer* 	gpoRenderer = NULL;
CFEFont*    	gpoFont;
FEHandler		ghLoopSound;
FEHandler		ghTrigSound;
FEHandler		ghSndSerie[3];
bool			gbPlayingLoop = false;
CFEInput*		gpoInput;
uint 			guiSndIdx = 0;
FEReal			grFPS = _0r;
//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
uint SCREEN_VWIDTH  = 0;
uint SCREEN_VHEIGHT = 0;

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{

    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoFont = CFEFontMgr::I()->poLoad("font");

    SCREEN_VWIDTH  = gpoRenderer->uiGetScreenVWidth();
    SCREEN_VHEIGHT = gpoRenderer->uiGetScreenVHeight();
    gpoInput = new CFEInput;
    gpoInput->Init(_pParam);
    
    CFESoundPlayer::Init(_pParam);

    ghTrigSound = CFESoundMgr::I()->hLoad("trig");
    ghLoopSound = CFESoundMgr::I()->hLoad("loop",true);
    CFESoundMgr::I()->bSetSoundProperty(ghLoopSound,"Loop",(FEPointer)true);
    
    ghSndSerie[0] = CFESoundMgr::I()->hLoad("spring");
    ghSndSerie[1] = CFESoundMgr::I()->hLoad("spring-1");
    ghSndSerie[2] = CFESoundMgr::I()->hLoad("spring-2");

    CFESoundPlayer::hPlay(ghLoopSound,SML_BGM);
    gbPlayingLoop = true;
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
    gpoRenderer->Finish();
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
	FEReal rDeltaT = gpoRenderer->rGetDeltaT();

	FEReal rNewFPS = _1r / rDeltaT;
    if (CFEMath::bIsFinite(rNewFPS))
		grFPS = rNewFPS;
	
	gpoInput->Update(rDeltaT);
	CFESoundMixer::Update(rDeltaT);
	CFESoundPlayer::Update(rDeltaT);	

    gpoRenderer->BeginScene();	

        gpoRenderer->ResetCamera();
        gpoRenderer->SetBlendMode(BM_ALPHA);
        gpoRenderer->SetMaterial(NULL);

        gpoRenderer->RenderQuad(0,0,SCREEN_VWIDTH,SCREEN_VHEIGHT,CFEColor(1,1,1,1));

        gpoRenderer->SetFont(gpoFont);
        gpoRenderer->SetTextPointSize(_05r);

        CFEString sString;
        sString.Format("Sound Sample: %.1f",grFPS);
    	gpoRenderer->RenderText(sString,0,48,CFEColor(1,0,0,1),THAM_LEFT);

		if (gpoInput->bDown(IB_A))
			CFESoundPlayer::hPlay(ghTrigSound,SML_FX);

		if (gpoInput->bDown(IB_B))
		{
			if (! gbPlayingLoop)
				CFESoundPlayer::hPlay(ghLoopSound,SML_BGM);
			else
				CFESoundPlayer::Stop(ghLoopSound);

			gbPlayingLoop = !gbPlayingLoop;
		}
		
		if (gpoInput->bDown(IB_C))
		{
			CFESoundPlayer::hPlay(ghSndSerie[guiSndIdx],SML_FX);
		}
		
		if (gpoInput->bDown(IB_UP))
		{
			if (guiSndIdx<2)
				guiSndIdx++;
		}

		if (gpoInput->bDown(IB_DOWN))
		{
			if (guiSndIdx>0)
				guiSndIdx--;
		}
		
    gpoRenderer->EndScene();
}
//-----------------------------------------------------------------------------
