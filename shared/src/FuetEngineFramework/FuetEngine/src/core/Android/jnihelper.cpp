/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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
#include "jnihelper.h"
#include <pthread.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <android/log.h>
#include <android/configuration.h>
#include "../FECoreTypes.h"
// ----------------------------------------------------------------------------
#if 1
#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...)
#endif
// ----------------------------------------------------------------------------
#define JAVAVM          m_psJavaVM
static JavaVM*          m_psJavaVM          = NULL;
static std::string      gs_sAPKPath         = "";
static std::string      gs_sLocale          = "";
static AAssetManager*   gs_poAssetMgr       = NULL;
static unsigned int     gs_uiScreenWidth    = 0;
static unsigned int     gs_uiScreenHeight   = 0;
static AConfiguration*  gs_poConfig         = NULL;
static jlong            gs_lMilis           = 0;
static TFEInputStruct   gs_oInputStruct;
static bool             gs_Initalized       = false;
// ----------------------------------------------------------------------------
static pthread_key_t s_threadKey;
// ----------------------------------------------------------------------------
static void detach_current_thread (void *env)
{
	JAVAVM->DetachCurrentThread();
}
// ----------------------------------------------------------------------------
jboolean GetEnv(JNIEnv **env)
{
	bool bRet = false;

	switch(JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4))
	{
	case JNI_OK:
		bRet = true;
		break;
	case JNI_EDETACHED:
		pthread_key_create (&s_threadKey, detach_current_thread);
		if (JAVAVM->AttachCurrentThread(env, 0) < 0)
		{
			LOGD("Failed to get the environment using AttachCurrentThread()");
			break;
		}
		if (pthread_getspecific(s_threadKey) == NULL)
			pthread_setspecific(s_threadKey, env);
		bRet = true;
		break;
	default:
		LOGD("Failed to get the environment using GetEnv()");
		break;
	}

	return bRet;
}
// ----------------------------------------------------------------------------
static jclass GetClassID_(const char *className, JNIEnv *env)
{
	JNIEnv *pEnv = env;
	jclass ret = 0;

	do
	{
		if (! pEnv)
		{
			if (! GetEnv(&pEnv))
			{
				break;
			}
		}

		ret = pEnv->FindClass(className);
		if (! ret)
		{
			 LOGD("Failed to find class of %s", className);
			break;
		}
	} while (0);

	return ret;
}
// ----------------------------------------------------------------------------
static jboolean GetStaticMethodInfo_(JniMethodInfo *methodinfo, const char *className, const char *methodName, const char *paramCode)
{
	jmethodID methodID = 0;
	JNIEnv *pEnv = 0;
	bool bRet = false;

	do
	{
		if (! GetEnv(&pEnv))
		{
			break;
		}

		jclass classID = GetClassID_(className, pEnv);

		methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
		if (! methodID)
		{
			LOGD("Failed to find static method id of %s", methodName);
			break;
		}

		methodinfo->classID = classID;
		methodinfo->env = pEnv;
		methodinfo->methodID = methodID;

		bRet = true;
	} while (0);

	return bRet;
}
// ----------------------------------------------------------------------------
static jboolean GetMethodInfo_(JniMethodInfo *methodinfo, const char *className, const char *methodName, const char *paramCode)
{
	jmethodID methodID = 0;
	JNIEnv *pEnv = 0;
	bool bRet = false;

	do
	{
		if (! GetEnv(&pEnv))
		{
			break;
		}

		jclass classID = GetClassID_(className, pEnv);

		methodID = pEnv->GetMethodID(classID, methodName, paramCode);
		if (! methodID)
		{
			LOGD("Failed to find method id of %s", methodName);
			break;
		}

		methodinfo->classID = classID;
		methodinfo->env = pEnv;
		methodinfo->methodID = methodID;

		bRet = true;
	} while (0);

	return bRet;
}
// ----------------------------------------------------------------------------
/*
static std::string jstring2string_(jstring jstr)
{
	if (jstr == NULL)
	{
		return "";
	}

	JNIEnv *env = 0;

	if (! GetEnv(&env))
	{
		return 0;
	}

	const char* chars = env->GetStringUTFChars(jstr, NULL);
	std::string ret(chars);
	env->ReleaseStringUTFChars(jstr, chars);

	return ret;
}
*/
// ----------------------------------------------------------------------------
void InitializeJNIHelper()
{
    memset(&gs_oInputStruct,0,sizeof(TFEInputStruct));
}
// ----------------------------------------------------------------------------
JavaVM* GetJavaVM()
{
    return m_psJavaVM;
}
// ----------------------------------------------------------------------------
void SetJavaVM(JavaVM *javaVM)
{
    // This is the first thing to be done on application startup
    m_psJavaVM = javaVM;
    if (! gs_Initalized)
    {
        InitializeJNIHelper();
        gs_Initalized = true;
    }
}
// ----------------------------------------------------------------------------
const char* GetLocale()
{
	return(gs_sLocale.c_str());
}
// ----------------------------------------------------------------------------
void SetLocale(const char* _szLocale)
{
    gs_sLocale = _szLocale;
}
// ----------------------------------------------------------------------------
const char* GetApkPath()
{
	return(gs_sAPKPath.c_str());
}
// ----------------------------------------------------------------------------
void SetApkPath(const char* _szPath)
{
	gs_sAPKPath = _szPath;
}
// ----------------------------------------------------------------------------
AAssetManager* GetAssetManager()
{
	return(gs_poAssetMgr);
}
// ----------------------------------------------------------------------------
void SetAssetManager(AAssetManager* _poAssetMgr)
{
	gs_poAssetMgr = _poAssetMgr;
}
// ----------------------------------------------------------------------------
unsigned int GetScreenWidth()
{
	return(gs_uiScreenWidth);
}
// ----------------------------------------------------------------------------
void SetScreenWidth(unsigned int _uiWidth)
{
	gs_uiScreenWidth = _uiWidth;
}
// ----------------------------------------------------------------------------
unsigned int GetScreenHeight()
{
	return(gs_uiScreenHeight);
}
// ----------------------------------------------------------------------------
void SetScreenHeight(unsigned int _uiHeight)
{
	gs_uiScreenHeight = _uiHeight;
}
// ----------------------------------------------------------------------------
AConfiguration* GetConfiguration()
{
    return(gs_poConfig);
}
// ----------------------------------------------------------------------------
void SetConfiguration(AConfiguration* _poConfig)
{
    gs_poConfig = _poConfig;
}
// ----------------------------------------------------------------------------
jlong GetSystemMilis()
{
    return(gs_lMilis);
}
// ----------------------------------------------------------------------------
void SetSystemMilis(jlong _lMilis)
{
    gs_lMilis = _lMilis;
}
// ----------------------------------------------------------------------------
TFEInputStruct* GetInputStruct()
{
    return(&gs_oInputStruct);
}
// ----------------------------------------------------------------------------
void SetInputCursorCoords(float _fX,float _fY)
{
    gs_oInputStruct.m_oCursor.m_oCoords.x = _fX;
    gs_oInputStruct.m_oCursor.m_oCoords.y = _fY;
}
// ----------------------------------------------------------------------------
void SetInputCursorPressureValue(int _iButton,float _fValue)
{
    gs_oInputStruct.m_oCursor.m_rPressure[_iButton] = _fValue;
}
// ----------------------------------------------------------------------------
void SetInputButtonState(int _iButton,jboolean _bDown)
{
    gs_oInputStruct.m_bButtons[_iButton] = _bDown;
}
// ----------------------------------------------------------------------------
void SetInputAxisValue(int _iAxis,float _fValue)
{
    gs_oInputStruct.m_rAxis[_iAxis] = _fValue;
}
// ----------------------------------------------------------------------------
