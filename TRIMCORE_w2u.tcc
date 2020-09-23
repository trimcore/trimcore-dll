#ifndef TRIMCORE_DLL_W2U_TCC
#define TRIMCORE_DLL_W2U_tCC

inline std::u8string TRIMCORE::w2u (std::wstring_view sv) {
    return TRIMCORE::w2u (sv.data (), sv.size ());
}

inline std::u8string TRIMCORE::w2u (const wchar_t * string) {
    std::u8string s;
    if (string) {
        auto n = WideCharToMultiByte (CP_UTF8, 0, string, -1, NULL, 0, NULL, NULL);
        if (n > 1) {
            s.resize (n - 1); // do not duplicated NUL-terminator
            WideCharToMultiByte (CP_UTF8, 0, string, -1, reinterpret_cast <LPSTR> (&s [0]), n, NULL, NULL);
        }
    }
    return s;
}

inline std::u8string TRIMCORE::w2u (const wchar_t * string, std::size_t length) {
    std::u8string s;
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto n = WideCharToMultiByte (CP_UTF8, 0, string, (int) length, NULL, 0, NULL, NULL);
    if (n > 0) {
        s.resize (n);
        WideCharToMultiByte (CP_UTF8, 0, string, (int) length, reinterpret_cast <LPSTR> (&s [0]), (int) n, NULL, NULL);
    }
    return s;
}

#endif
