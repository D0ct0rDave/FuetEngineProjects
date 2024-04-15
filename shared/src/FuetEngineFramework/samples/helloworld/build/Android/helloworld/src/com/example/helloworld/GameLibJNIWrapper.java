package com.example.helloworld;

import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.view.MotionEvent;

public class GameLibJNIWrapper {
	static	{
		System.loadLibrary("game");
	}

	public static native void InitEngine();

	public static native void LoopEngine();	
	
	public static native void SetAssetManager(AssetManager mgr);
	
	public static native void SetConfiguration(Configuration cfg);
	
	// public static native void SetSurface(Surface surf);	

	public static native void SetApkPath(String path);
	
	public static native void SetLocale(String locale);

	public static native void SetScreenWidth(int _iScreenWidth);
	
	public static native void SetScreenHeight(int _iScreenHeight);
	
	public static native void SetSystemMilis(long _lMilis);
	
	public static native void SetInputEvent(MotionEvent _oEvent,int _iIdx);
}
