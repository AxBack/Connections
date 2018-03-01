#include <jni.h>

#include "electric/engine.h"

#include <map>
#include <cpu-features.h>

int id = 0;

std::map<int, Engine::Engine*> engines;

extern "C" {

JNIEXPORT jint JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_create(JNIEnv* pEnv, jobject /*thiz*/, jstring type,
												  jstring internalFile, jobject assetManager)
{
	if(android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
			(android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON))
	{
		NEON_AVAILABLE = true;
	}

    const char* str = pEnv->GetStringUTFChars(type, (jboolean*)0);
	const char* internalFilePath = pEnv->GetStringUTFChars(internalFile, (jboolean*)0);
    Engine::Engine* pEngine = nullptr;
    if(strcmp(str, "Electric") == 0)
        pEngine = new Electric::ElectricEngine;
    else
        return -1;

	AAssetManager* pAssetManager = AAssetManager_fromJava(pEnv, assetManager);
	if(!pEngine->init(internalFilePath, pAssetManager))
	{
		SAFE_DELETE(pEngine);
		return -1;
	}

	engines[id] = pEngine;
	int i = id;
	++id;
	return static_cast<jint>(i);
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_reset(JNIEnv* pEnv, jobject /*thiz*/, jint id, jstring internalFile)
{
	const char* internalFilePath = pEnv->GetStringUTFChars(internalFile, (jboolean*)0);

	auto it = engines.find(id);
	if(it == engines.end())
		return;

	it->second->reset(internalFilePath);
}


JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_destroy(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it == engines.end())
		return;

	SAFE_DELETE(it->second);
	engines.erase(it);
	LOGI("Engine destroyed");
}

JNIEXPORT jboolean JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_render(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		return static_cast<jboolean>(it->second->render());
	else
		LOGI("No Engine to render");
	return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_resume(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		engines[id]->resume();
	else
		LOGE("No Engine to resume");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_pause(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->pause();
	else
		LOGE("No Engine to pause");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_setSize(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, int w, int h)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->updateSize(w,h);
	else
		LOGE("No Engine to set size to");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_onTouch(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, float x, float y)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->touch(x,y);
	else
		LOGE("No Engine to touch");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_onPinch(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, float diff)
{
	auto it = engines.find(id);
	if(it != engines.end())
		it->second->pinch(diff);
	else
		LOGE("No Engine to touch");
}

JNIEXPORT void JNICALL
Java_com_wallpaper_axb_engine_NativeEngine_onOffsetChanged(JNIEnv* /*pEnv*/, jobject /*thiz*/, jint id, float x, float y)
{
    auto it = engines.find(id);
    if(it != engines.end())
        it->second->setOffset(x,y);
    else
        LOGE("No Engine to touch");
}

}