//
// Created by Ross on 25/08/2026.
//

#ifndef LOADER_EXPORT_H
#define LOADER_EXPORT_H

#include <rdi/ReflectiveDLLInjection.h>
#include <string>
#include <socket.hpp>

#define REFLECTIVEDLLINJECTION_VIA_LOADREMOTELIBRARYR
#define REFLECTIVEDLLINJECTION_CUSTOM_DLLMAIN

HINSTANCE hInst;

DWORD WINAPI StartThread(LPVOID lpThreadParameter) {
    int port = uintptr_t(lpThreadParameter);

    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    MessageBoxA(NULL, ("Port: " + std::to_string(port)).c_str(), "MASTER DLL INJECTION SUCCESS", MB_OK | MB_ICONQUESTION);

    connect(port);

    FreeConsole();
    FreeLibraryAndExitThread(hInst, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        MessageBoxA(nullptr, "DllMain reached", "DEBUG", MB_OK);

        HANDLE tHandle = CreateThread(NULL, 0, &StartThread, lpReserved, NULL, NULL);
        CloseHandle(tHandle);
        hInst = hinst;
    }

    return TRUE;
}

#endif //LOADER_EXPORT_H
