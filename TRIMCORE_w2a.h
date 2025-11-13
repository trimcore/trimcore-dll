#ifndef TRIMCORE_DLL_W2A_H
#define TRIMCORE_DLL_W2A_H

#include <windows.h>
#include <string>
#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif
#include <limits>

namespace TRIMCORE {

    // w2a
    //  - 

    inline std::string w2a (USHORT cp, const wchar_t *);
    inline std::string w2a (USHORT cp, const wchar_t *, std::size_t);
    inline std::string w2a (USHORT cp, std::wstring_view);

    inline bool w2a (USHORT cp, std::string &, const wchar_t *);
    inline bool w2a (USHORT cp, std::string &, const wchar_t *, std::size_t);
    inline bool w2a (USHORT cp, std::string &, std::wstring_view);

    inline std::string w2a (const wchar_t * string) { return w2a (CP_ACP, string); }
    inline std::string w2a (const wchar_t * string, std::size_t length) { return w2a (CP_ACP, string, length); }
    inline std::string w2a (std::wstring_view string) { return w2a (CP_ACP, string); };

    inline bool w2a (std::string & result, const wchar_t * string) { return w2a (CP_ACP, result, string); };
    inline bool w2a (std::string & result, const wchar_t * string, std::size_t length) { return w2a (CP_ACP, result, string, length); };
    inline bool w2a (std::string & result, std::wstring_view string) { return w2a (CP_ACP, result, string); };

    // w2ascii
    //  - 
    //  - 'invalid' is replacement character that can't be represented in ASCII
    //    - if -1 then string is terminated at that point

    std::size_t w2ascii (char * buffer, std::size_t length, const wchar_t *, std::size_t, int invalid = 127) noexcept;

}

#include "TRIMCORE_w2a.tcc"
#endif
