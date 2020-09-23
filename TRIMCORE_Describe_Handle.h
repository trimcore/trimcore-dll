#ifndef TRIMCORE_DLL_DESCRIBE_HANDLE_H
#define TRIMCORE_DLL_DESCRIBE_HANDLE_H

namespace TRIMCORE {

    // Describe HKEY
    //  - returns registry path of the key handle
    //  - supported format parameters:
    //     - "NT" - keep NT format
    // 
    inline std::wstring Describe (HKEY hKey, DescriptionFormatting * format = nullptr);

    // Describe HMODULE
    //  - generates module identifier: "7ff8000!module.dll"
    //  - supports parameters for pointer formatting and string formatting
    //  - additional supported format parameters: none
    // 
    inline std::wstring Describe (HMODULE module, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (IMAGE_DOS_HEADER * module, DescriptionFormatting * format = nullptr) {
        return Describe ((HMODULE) module, format);
    }

    // DescriptionLengthEst HKEY/HMODULE
    //  - HKEY contains whole path in either NT or WIN32 format
    //  - HMODULE - 12 or 8 for pointer, !, dll name (up to 255 characters)

    inline std::size_t DescriptionLengthEst (HKEY) { return 128; }
    inline std::size_t DescriptionLengthEst (HMODULE) { return 32; }
    inline std::size_t DescriptionLengthEst (IMAGE_DOS_HEADER * module) { return DescriptionLengthEst ((HMODULE) module); }

}

#include "TRIMCORE_Describe_Handle.tcc"
#endif
