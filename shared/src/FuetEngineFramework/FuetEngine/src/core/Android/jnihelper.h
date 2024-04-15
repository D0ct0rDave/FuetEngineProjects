/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __ANDROID_JNI_HELPER_H__
#define __ANDROID_JNI_HELPER_H__
// ----------------------------------------------------------------------------
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/configuration.h>
#include <android/input.h>
// ----------------------------------------------------------------------------
//#include <string>
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// ----------------------------------------------------------------------------
typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;
// ----------------------------------------------------------------------------
jboolean GetEnv(JNIEnv **env);
JavaVM* GetJavaVM();
void SetJavaVM(JavaVM *javaVM);
// jclass GetClassID(const char *className, JNIEnv *env=0);
jboolean GetStaticMethodInfo(JniMethodInfo *methodinfo, const char *className, const char *methodName, const char *paramCode);
jboolean GetMethodInfo(JniMethodInfo *methodinfo, const char *className, const char *methodName, const char *paramCode);
// std::string jstring2string(jstring str);
// ----------------------------------------------------------------------------
const char* GetLocale();
void SetLocale(const char* _szLocale);

const char* GetApkPath();
void SetApkPath(const char* _szPath);

AAssetManager* GetAssetManager();
void SetAssetManager(AAssetManager* _poAssetMgr);

AConfiguration* GetConfiguration();
void SetConfiguration(AConfiguration* _poConfig);

unsigned int GetScreenWidth();
void SetScreenWidth(unsigned int _uiWidth);

unsigned int GetScreenHeight();
void SetScreenHeight(unsigned int _uiHeight);

jlong GetSystemMilis();
void SetSystemMilis(jlong _lMilis);


// Input management

// The way android defines undefined structures
struct TFEInputStruct;
typedef struct TFEInputStruct TFEInputStruct;

TFEInputStruct* GetInputStruct();

void SetInputCursorCoords(float _fX,float _fY);

void SetInputCursorPressureValue(int _iButton,float _fValue);

void SetInputButtonState(int _iButton,jboolean _bDown);

void SetInputAxisValue(int _iAxis,float _fValue);
// ----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
// ----------------------------------------------------------------------------
#endif // __ANDROID_JNI_HELPER_H__
// ----------------------------------------------------------------------------

