// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

    FSM_NUM_MODES

} EFEFileSeekMode;

/// File open modes
typedef enum
{
    FOM_NONE,

    FOM_READ,
    FOM_WRITE,

    FOM_NUM_MODES

}EFEFileOpenMode;

/// Horizontal alignment modes to draw text
typedef enum
{
    THAM_NONE,

    THAM_LEFT,
    THAM_CENTER,
    THAM_RIGHT,

    TAM_NUM_MODES

}EFETextHAlignmentMode;

/// Vertical alignment modes to draw text
typedef enum
{
    TVAM_NONE,

    TVAM_TOP,
    TVAM_CENTER,
    TVAM_BOTTOM,

    TVM_NUM_MODES

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

    BM_NUM_MODES

}EFEBlendMode;

/// Material Pixel Formats
typedef enum{

    PF_NONE,

    PF_GRAY,
    PF_RGBA16,
    PF_RGB24,
    PF_RGBA32,

    PF_NUM_FORMATS

}EFEPixelFormat;

/// Sound Sample Formats
typedef enum
{
    SF_NONE,

    SF_8MONO,
    SF_8STEREO,
    SF_16MONO,
    SF_16STEREO,

    SF_NUM_FORMATS

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

	IB_NUM_BUTTONS

}EFEInputButton;

// Input axis
typedef enum
{
	IA_X,
	IA_Y,
	IA_Z,
	IA_W,

	IA_NUMAXIS

}EFEInputAxis;

// Input pressure buttons
typedef enum
{
	IPB_A,
	IPB_B,
	IPB_C,

	IPB_NUMIPBS

}EFEInputPressureButton;

/// Sound mixer lines
typedef enum
{
    SML_NONE,

    SML_VOICE,
    SML_BGM,
    SML_FX,
    FSM_ENVIRONMENT,

    SML_NUM_LINES

} EFESoundMixerLine;

/// Memory allocation policies
typedef enum{

    MP_NONE,
    
    MP_SYSTEM_ALLOCATION,
    MP_STATIC_ALLOCATION,
    MP_DYNAMIC_ALLOCATION,
    MP_HARDWARE_ALLOCATION,
    
    MP_NUM_MEM_ALLOC_POLICIES

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
    
    MA_NUM_ALIGNMENTS
    
}EFEMemAlignment;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
