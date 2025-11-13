#ifndef TRIMCORE_DLL_W2U_H
#define TRIMCORE_DLL_W2U_H

#include <windows.h>
#include <string>
#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif
#include <limits>

namespace TRIMCORE {
    inline std::u8string w2u (const wchar_t *);
    inline std::u8string w2u (const wchar_t *, std::size_t);
    inline std::u8string w2u (std::wstring_view);

    inline bool w2u (std::u8string &, const wchar_t *);
    inline bool w2u (std::u8string &, const wchar_t *, std::size_t);
    inline bool w2u (std::u8string &, std::wstring_view);
}

#include "TRIMCORE_w2u.tcc"
#endif
