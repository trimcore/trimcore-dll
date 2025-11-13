#ifndef TRIMCORE_DLL_A2W_H
#define TRIMCORE_DLL_A2W_H

#include <windows.h>
#include <string>
#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif
#include <limits>

namespace TRIMCORE {

    // a2w
    //  - converts CP_ACP string to buffer/length, returns number of characters written
    
    inline std::size_t a2w (wchar_t * buffer, std::size_t length, const char *) noexcept;
    inline std::size_t a2w (wchar_t * buffer, std::size_t length, std::string_view) noexcept;
    inline std::size_t a2w (wchar_t * buffer, std::size_t length, const std::string &) noexcept;
    inline std::size_t a2w (wchar_t * buffer, std::size_t length, const char *, std::size_t) noexcept;

    // a2w
    //  - returns resulting string as std::wstring

    inline std::wstring a2w (const char *);
    inline std::wstring a2w (std::string_view);
    inline std::wstring a2w (const std::string &);
    inline std::wstring a2w (const char *, std::size_t, std::size_t extra = 0);
    inline std::wstring a2wSzWithReserve (const char *, std::size_t extra);

    // a2w64kB
    //  - calls a2w over temporary 64kB buffer
    
    inline Temporary64kB <wchar_t> a2w64kB (const char *);
    inline Temporary64kB <wchar_t> a2w64kB (std::string_view);
    inline Temporary64kB <wchar_t> a2w64kB (const std::string &);
    inline Temporary64kB <wchar_t> a2w64kB (const char *, std::size_t);
}

#include "TRIMCORE_a2w.tcc"
#endif
