#include <windows.h>
#include <stdio.h>
int main(void)
{
    HMODULE h = LoadLibraryA("libHelper.dll");

    if (!h) {
        printf("LoadLibrary failed: %lu\n", GetLastError());
        return 1;
    }

    printf("Loaded successfully\n");
    FreeLibrary(h);
    return 0;
}