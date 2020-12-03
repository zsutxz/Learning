    #ifndef __NativeCode_H__
    #define __NativeCode_H__
    #if 0
    #define EXPORT_DLL __declspec(dllexport) //导出dll声明
    #else
    #define EXPORT_DLL 
    #endif
    extern "C" {
        EXPORT_DLL int MyAddFunc(int _a, int _b);
    }
    #endif