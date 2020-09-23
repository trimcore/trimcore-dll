#ifndef TRIMCORE_DLL_U2W_H
#define TRIMCORE_DLL_U2W_H

#include <windows.h>
#include <string>
#include <string_view>
#include <limits>

namespace TRIMCORE {
    inline std::wstring u2w (const char *);
    inline std::wstring u2w (const char8_t *);
    inline std::wstring u2w (std::string_view);
    inline std::wstring u2w (std::u8string_view);
    inline std::wstring u2w (const std::string &);
    inline std::wstring u2w (const std::u8string &);
    inline std::wstring u2w (const char *, std::size_t);
    inline std::wstring u2w (const char8_t *, std::size_t);
}

#include "TRIMCORE_u2w.tcc"
#endif
