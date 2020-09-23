#ifndef TRIMCORE_DLL_RSRC_STRING_H
#define TRIMCORE_DLL_RSRC_STRING_H

#include <cstdint>
#include <string_view>

namespace TRIMCORE::Rsrc {
    class StringTable {
        // TODO: something like Rsrc::Raw, inherit, fast access
    };

    // String
    //  - same functionality as LoadStringW, but doesn't inadvertly load USER32.dll and GDI32.dll
    //  - does not copy, returned view points to read-only EXE .rsrc section
    //
    inline std::wstring_view String (unsigned int id, LANGID = 0) noexcept;

    // String
    //  - same functionality as LoadStringW, but doesn't inadvertly load USER32.dll and GDI32.dll
    //  - does not copy, returned view points to read-only EXE .rsrc section
    //
    inline std::wstring_view String (HMODULE, unsigned int, LANGID = 0) noexcept;
}

#include "TRIMCORE_Rsrc_StringTable.tcc"
#endif
