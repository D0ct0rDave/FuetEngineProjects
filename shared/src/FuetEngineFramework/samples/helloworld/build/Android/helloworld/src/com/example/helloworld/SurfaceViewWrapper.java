package com.example.helloworld;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;

public class SurfaceViewWrapper extends GLSurfaceView
{
	SurfaceViewWrapper(Context context)
	{
        super(context);
        setEGLContextClientVersion(2); // This is the important line

        if (isProbablyEmulator())
        {
            // Avoids crashes on startup with some emulator images.
            setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        }

        // renderer = new RendererWrapper();
        // setRenderer(renderer);
    }

    private boolean isProbablyEmulator() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                        // || Build.FINGERPRINT.startsWith("unknown")
                        || Build.MODEL.contains("google_sdk")
                        || Build.MODEL.contains("Emulator")
                        || Build.MODEL.contains("Android SDK built for x86"));
    }
}
