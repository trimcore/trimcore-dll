#ifndef TRIMCORE_DLL_PROCESS_ARCHITECTURE_H
#define TRIMCORE_DLL_PROCESS_ARCHITECTURE_H

namespace TRIMCORE {
    namespace Implementation {
        TRIMCORE_DLL_IMPORT USHORT TRIMCORE_APIENTRY GetProcessArch (DWORD pid) noexcept;
        TRIMCORE_DLL_IMPORT USHORT TRIMCORE_APIENTRY GetProcessArchByHandle (HANDLE hProcess) noexcept;
    }

    // QueryProcessArchitecture
    //  -
    //
    inline USHORT QueryProcessArchitecture (DWORD pid) noexcept { return Implementation::GetProcessArch (pid); }

    // QueryProcessArchitecture
    //  - 
    //  - requires at least PROCESS_QUERY_LIMITED_INFORMATION access right
    //
    inline USHORT QueryProcessArchitecture (HANDLE h) noexcept { return Implementation::GetProcessArchByHandle (h); }
}

#endif

