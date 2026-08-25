//
// Created by Ross on 04/06/2023.
//

#ifndef INJECTION_JAVA_H
#define INJECTION_JAVA_H

#include <java/jni.hpp>
#include <java/jvmti.h>

class JVMContext {
public:
    JavaVM *vm{};
    JNIEnv *jniEnv{};
    jvmtiEnv *jvmtiEnv{};

private:
    static JVMContext *instancePtr;

private:
    JVMContext();

public:
    JVMContext(const JVMContext &obj) = delete;

    static JVMContext *instance() {
        static JVMContext* instance = new JVMContext();
        return instance;
    }
};


#endif //INJECTION_JAVA_H
