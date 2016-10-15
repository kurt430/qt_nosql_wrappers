#ifndef NTDLL_H
#define NTDLL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef NTSTATUS (NTAPI *PNtClose)(HANDLE);
typedef NTSTATUS (NTAPI *PNtCreateSection)(OUT PHANDLE sh, IN ACCESS_MASK acc,
  IN void * oa OPTIONAL,
  IN PLARGE_INTEGER ms OPTIONAL,
  IN ULONG pp, IN ULONG aa, IN HANDLE fh OPTIONAL);

typedef NTSTATUS (NTAPI *PNtMapViewOfSection)(IN PHANDLE sh, IN HANDLE ph,
  IN OUT PVOID *addr, IN ULONG_PTR zbits,
  IN SIZE_T cs, IN OUT PLARGE_INTEGER off OPTIONAL,
  IN OUT PSIZE_T vs, IN SECTION_INHERIT ih,
  IN ULONG at, IN ULONG pp);

typedef struct NtDll
{
    PNtCreateSection NtCreateSection;
    PNtMapViewOfSection NtMapViewOfSection;
    PNtClose NtClose;

} NtDll;

int ntdll_load_functions(struct NtDll *nt)
{
    HMODULE dll = GetModuleHandle(TEXT("ntdll.dll"));
    nt->NtCreateSection = NULL;
    nt->NtMapViewOfSection = NULL;
    nt->NtClose = NULL;
    if (dll == NULL) {
        return 1;
    }

    nt->NtCreateSection    = (PNtCreateSection)GetProcAddress(dll, ("NtCreateSection"));
    nt->NtMapViewOfSection = (PNtMapViewOfSection)GetProcAddress(dll, ("NtMapViewOfSection"));
    nt->NtClose            = (PNtClose)GetProcAddress(dll, ("NtClose"));

    if (!nt->NtCreateSection)    return 2;
    if (!nt->NtMapViewOfSection) return 2;
    if (!nt->NtClose)            return 2;

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif // NTDLL_H

