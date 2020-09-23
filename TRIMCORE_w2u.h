#ifndef TRIMCORE_DLL_W2U_H
#define TRIMCORE_DLL_W2U_H

#include <windows.h>
#include <string>
#include <string_view>
#include <limits>

namespace TRIMCORE {
    inline std::u8string w2u (const wchar_t *);
    inline std::u8string w2u (const wchar_t *, std::size_t);
    inline std::u8string w2u (std::wstring_view);
}

#include "TRIMCORE_w2u.tcc"
#endif
