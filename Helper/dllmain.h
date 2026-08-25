//
// Created by Ross on 25/08/2026.
//

#ifndef HELPER_DLLMAIN_H12345
#define HELPER_DLLMAIN_H12345

#include <Windows.h>
#include "./include/rdi/LoadLibraryR.c"
#include <iostream>
#include "./include/jni.hpp"

BOOL Inject(PBYTE dllBuffer, SIZE_T dllBufferSize, DWORD processId, int port);

#endif HELPER_DLLMAIN_H12345
