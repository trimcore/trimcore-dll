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
    TRIMCORE::u2w (s, string);
    return s;
}
inline std::wstring TRIMCORE::u2w (const char8_t * string, std::size_t length) {
    return TRIMCORE::u2w ((const char *) string, length);
}
inline std::wstring TRIMCORE::u2w (const char * string, std::size_t length) {
    std::wstring s;
    TRIMCORE::u2w (s, string, length);
    return s;
}

inline bool TRIMCORE::u2w (std::wstring & s, std::string_view sv) {
    return TRIMCORE::u2w (s, sv.data (), sv.size ());
}
inline bool TRIMCORE::u2w (std::wstring & s, std::u8string_view sv) {
    return TRIMCORE::u2w (s, sv.data (), sv.size ());
}

inline bool TRIMCORE::u2w (std::wstring & r, const std::string & s) {
    return TRIMCORE::u2w (r, s.data (), s.size ());
}
inline bool TRIMCORE::u2w (std::wstring & r, const std::u8string & s) {
    return TRIMCORE::u2w (r, s.data (), s.size ());
}

inline bool TRIMCORE::u2w (std::wstring & s, const char8_t * string) {
    return TRIMCORE::u2w (s, (const char *) string);
}
inline bool TRIMCORE::u2w (std::wstring & s, const char * string) {
    if (string) {
        if (string [0] == L'\0') {
            s.clear ();
            return true;
        }
        auto n = MultiByteToWideChar (CP_UTF8, 0, string, -1, NULL, 0) - 1; // do not duplicated NUL-terminator
        if (n > 0) {
            s.resize (n);
            return MultiByteToWideChar (CP_UTF8, 0, string, -1, &s [0], n + 1);
        }
    }
    return false;
}
inline bool TRIMCORE::u2w (std::wstring & s, const char8_t * string, std::size_t length) {
    return TRIMCORE::u2w (s, (const char *) string, length);
}
inline bool TRIMCORE::u2w (std::wstring & s, const char * string, std::size_t length) {
    if (length == 0) {
        s.clear ();
        return true;
    }
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto n = MultiByteToWideChar (CP_UTF8, 0, string, (int) length, NULL, 0);
    if (n > 0) {
        s.resize (n);
        return MultiByteToWideChar (CP_UTF8, 0, string, (int) length, &s [0], (int) n);
    } else
        return false;
}

#endif
