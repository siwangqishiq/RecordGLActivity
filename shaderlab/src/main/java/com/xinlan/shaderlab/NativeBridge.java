package com.xinlan.shaderlab;

public final class NativeBridge {
    static {
        System.loadLibrary("shaderlab");
    }

    public native String sayHello(String input);

    public native void init();

    public native void resize(int width , int height);

    public native void render(long deltaTime);

    public native void destory();
}
