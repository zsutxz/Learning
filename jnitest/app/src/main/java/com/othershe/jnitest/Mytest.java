package com.othershe.jnitest;

public class Mytest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public native String stringFromJNI();
    public native String encrypt(String para);
}
