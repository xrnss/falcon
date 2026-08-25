//
// Created by Ross on 04/06/2023.
//

#include <iostream>
#include "Java.h"

JVMContext::JVMContext() {
    jsize jvmCount;

    std::cout << "JVMCONTEXT-1" << std::endl;
    fflush(stdout);

    if (JNI_GetCreatedJavaVMs(&this->vm, 1, &jvmCount) != JNI_OK || jvmCount == 0) {
        std::cout << "JVM not found (JNI)" << std::endl;
        fflush(stdout);
        return;
    }

    std::cout << "JVMCONTEXT-2" << std::endl;
    fflush(stdout);

    jint res = this->vm->GetEnv((void **)&this->jniEnv, JNI_VERSION_1_8);

    std::cout << "JVMCONTEXT-3" << std::endl;
    fflush(stdout);

    JavaVMAttachArgs args = {sizeof(JavaVMAttachArgs)};

    args.version = JNI_VERSION_1_6;
    args.name = NULL;
    args.group = NULL;

    res = this->vm->AttachCurrentThread((void **)&this->jniEnv, &args);

    std::cout << "JVMCONTEXT-4" << std::endl;
    fflush(stdout);

    if (res != JNI_OK) {
        std::cout << "Couldn't attach thread (JNI)" << std::endl;
        fflush(stdout);
        return;
    }

    std::cout << "JVMCONTEXT-5" << std::endl;
    fflush(stdout);

    res = this->vm->GetEnv((void **)&this->jvmtiEnv, JVMTI_VERSION_1_1);

    std::cout << "JVMCONTEXT-6" << std::endl;
    fflush(stdout);

    //res = this->vm->AttachCurrentThread((void **)&this->jvmtiEnv, nullptr);

    std::cout << "JVMCONTEXT-7" << std::endl;
    fflush(stdout);

    if (res != JNI_OK) {
        std::cout << "Couldn't attach thread (JVMTI)" << std::endl;
        fflush(stdout);
        return;
    }

    std::cout << "JVMCONTEXT-8" << std::endl;
    fflush(stdout);

    jvmtiCapabilities tiCapabilities = {0};
    tiCapabilities.can_tag_objects = 1;
    tiCapabilities.can_retransform_classes = 1;
    tiCapabilities.can_retransform_any_class = 1;
    tiCapabilities.can_redefine_classes = 1;
    tiCapabilities.can_redefine_any_class = 1;
    tiCapabilities.can_generate_all_class_hook_events = 1;
    this->jvmtiEnv->AddCapabilities(&tiCapabilities);

    std::cout << "JVMCONTEXT-9" << std::endl;
    fflush(stdout);
}