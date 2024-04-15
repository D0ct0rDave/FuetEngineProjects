package com.example.helloworld;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.Toast;
// import android.opengl.EGLConfig;
// import android.opengl.GLSurfaceView;

public class MainActivity extends Activity
{
	private GLSurfaceView 	glSurfaceView;
	private boolean 		rendererSet;
	private AssetManager 	mgr;
	private Configuration 	cfg;
	private int				m_iCurPointer;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
	    super.onCreate(savedInstanceState);

	    ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
	    ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

	    final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();

	    if (supportsEs2)
	    {
	        glSurfaceView = new GLSurfaceView(this); 	 //new SurfaceViewWrapper(this); //
	        if (isProbablyEmulator())
	        {
	            // Avoids crashes on startup with some emulator images.
	        	glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
	        }

	        glSurfaceView.setEGLContextClientVersion(2); // This is the important line

	        glSurfaceView.setRenderer(new RendererWrapper());
	        rendererSet = true;
	        setContentView(glSurfaceView);

	        cfg = getResources().getConfiguration();
	        GameLibJNIWrapper.SetConfiguration(cfg);

	        GameLibJNIWrapper.SetLocale(cfg.locale.toString());
	        
	        mgr = getResources().getAssets();
	        GameLibJNIWrapper.SetAssetManager(mgr);

	        ApplicationInfo applicationInfo = getApplicationInfo();
	        GameLibJNIWrapper.SetApkPath(applicationInfo.sourceDir);

	        DisplayMetrics metrics = new DisplayMetrics();  
	        getWindowManager().getDefaultDisplay().getMetrics(metrics);
	        
	        GameLibJNIWrapper.SetScreenWidth( metrics.widthPixels );
			GameLibJNIWrapper.SetScreenHeight( metrics.heightPixels );
			
			m_iCurPointer = -1;
	    }
	    else
	    {
	        // Should never be seen in production, since the manifest filters
	        // unsupported devices.
	        Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
	                Toast.LENGTH_LONG).show();
	        return;
	    }
	}

    private boolean isProbablyEmulator()
    {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                        // || Build.FINGERPRINT.startsWith("unknown")
                        || Build.MODEL.contains("google_sdk")
                        || Build.MODEL.contains("Emulator")
                        || Build.MODEL.contains("Android SDK built for x86"));
    }
    /*
    @Override
    protected void onTouchEvent()
    {
  
    }
    */
    
    // @Override
    public boolean dispatchTouchEvent(MotionEvent event)
    {
    	int iEvent 		= event.getActionMasked();
    	int iIdx 		= event.getActionIndex();
    	int iPointerID  = event.getPointerId( iIdx );

    	switch (iEvent)
    	{
    		case MotionEvent.ACTION_POINTER_DOWN:
    		{
        		if (m_iCurPointer != -1)
        		{
        			m_iCurPointer = iPointerID;
        			GameLibJNIWrapper.SetInputEvent(event,iIdx);
        		}
        		else
        		{
        			// do nothing, skip action
        		}    		
    		}
    		break;

    		case  MotionEvent.ACTION_POINTER_UP:
    		{
    			if (iPointerID == m_iCurPointer)
    			{
    				m_iCurPointer = -1;
    				GameLibJNIWrapper.SetInputEvent(event,iIdx);
    			}
    			else
    			{
    				// do nothing
    			}
    		}
    		break;
    		
    		case MotionEvent.ACTION_MOVE:
    		{
    			if (iPointerID == m_iCurPointer)
    			{
    				GameLibJNIWrapper.SetInputEvent(event,iIdx);
    			}
    			else
    			{
    				// do nothing
    			}
    		}
    		break;
    	}

    	return(true);
    }
	
@Override
    protected void onPause()
    {
        super.onPause();
     
        if (rendererSet)
        {
            glSurfaceView.onPause();
        }
    }

    public String messageMe(String text)
    {
        System.out.println(text);
        return text;
    }
    
    @Override
    protected void onResume()
    {
        super.onResume();
     
        if (rendererSet)
        {
            glSurfaceView.onResume();
        }
    }
}
