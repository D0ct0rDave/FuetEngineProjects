// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEEnumsH
#define FEEnumsH
// ----------------------------------------------------------------------------
/// File seek modes
typedef enum
{
    FSM_NONE,

    FSM_SET,
    FSM_CUR,
    FSM_END,

    FSM_NUM_MODES,
   	FSM_MAX = 0xffffffff // to force 32 bit enum

} EFEFileSeekMode;

/// File open modes
typedef enum
{
    FOM_NONE,

    FOM_READ,
    FOM_WRITE,

    FOM_NUM_MODES,
	FOM_MAX = 0xffffffff // to force 32 bit enum

}EFEFileOpenMode;

/// Horizontal alignment modes to draw text
typedef enum
{
    THAM_NONE,

    THAM_LEFT,
    THAM_CENTER,
    THAM_RIGHT,
		
    THAM_NUM_MODES,
	THAM_MAX = 0xffffffff, // to force 32 bit enum
  
	THAM_DEFAULT = THAM_LEFT

}EFETextHAlignmentMode;

/// Vertical alignment modes to draw text
typedef enum
{
    TVAM_NONE,

    TVAM_TOP,
    TVAM_CENTER,
    TVAM_BOTTOM,

    TVAM_NUM_MODES,
	TVAM_MAX = 0xffffffff, // to force 32 bit enum

    TVAM_DEFAULT = TVAM_TOP
	
}EFETextVAlignmentMode;

/// Rendering Blending modes
typedef enum
{
    BM_NONE,

    BM_COPY,
    BM_ALPHA,
    BM_ADD,
    BM_ALPHAADD,
    BM_MULT,

    BM_FOG,
    BM_FOGADD,

    BM_MAGICMARKER,
    BM_LIGHTMARKER,
    BM_LIGHTSABER,
    BM_REVEAL,
	BM_LUMISHADE_REVEAL,

    BM_NUM_MODES,
   	BM_MAX = 0xffffffff, // to force 32 bit enum

    BM_DEFAULT = BM_ALPHA

}EFEBlendMode;

/// Material Pixel Formats
typedef enum{

    PF_NONE,

    PF_GRAY,
    PF_RGBA16,
    PF_RGB24,
    PF_RGBA32,

    PF_NUM_FORMATS,

    // PF_MAX = 0xffffffff // to force 32 bit enum  <------ Does not work ?!?!?!?!?!?
    PF_MAX_FORMATS =0xffffffff // to force 32 bit enum

}EFEPixelFormat;

/// Sound Sample Formats
typedef enum
{
    SF_NONE,

    SF_8MONO,
    SF_8STEREO,
    SF_16MONO,
    SF_16STEREO,

    SF_NUM_FORMATS,
    SF_MAX = 0xffffffff // to force 32 bit enum

}EFESampleFormat;

/// Input buttons
typedef enum
{
	IB_UP,			// digital up.
	IB_DOWN,		// digital down.
	IB_LEFT,		// digital left.
	IB_RIGHT,		// digital right.

	IB_MIG,			// Menu in game button.

	IB_A,			// Generic buttons.
	IB_B,
	IB_C,
	IB_D,
	IB_E,
	IB_F,
	IB_G,
	IB_H,
	IB_I,
	IB_J,
	IB_K,

	IB_NUM_BUTTONS,
    IB_MAX = 0xffffffff // to force 32 bit enum

}EFEInputButton;

// Input axis
typedef enum
{
	IA_X,
	IA_Y,
	IA_Z,
	IA_W,

	IA_NUMAXIS,
    IA_MAX = 0xffffffff // to force 32 bit enum

}EFEInputAxis;

// Input pressure buttons
typedef enum
{
	IPB_A,
	IPB_B,
	IPB_C,

	IPB_NUMIPBS,
    IPB_MAX = 0xffffffff // to force 32 bit enum

}EFEInputPressureButton;

/// Sound mixer lines
typedef enum
{
    SML_NONE,

    SML_VOICE,
    SML_BGM,
    SML_FX,
    SML_ENVIRONMENT,

    SML_USER_0,
    SML_USER_1,
    SML_USER_2,
    SML_USER_3,
    SML_USER_4,
    SML_USER_5,
    SML_USER_6,
    SML_USER_7,

    SML_NUM_LINES,
    SML_MAX = 0xffffffff // to force 32 bit enum

} EFESoundMixerLine;

/// Memory allocation policies
typedef enum{

    MP_NONE,

    MP_SYSTEM_ALLOCATION,
    MP_STATIC_ALLOCATION,
    MP_DYNAMIC_ALLOCATION,
    MP_HARDWARE_ALLOCATION,
    
    MP_NUM_MEM_ALLOC_POLICIES,
    MP_MAX = 0xffffffff, // to force 32 bit enum

    MP_DEFAULT = MP_SYSTEM_ALLOCATION

}EFEMemAllocPolicy;

/// Memory alignment values
typedef enum{

    MA_NONE = 1,
    MA_2 = 2,
    MA_4 = 4,
    MA_8 = 8,
    MA_16 = 16,
    MA_32 = 32,
    MA_64 = 64,
    MA_128 = 128,
    MA_256 = 256,
    MA_512 = 512,
    MA_1024 = 1024,
    MA_2048 = 2048,
    MA_4096 = 4096,
    MA_8192 = 8192,
    MA_16384 = 16384,
    MA_32768 = 32768,
    MA_65536 = 65536,
    
    MA_NUM_ALIGNMENTS,
    MA_MAX = 0xffffffff, // to force 32 bit enum
    
    MA_DEFAULT = MA_NONE

}EFEMemAlignment;

// ------------------------------------------------------------------------
// WARNING IMPORTANT NOTE!!!!: Every time a new language is appended, we should
// add appropriate strings to Local String ID (CFECStdLocal) and SystemMessages 
// (FECDSMessages,etc)
// ------------------------------------------------------------------------

/// Local IDs
typedef enum
{
	LID_NONE = 0,
	LID_COMMON = 0,

	LID_ES,						// spanish
	LID_EN,						// english
	LID_FR,						// french
	LID_IT,						// italian
	LID_DE,						// deutch (german)
	LID_DU,						// dutch
	LID_PT,						// portuguese

	LID_US_ES,					// us spanish
	LID_US_EN,					// us english
	LID_US_FR,					// us french
	LID_US_PT,					// us portuguese

	LID_JP,						// japanish

	LID_NUM_IDs,
	LID_MAX = 0xffffffff, // to force 32 bit enum
    
    LID_DEFAULT = LID_EN,
    LID_US_DEFAULT = LID_US_EN

}EFELocalID;

// ------------------------------------------------------------------------
// WARNING IMPORTANT NOTE!!!!: Every time a new region is appended, we should
// add appropriate strings to Local String ID (CFECStdLocal)
// ------------------------------------------------------------------------
/// Local Region IDs
typedef enum
{
	LRID_NONE = 0,
	LRID_COMMON = 0,

	LRID_US,					// united states
	LRID_EU,					// Europe
	LRID_JP,					// japan
	LRID_AS,					// asia

	LRID_NUM_IDs,
	LRID_MAX = 0xffffffff,		// to force 32 bit enum

    LRID_DEFAULT = LRID_EU

}EFELocalRegionID;


// Skeleton animation node types
typedef enum 
{
	SANT_NONE,
	
	SANT_SPRITE,
	SANT_SPRITE_MODEL,
	SANT_MESH,
	SANT_MESH_MODEL,
	SANT_GROUP,
	SANT_BONE,
	
	SANT_NUM,
	SANT_MAX = 0xffffffff // to force 32 bit enum

}EFESkelAnimNodeType;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
