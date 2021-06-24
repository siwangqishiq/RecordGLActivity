package com.xinlan.shaderlab;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class ShaderView extends GLSurfaceView  implements GLSurfaceView.Renderer{
    private NativeBridge mNativeBridge = new NativeBridge();

    private long lastRenderTime = -1;

    public ShaderView(Context context) {
        super(context);
        initView();
    }

    public ShaderView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public void compileAndRun(final String src){
        queueEvent(()->{
            mNativeBridge.compileAndRun(src);
        });
    }

    private void initView() {
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        setRenderer(this);

//        setRenderMode(RENDERMODE_WHEN_DIRTY);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        mNativeBridge.init();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        mNativeBridge.resize(width , height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        long deltaTime = 0;
        long currentTime = System.currentTimeMillis();
        if(lastRenderTime >= 0){
            deltaTime = currentTime - lastRenderTime;
        }
        //System.out.println("deltaTime = " + deltaTime);
        lastRenderTime = currentTime;
        mNativeBridge.render(deltaTime);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mNativeBridge.destory();
    }


}//end class
