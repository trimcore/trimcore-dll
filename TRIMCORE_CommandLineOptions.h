#ifndef TRIMCORE_DLL_COMMANDLINEOPTIONS_H
#define TRIMCORE_DLL_COMMANDLINEOPTIONS_H

#include <cwchar>
#include <cwctype>
#include <string>
// #include <stdexcept>

namespace TRIMCORE {

    // MatchNthCommandLineOption
    //  - calls 'f' for every command-line parameter matching 'name' in as:
    //  - optionally skips one "/" or any number of "-"
    //  - if 'name' is followed by ":" or "=" then skips that too and passes the rest as a parameter to 'f'
    //
    template <typename F>
    std::size_t MatchNthCommandLineOption (std::size_t i, wchar_t ** argw, std::wstring_view name, F f) {
        auto skip = 0;
        switch (argw [i][0]) {
            case L'/':
                skip = 1;
                break;
            case L'-':
                while (argw [i][skip] == '-') {
                    ++skip;
                }
                break;
        }

        if (std::wcsncmp (&argw [i][skip], name.data (), name.length ()) == 0) {
            switch (argw [i][name.length () + skip]) {
                case L':':
                case L'=':
                    f (&argw [i][name.length () + skip + 1]);
                    return true;

                case L'\0':
                    f (&argw [i][name.length () + skip]);
                    return true;
            }
        }
        return false;
    }

    // MatchCommandLineOptions
    //  - 
    //
    template <typename F>
    std::size_t MatchCommandLineOptions (std::size_t argc, wchar_t ** argw, std::wstring_view name, F f) {
        std::size_t n = 0;
        for (auto i = 1uL; i < argc; ++i) {
            n += MatchNthCommandLineOption (i, argw, name, f);
        }
        return n;
    }

    // GetCommandLineOption
    //  -
    //
    inline const wchar_t * GetCommandLineOption (std::size_t argc, wchar_t ** argw, std::wstring_view name, const wchar_t * default_ = nullptr) {
        MatchCommandLineOptions (argc, argw, name,
                                 [&default_] (const wchar_t * result) { default_ = result; });
        return default_;
    }

    /*
    static inline void cmdline_option_convert (const wchar_t * p, bool & v) {
        v = true;

        if (*p) {
            switch (std::towlower (*p)) {
                case L'f':
                case L'n':
                case L'o':
                case L'x':
                    v = false;
                    return;
            }
            try {
                if (std::stold (p) == 0.0L) {
                    v = false;
                    return;
                }
            } catch (...) {}

            try {
                if (std::stoll (p, nullptr, 0) == 0LL) {
                    v = false;
                    return;
                }
            } catch (...) {}
        }
    }
    static inline void cmdline_option_convert (const wchar_t * p, int & v) { v = std::stoi (p); }
    static inline void cmdline_option_convert (const wchar_t * p, unsigned int & v) { v = std::stoul (p); }
    static inline void cmdline_option_convert (const wchar_t * p, long & v) { v = std::stol (p); }
    static inline void cmdline_option_convert (const wchar_t * p, unsigned long & v) { v = std::stoul (p); }
    static inline void cmdline_option_convert (const wchar_t * p, long long & v) { v = std::stoll (p); }
    static inline void cmdline_option_convert (const wchar_t * p, unsigned long long & v) { v = std::stoull (p); }
    static inline void cmdline_option_convert (const wchar_t * p, float & v) { v = std::stof (p); }
    static inline void cmdline_option_convert (const wchar_t * p, double & v) { v = std::stod (p); }
    static inline void cmdline_option_convert (const wchar_t * p, long double & v) { v = std::stold (p); }
    static inline void cmdline_option_convert (const wchar_t * p, std::wstring & v) { v = p; }
    // */

    // GetCommandLineOption
    //  -
    //
    //template <typename T, std::size_t N>
    //void GetCommandLineOption (unsigned long argc, wchar_t ** argw, const wchar_t (&name) [N], T & value) {
        /*if (auto p = GetCommandLineOption (argc, argw, name)) {
            try {
                cmdline_option_convert (p, value);
            } catch (const std::invalid_argument &) {
            } catch (const std::out_of_range &) {}
        }*/
    //}
}

#endif
