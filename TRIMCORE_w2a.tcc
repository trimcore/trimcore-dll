#ifndef TRIMCORE_DLL_W2A_TCC
#define TRIMCORE_DLL_W2A_tCC

inline std::string TRIMCORE::w2a (USHORT cp, std::wstring_view sv) {
    return TRIMCORE::w2a (cp, sv.data (), sv.size ());
}

inline bool TRIMCORE::w2a (USHORT cp, std::string & s, std::wstring_view sv) {
    return TRIMCORE::w2a (cp, s, sv.data (), sv.size ());
}

inline std::string TRIMCORE::w2a (USHORT cp, const wchar_t * string) {
    std::string s;
    TRIMCORE::w2a (cp, s, string);
    return s;
}

inline std::string TRIMCORE::w2a (USHORT cp, const wchar_t * string, std::size_t length) {
    std::string s;
    TRIMCORE::w2a (cp, s, string, length);
    return s;
}

inline bool TRIMCORE::w2a (USHORT cp, std::string & s, const wchar_t * string) {
    if (string) {
        if (string [0] == L'\0') {
            s.clear ();
            return true;
        }
        auto n = WideCharToMultiByte (cp, 0, string, -1, NULL, 0, NULL, NULL) - 1; // do not duplicated NUL-terminator
        if (n > 0) {
            s.resize (n);
            return WideCharToMultiByte (cp, 0, string, -1, reinterpret_cast <LPSTR> (&s [0]), n + 1, NULL, NULL);
        }
    }
    return false;
}

inline bool TRIMCORE::w2a (USHORT cp, std::string & s, const wchar_t * string, std::size_t length) {
    if (string == nullptr)
        return false;

    if (length == 0) {
        s.clear ();
        return true;
    }
    if (length > INT_MAX) {
        length = INT_MAX;
    }
    auto n = WideCharToMultiByte (cp, 0, string, (int) length, NULL, 0, NULL, NULL);
    if (n > 0) {
        s.resize (n);
        return WideCharToMultiByte (cp, 0, string, (int) length, reinterpret_cast <LPSTR> (&s [0]), (int) n, NULL, NULL);
    } else
        return false;
}

#endif
