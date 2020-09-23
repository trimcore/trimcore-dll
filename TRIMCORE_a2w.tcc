#ifndef TRIMCORE_DLL_A2W_TCC
#define TRIMCORE_DLL_A2W_tCC

inline std::size_t TRIMCORE::a2w (wchar_t * buffer, std::size_t length, const char * s) noexcept {
    if (s) {
        auto n = MultiByteToWideChar (CP_ACP, 0, s, -1, buffer, (int) length);
        if (n > 0)
            return n - 1; // don't count NUL-terminator
    }
    return 0;
}
inline std::size_t TRIMCORE::a2w (wchar_t * buffer, std::size_t length, const char * s, std::size_t n) noexcept {
    // TODO: call in loop for strings longer than INT_MAX
    if (n > INT_MAX) {
        n = INT_MAX;
    }
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto r = MultiByteToWideChar (CP_ACP, 0, s, (int) n, buffer, (int) length);
    if (r > 0)
        return r;
    else
        return 0;
}

inline std::size_t TRIMCORE::a2w (wchar_t * buffer, std::size_t length, std::string_view sv) noexcept {
    return TRIMCORE::a2w (buffer, length, sv.data (), sv.size ());
}
inline std::size_t TRIMCORE::a2w (wchar_t * buffer, std::size_t length, const std::string & s) noexcept {
    return TRIMCORE::a2w (buffer, length, s.data (), s.size ());
}

inline std::wstring TRIMCORE::a2wSzWithReserve (const char * string, std::size_t extra) {
    std::wstring s;
    if (string) {
        auto n = MultiByteToWideChar (CP_ACP, 0, string, -1, NULL, 0);
        if (n > 1) {
            s.reserve (n - 1 + extra);
            s.resize (n - 1); // do not duplicated NUL-terminator
            MultiByteToWideChar (CP_ACP, 0, string, -1, &s [0], n);
        }
    }
    return s;
}
inline std::wstring TRIMCORE::a2w (const char * string) {
    return a2wSzWithReserve (string, 0);
}
inline std::wstring TRIMCORE::a2w (const char * string, std::size_t length, std::size_t extra) {
    std::wstring s;
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto n = MultiByteToWideChar (CP_ACP, 0, string, (int) length, NULL, 0);
    if (n > 0) {
        s.reserve (n + extra);
        s.resize (n);
        MultiByteToWideChar (CP_ACP, 0, string, (int) length, &s [0], (int) n);
    }
    return s;
}

inline std::wstring TRIMCORE::a2w (std::string_view sv) {
    return TRIMCORE::a2w (sv.data (), sv.size ());
}
inline std::wstring TRIMCORE::a2w (const std::string & s) {
    return TRIMCORE::a2w (s.data (), s.size ());
}

inline TRIMCORE::Temporary64kB <wchar_t> TRIMCORE::a2w64kB (const char * string) {
    Temporary64kB <wchar_t> buffer;
    a2w (buffer.data (), buffer.size (), string);
    return buffer;
}
inline TRIMCORE::Temporary64kB <wchar_t> TRIMCORE::a2w64kB (std::string_view string) {
    Temporary64kB <wchar_t> buffer;
    buffer [a2w (buffer.data (), buffer.size () - 1, string)] = L'\0';
    return buffer;
}
inline TRIMCORE::Temporary64kB <wchar_t> TRIMCORE::a2w64kB (const std::string & string) {
    return a2w64kB (std::string_view (string));
}
inline TRIMCORE::Temporary64kB <wchar_t> TRIMCORE::a2w64kB (const char * string, std::size_t length) {
    return a2w64kB (std::string_view (string, length));
}


#endif
