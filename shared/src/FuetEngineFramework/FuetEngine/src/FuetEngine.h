/*! \mainpage FuetEngine Documentation.
 * \section intro_sec FuetEngine
 * Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *
 * 2009-2011
 *
 * FuetEngine License.
 * \htmlonly
 * <center><img src="fuetengine_logo.png"></center>
 * <a href="todo.html">TODO List for the project.</a>
 * \endhtmlonly
 *
 *! \class FuetEngine
 *  \brief Main include file for FuetEngine
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FuetEngineH
#define FuetEngineH
// ----------------------------------------------------------------------------

/// Basic types exposed by the engine.
#include "FEBasicTypes.h"

/// Enumerations exposed by the engine.
#include "FEEnums.h"

/// Constants exposed by the engine.
#include "FEConsts.h"

/// Other types.
#include "types/CFEArray.h"
#include "types/CFEMatrix.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "types/CFERect.h"
#include "types/CFESegment.h"
#include "types/CFEString.h"
#include "types/CFELookupDatabase.h"
#include "types/CFEHashTable.h"
#include "types/CFEResourceMgr.h"
#include "types/CFEInstanceMgr.h"
#include "types/CFENamedObject.h"
#include "types/CFEResourceQueue.h"
#include "types/CFESingleton.h"
#include "types/CFEKFBFunc.h"
#include "types/CFEKFBFuncUtils.h"

/// Configuration
#include "support/config/CFEConfigFile.h"

/// File Management
#include "support/file/CFEFile.h"

/// Graphic Rendering support
#include "support/graphics/CFERenderer.h"
#include "support/graphics/CFEMaterialMgr.h"

/// Input managing
#include "support/input/CFEInput.h"

/// Basic math support
#include "support/math/CFEMath.h"
#include "support/math/CFEMathIntersect.h"
#include "support/math/CFESmoother.h"

/// Simple Memory Management support
#include "support/mem/CFEMem.h"
#include "support/mem/CFEResourceContextMgr.h"

/// Sound support
#include "support/sound/CFESound.h"
#include "support/sound/CFESoundMgr.h"
#include "support/sound/CFESoundPlayer.h"
#include "support/sound/CFESoundMixer.h"

// Misc support
#include "support/misc/CFELogger.h"
#include "support/misc/CFEStringUtils.h"
#include "support/misc/CFEChrono.h"

/// Profiles
#include "support/profile/CFEProfile.h"

// High Level Graphic support
#include "graphics/font/CFEFont.h"
#include "graphics/font/CFEFontMgr.h"

// Sprite functionality.
#include "graphics/sprite/CFESprite.h"
#include "graphics/sprite/CFESpriteMgr.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESpriteRenderer.h"

// Map functionality.
#include "graphics/map/CFEMap.h"
#include "graphics/map/CFEMapLoader.h"
#include "graphics/map/CFEMapRenderer.h"

// Screen utils.
#include "graphics/ScreenUtils/CFEScreenUtils.h"
#include "graphics/ScreenUtils/CFEScreenFX.h"

// HUD functionality
#include "graphics/HUD/CFEHUD.h"
#include "graphics/HUD/CFEHUDAction.h"
#include "graphics/HUD/CFEHUDElement.h"
#include "graphics/HUD/CFEHUDGroup.h"
#include "graphics/HUD/CFEHUDIcon.h"
#include "graphics/HUD/CFEHUDLabel.h"
#include "graphics/HUD/CFEHUDRect.h"
#include "graphics/HUD/CFEHUDShape.h"

#include "graphics/HUD/CFEHUDInstancer.h"
#include "graphics/HUD/CFEHUDInstMgr.h"

#include "graphics/HUD/CFEHUDLoader.h"
#include "graphics/HUD/CFEHUDObject.h"
#include "graphics/HUD/CFEHUDRenderer.h"
#include "graphics/HUD/CFEHUDUpdater.h"
#include "graphics/HUD/CFEHUDVisitor.h"
#include "graphics/HUD/CFEHUDElemLocator.h"
#include "graphics/HUD/CFEHUDRectGen.h"

// Skeleton Animations.
#include "graphics/skelanim/CFESkelAnim.h"
#include "graphics/skelanim/CFESkelAnimNode.h"
#include "graphics/skelanim/CFESkelAnimGroup.h"
#include "graphics/skelanim/CFESkelAnimSpriteModel.h"
#include "graphics/skelanim/CFESkelAnimSprite.h"
#include "graphics/skelanim/CFESkelAnimBone.h"
#include "graphics/skelanim/CFESkelAnimMeshModel.h"
#include "graphics/skelanim/CFESkelAnimMesh.h"

#include "graphics/skelanim/CFESkelAnimInstMgr.h"

#include "graphics/skelanim/CFESkelAnimMgr.h"
#include "graphics/skelanim/CFESkelAnimLoader.h"
#include "graphics/skelanim/CFESkelAnimRenderer.h"
#include "graphics/skelanim/CFESkelAnimUpdater.h"
#include "graphics/skelanim/CFESkelAnimNodeUpdater.h"

#include "graphics/skelanim/CFESkelAnimNodeVisitor.h"
#include "graphics/skelanim/CFESkelAnimNodeLocator.h"
#include "graphics/skelanim/CFESkelAnimInstancer.h"
#include "graphics/skelanim/CFESkelAnimNodeInstancer.h"

// Particle Systems.
#include "graphics/particlesys/CFEParticleSys.h"
#include "graphics/particlesys/CFEParticleSysInstMgr.h"
#include "graphics/particlesys/CFEParticleMgr.h"
#include "graphics/particlesys/CFEParticleSysMgr.h"
#include "graphics/particlesys/CFEParticleSysLoader.h"

// Basic Script System.
#include "game/script/CFEScript.h"
#include "game/script/CFEScriptMgr.h"
#include "game/script/CFEScriptLoader.h"
#include "game/script/Action/CFEAction.h"
#include "game/script/Action/CFEActionLoader.h"

// Cinematic system.
#include "game/cinematic/CFECinematic.h"
#include "game/cinematic/CFECinematicLoader.h"
#include "game/cinematic/CFECinematicMgr.h"

// Game classes
#include "game/classes/CFEStatedObject.h"

/// Localization facilities
#include "support/local/CFELocal.h"
#include "support/local/CFEDictionary.h"
#include "graphics/HUD/CFEHUDTranslator.h"

/// TRC functionality
#include "support/TRC/CFETRC.h"
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

