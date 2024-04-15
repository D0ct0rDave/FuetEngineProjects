package com.example.helloworld;

import static android.opengl.GLES20.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES20.glClear;
import static android.opengl.GLES20.glClearColor;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.os.SystemClock;

public class RendererWrapper implements Renderer
{
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		GameLibJNIWrapper.InitEngine();
		// glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		// GameLibJNIWrapper.SetScreenWidth(width);
		// GameLibJNIWrapper.SetScreenHeight(height);
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		GameLibJNIWrapper.SetSystemMilis(SystemClock.elapsedRealtime());
		GameLibJNIWrapper.LoopEngine();
	}
}