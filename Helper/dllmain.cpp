//
// Created by Ross on 25/08/2026.
//

#include "dllmain.h"

PBYTE CastJByteArrayToPBYTE(JNIEnv* env, jbyteArray jByteArray) {
    jbyte* jBytes = env->GetByteArrayElements(jByteArray, nullptr);
    jsize length = env->GetArrayLength(jByteArray);

    PBYTE pBytes = new BYTE[length];
    memcpy(pBytes, jBytes, length);

    env->ReleaseByteArrayElements(jByteArray, jBytes, JNI_ABORT);

    return pBytes;
}

BOOL Inject(PBYTE dllBuffer, SIZE_T dllBufferSize, DWORD processId, int port)
{
    HANDLE processHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
    std::cout << "\nProcess handle == null? : " << std::boolalpha << (processHandle == NULL);
    HANDLE threadHandle = LoadRemoteLibraryR(processHandle, dllBuffer, (DWORD)dllBufferSize, (LPVOID)port);
    std::cout << "\nThread handle == null? : " << std::boolalpha << (threadHandle == NULL);

    return CloseHandle(processHandle) && threadHandle != NULL;
}

extern "C"
JNIEXPORT jboolean JNICALL Java_club_rnss_launcher_injection_Natives_inject(JNIEnv *env, jobject thisObject, jint pid, jbyteArray dll, jint port) {
    PBYTE cppArray = CastJByteArrayToPBYTE(env, dll);

    SIZE_T size = env->GetArrayLength(dll);

    std::cout << "\n Size: " << size;
    std::cout << "\n Port: " << static_cast<int>(port);
    std::cout << "\n PID: " << static_cast<int>(pid);

    BOOL success = Inject(cppArray, size, static_cast<DWORD>(pid), static_cast<int>(port));

    delete[] cppArray;

    return static_cast<jboolean>(success);
}

