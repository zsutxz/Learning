#include <jni.h>
#include <string>
#include "test.h"


extern "C" JNIEXPORT jstring JNICALL
Java_com_othershe_jnitest_Mytest_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from trtr sdd C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_othershe_jnitest_Mytest_encrypt(
        JNIEnv* env,
        jobject jObj,jstring param1) {
    char *target = (char *)env->GetStringUTFChars(param1,0);
    char *result = encode(target);
    return env->NewStringUTF(result);
}