//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
FEHandler gh;

float gfAngle = 0.0f;
bool gbLink = true;

typedef enum
{
    IS_INIT_INTRO,

    IS_SCREEN_1,
    IS_SCREEN_1_OUT,

    IS_SCREEN_2,
    IS_SCREEN_2_OUT,

    IS_SCREEN_3,
    IS_SCREEN_3_OUT,

    IS_FINISH_INTRO

}EIntroState;

typedef struct TGlobals
{
    FEHandler   m_hBackground[3];
    uint        m_uiCurBck;
    EIntroState m_eIntroState;

    CFERenderer m_oRender;
    CFEInput    m_oInput;
};

static TGlobals* globals = NULL;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    //
	globals = new TGlobals;    
    globals->m_oRender.Init(_pParam);
    globals->m_oInput.Init(_pParam);
	
	globals->m_oRender.SetScreenVWidth(1280);
	globals->m_oRender.SetScreenVHeight(960);
	
    globals->m_hBackground[0] = CFEMaterialMgr::I()->hLoad("bck1");
    globals->m_hBackground[1] = CFEMaterialMgr::I()->hLoad("bck2");
    globals->m_hBackground[2] = CFEMaterialMgr::I()->hLoad("bck3");

    globals->m_uiCurBck = 0;
    globals->m_eIntroState = IS_INIT_INTRO;
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
    globals->m_oRender.Finish();
}
// -----------------------------------------------------------------------------
// State Management
// -----------------------------------------------------------------------------
void OnExitState(EIntroState _eState)
{
    switch ( _eState)
    {
        case IS_SCREEN_1:
        CFEScreenFX::FadeOut(1.0f);
        break;

        case IS_SCREEN_2:
        CFEScreenFX::FadeOut(1.0f);
        break;

        case IS_SCREEN_3:
        CFEScreenFX::FadeOut(1.0f);
        break;
    }
}
//-----------------------------------------------------------------------------
void OnEnterState(EIntroState _eState)
{
    switch ( _eState)
    {
        case IS_SCREEN_1:
        CFEScreenFX::SetBackground(globals->m_hBackground[0],CFEColor(1,1,1,1));
        CFEScreenFX::FadeIn(2.0f);
        break;

        case IS_SCREEN_2:
        CFEScreenFX::SetBackground(globals->m_hBackground[1],CFEColor(1,1,1,1));
        CFEScreenFX::FadeIn(2.0f);
        break;

        case IS_SCREEN_3:
        CFEScreenFX::SetBackground(globals->m_hBackground[2],CFEColor(1,1,1,1));
        CFEScreenFX::FadeIn(2.0f);
        break;

        case IS_FINISH_INTRO:
        CFEScreenFX::FadeIn(4.0f);
        break;
    }
}
//-----------------------------------------------------------------------------
void ChangeState(EIntroState _eNewState)
{
    OnExitState(globals->m_eIntroState);
    OnEnterState(_eNewState);

    globals->m_eIntroState = _eNewState;
}
//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
// -----------------------------------------------------------------------------
void Update(FEReal _rDeltaT)
{
	globals->m_oInput.Update(_rDeltaT);

    switch (globals->m_eIntroState)
    {
        case IS_INIT_INTRO:
        {
            ChangeState(IS_SCREEN_1);
        }
        break;

        case IS_SCREEN_1:
        {
            if ((CFEScreenFX::bPlaying() == false) && (globals->m_oInput.rCursorPressure(IPB_A)>0.0f))
            {
                ChangeState(IS_SCREEN_1_OUT);
            }
        }
        break;

        case IS_SCREEN_1_OUT:
        {
            if (CFEScreenFX::bPlaying() == false)
            {
                ChangeState(IS_SCREEN_2);
            }
        }
        break;

        case IS_SCREEN_2:
        {
            if ((CFEScreenFX::bPlaying() == false) && (globals->m_oInput.rCursorPressure(IPB_A)>0.0f))
            {
                ChangeState(IS_SCREEN_2_OUT);
            }
        }
        break;

        case IS_SCREEN_2_OUT:
        {
            if (CFEScreenFX::bPlaying() == false)
            {
                ChangeState(IS_SCREEN_3);
            }
        }
        break;

        case IS_SCREEN_3:
        {
            if ((CFEScreenFX::bPlaying() == false) && (globals->m_oInput.rCursorPressure(IPB_A)>0.0f))
            {
                ChangeState(IS_SCREEN_3_OUT);
            }
        }
        break;

        case IS_SCREEN_3_OUT:
        {
            if (CFEScreenFX::bPlaying() == false)
            {
                ChangeState(IS_SCREEN_1);
            }
        }
        break;
    }

    CFEScreenFX::Update(_rDeltaT);
}
// -----------------------------------------------------------------------------
void Render()
{
	globals->m_oRender.SetBlendMode(BM_ALPHA);
    globals->m_oRender.BeginScene();

		globals->m_oRender.SetMaterial( NULL );
		globals->m_oRender.RenderQuad(-10,-10,globals->m_oRender.uiGetScreenVWidth()+10,globals->m_oRender.uiGetScreenVHeight()+10,CFEColor(_0r,_1r,_0r,_1r));

		CFEScreenFX::Render( &globals->m_oRender );

    globals->m_oRender.EndScene();
}
//-----------------------------------------------------------------------------
void Loop()
{
	FEReal rDeltaT = globals->m_oRender.rGetDeltaT();

	Update(rDeltaT);
	Render();
}
// -----------------------------------------------------------------------------
