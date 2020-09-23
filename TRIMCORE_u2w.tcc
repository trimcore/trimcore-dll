#ifndef TRIMCORE_DLL_U2W_TCC
#define TRIMCORE_DLL_U2W_tCC

inline std::wstring TRIMCORE::u2w (std::string_view sv) {
    return TRIMCORE::u2w (sv.data (), sv.size ());
}
inline std::wstring TRIMCORE::u2w (std::u8string_view sv) {
    return TRIMCORE::u2w (sv.data (), sv.size ());
}

inline std::wstring TRIMCORE::u2w (const std::string & s) {
    return TRIMCORE::u2w (s.data (), s.size ());
}
inline std::wstring TRIMCORE::u2w (const std::u8string & s) {
    return TRIMCORE::u2w (s.data (), s.size ());
}

inline std::wstring TRIMCORE::u2w (const char8_t * string) {
    return TRIMCORE::u2w ((const char *) string);
}
inline std::wstring TRIMCORE::u2w (const char * string) {
    std::wstring s;
    if (string) {
        auto n = MultiByteToWideChar (CP_UTF8, 0, string, -1, NULL, 0);
        if (n > 1) {
            s.resize (n - 1); // do not duplicated NUL-terminator
            MultiByteToWideChar (CP_UTF8, 0, string, -1, &s [0], n);
        }
    }
    return s;
}

inline std::wstring TRIMCORE::u2w (const char8_t * string, std::size_t length) {
    return TRIMCORE::u2w ((const char *) string, length);
}
inline std::wstring TRIMCORE::u2w (const char * string, std::size_t length) {
    std::wstring s;
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto n = MultiByteToWideChar (CP_UTF8, 0, string, (int) length, NULL, 0);
    if (n > 0) {
        s.resize (n);
        MultiByteToWideChar (CP_UTF8, 0, string, (int) length, &s [0], (int) n);
    }
    return s;
}

#endif
