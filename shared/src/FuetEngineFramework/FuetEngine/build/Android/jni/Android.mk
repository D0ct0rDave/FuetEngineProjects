LOCAL_PATH := $(call my-dir)

# Define vars for library that will be build statically.
include $(CLEAR_VARS) 

LOCAL_MODULE    := fuetengine

# Optional compiler flags.
LOCAL_CFLAGS    := -Wall -D_DEBUG -DANDROID -Dvsprintf_s=vsnprintf -Dsprintf_s=snprintf -Dstrcat_s(a,b,c)=strcat(a,c) -Dsscanf_s=snscanf

LOCAL_C_INCLUDES := /CoderChild/shared/src/FuetEngineFramework/FuetEngine/src

LOCAL_SRC_FILES :=\
../../../src/core/Android/CFECGlobals.cpp\
../../../src/core/common/CBinConfigFile.cpp\
../../../src/core/common/CFECBinAndLibConfig.cpp\
../../../src/core/common/CFECBinConfig.cpp\
../../../src/core/common/CFECFreeImage.cpp\
../../../src/core/common/CFECLibConfig.cpp\
../../../src/core/common/CFECOpenAL.cpp\
../../../src/core/common/CFECPhysixFSFileSys.cpp\
../../../src/core/common/CFECStdFileSys.cpp\
../../../src/core/common/CFECStdLocal.cpp\
../../../src/core/common/CFECStdMath.cpp\
../../../src/core/common/CFECStdMem.cpp\
../../../src/core/common/CFECStdProfile.cpp\
../../../src/core/common/CFECStdString.cpp\
../../../src/core/common/CFERenderList.cpp\
../../../src/core/common/CFECImageLib.cpp\
../../../src/game/cinematic/CFECinematic.cpp\
../../../src/game/cinematic/CFECinematicLoader.cpp\
../../../src/game/cinematic/CFECinematicMgr.cpp\
../../../src/game/classes/CFEStatedObject.cpp\
../../../src/game/script/CFEScript.cpp\
../../../src/game/script/CFEScriptLoader.cpp\
../../../src/game/script/CFEScriptMgr.cpp\
../../../src/game/script/action/CFEAction.cpp\
../../../src/game/script/action/CFEActionLoader.cpp\
../../../src/game/script/action/CFEActionPlaySound.cpp\
../../../src/game/script/action/CFEActionSleep.cpp\
../../../src/game/script/action/CFEActionStopSound.cpp\
../../../src/game/script/action/CFEActionWaitInput.cpp\
../../../src/graphics/font/CFEFont.cpp\
../../../src/graphics/font/CFEFontANSI.cpp\
../../../src/graphics/font/CFEFontLoader.cpp\
../../../src/graphics/font/CFEFontMgr.cpp\
../../../src/graphics/font/CFEFontUTF8.cpp\
../../../src/graphics/HUD/CFEHUD.cpp\
../../../src/graphics/HUD/CFEHUDAction.cpp\
../../../src/graphics/HUD/CFEHUDActionDefaultValues.cpp\
../../../src/graphics/HUD/CFEHUDActionTime.cpp\
../../../src/graphics/HUD/CFEHUDElement.cpp\
../../../src/graphics/HUD/CFEHUDElemLocator.cpp\
../../../src/graphics/HUD/CFEHUDEventFunc.cpp\
../../../src/graphics/HUD/CFEHUDEventMgr.cpp\
../../../src/graphics/HUD/CFEHUDGroup.cpp\
../../../src/graphics/HUD/CFEHUDIcon.cpp\
../../../src/graphics/HUD/CFEHUDInstancer.cpp\
../../../src/graphics/HUD/CFEHUDInstMgr.cpp\
../../../src/graphics/HUD/CFEHUDLabel.cpp\
../../../src/graphics/HUD/CFEHUDLoader.cpp\
../../../src/graphics/HUD/CFEHUDPSys.cpp\
../../../src/graphics/HUD/CFEHUDRect.cpp\
../../../src/graphics/HUD/CFEHUDRectGen.cpp\
../../../src/graphics/HUD/CFEHUDRenderer.cpp\
../../../src/graphics/HUD/CFEHUDShape.cpp\
../../../src/graphics/HUD/CFEHUDTranslator.cpp\
../../../src/graphics/HUD/CFEHUDUpdater.cpp\
../../../src/graphics/HUD/CFEHUDVisitor.cpp\
../../../src/graphics/map/CFEMap.cpp\
../../../src/graphics/map/CFEMapLoader.cpp\
../../../src/graphics/map/CFEMapRenderer.cpp\
../../../src/graphics/mesh/CFEMesh.cpp\
../../../src/graphics/mesh/CFEMeshInstMgr.cpp\
../../../src/graphics/mesh/CFEMeshLoader.cpp\
../../../src/graphics/mesh/CFEMeshMgr.cpp\
../../../src/graphics/mesh/CFEMeshRenderer.cpp\
../../../src/graphics/particlesys/CFEParticleMgr.cpp\
../../../src/graphics/particlesys/CFEParticleSysInstMgr.cpp\
../../../src/graphics/particlesys/CFEParticleSysLoader.cpp\
../../../src/graphics/particlesys/CFEParticleSysMgr.cpp\
../../../src/graphics/screenutils/CFEScreenFX.cpp\
../../../src/graphics/screenutils/CFEScreenUtils.cpp\
../../../src/graphics/skelanim/CFESkelAnim.cpp\
../../../src/graphics/skelanim/CFESkelAnimGroup.cpp\
../../../src/graphics/skelanim/CFESkelAnimInstancer.cpp\
../../../src/graphics/skelanim/CFESkelAnimInstMgr.cpp\
../../../src/graphics/skelanim/CFESkelAnimLoader.cpp\
../../../src/graphics/skelanim/CFESkelAnimMesh.cpp\
../../../src/graphics/skelanim/CFESkelAnimMeshModel.cpp\
../../../src/graphics/skelanim/CFESkelAnimMgr.cpp\
../../../src/graphics/skelanim/CFESkelAnimNodeInstancer.cpp\
../../../src/graphics/skelanim/CFESkelAnimNodeLocator.cpp\
../../../src/graphics/skelanim/CFESkelAnimNodeTableBuilder.cpp\
../../../src/graphics/skelanim/CFESkelAnimNodeUpdater.cpp\
../../../src/graphics/skelanim/CFESkelAnimNodeVisitor.cpp\
../../../src/graphics/skelanim/CFESkelAnimRenderer.cpp\
../../../src/graphics/skelanim/CFESkelAnimSprite.cpp\
../../../src/graphics/skelanim/CFESkelAnimSpriteModel.cpp\
../../../src/graphics/skelanim/CFESkelAnimUpdater.cpp\
../../../src/graphics/skelanim/CFESkelAnimUtils.cpp\
../../../src/graphics/sprite/CFESprite.cpp\
../../../src/graphics/sprite/CFESpriteInstMgr.cpp\
../../../src/graphics/sprite/CFESpriteLoader.cpp\
../../../src/graphics/sprite/CFESpriteMgr.cpp\
../../../src/graphics/sprite/CFESpriteRenderer.cpp\
../../../src/support/config/CFEConfigFile.cpp\
../../../src/support/file/CFEFile.cpp\
../../../src/support/file/CFEMemFile.cpp\
../../../src/support/graphics/CFEMaterialMgr.cpp\
../../../src/support/graphics/CFERenderer.cpp\
../../../src/support/input/CFEInput.cpp\
../../../src/support/local/CFEDictionary.cpp\
../../../src/support/local/CFELocal.cpp\
../../../src/support/math/CFEMath.cpp\
../../../src/support/math/CFEMathIntersect.cpp\
../../../src/support/math/CFERandomGen.cpp\
../../../src/support/math/CFESmoother.cpp\
../../../src/support/mem/CFEMem.cpp\
../../../src/support/mem/CFEResourceContextMgr.cpp\
../../../src/support/misc/CFEChrono.cpp\
../../../src/support/misc/CFELogger.cpp\
../../../src/support/misc/CFEStringUtils.cpp\
../../../src/support/profile/CFEProfile.cpp\
../../../src/support/sound/CFESound.cpp\
../../../src/support/sound/CFESoundMgr.cpp\
../../../src/support/sound/CFESoundMixer.cpp\
../../../src/support/sound/CFESoundPlayer.cpp\
../../../src/support/system/CFESystem.cpp\
../../../src/support/TRC/CFETRC.cpp\
../../../src/types/CFEArray.cpp\
../../../src/types/CFEColor.cpp\
../../../src/types/CFEHashTable.cpp\
../../../src/types/CFEKFBFunc.cpp\
../../../src/types/CFEKFBFuncUtils.cpp\
../../../src/types/CFEResourceQueue.cpp\
../../../src/types/CFESingleton.cpp\
../../../src/types/CFEString.cpp\
../../../src/types/CFEVect2.cpp\
../../../src/types/FEKFBFLerpFuncs.cpp\
../../../src/core/Android/jnihelper.cpp\
../../../src/core/Android/CFECAndroid.cpp\
../../../src/core/Android/CFECAndroidInput.cpp\
../../../src/core/Android/CFECAndroidSound.cpp\
../../../src/core/Android/esShader.cpp\
../../../src/core/Android/esTransform.cpp\
../../../src/core/Android/CFECAndroidOpenGL.cpp\

include $(BUILD_STATIC_LIBRARY)
