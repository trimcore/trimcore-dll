#ifndef TRIMCORE_DLL_DESCRIBE_TIME_TCC
#define TRIMCORE_DLL_DESCRIBE_TIME_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t DescFileTime (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const FILETIME * t) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t DescSysTime  (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const SYSTEMTIME * t) noexcept;
}

inline std::wstring TRIMCORE::Describe (const std::tm & tm, DescriptionFormatting * format) {
    SYSTEMTIME st;
    st.wYear = tm.tm_year + 1900u;
    st.wMonth = tm.tm_mon + 1u;
    st.wDay = tm.tm_mday;
    st.wDayOfWeek = tm.tm_wday;
    st.wHour = tm.tm_hour;
    st.wMinute = tm.tm_min;
    st.wSecond = tm.tm_sec;
    st.wMilliseconds = 0;
    return Describe (st, format);
}

inline std::wstring TRIMCORE::Describe (const FILETIME & ft, DescriptionFormatting * format) {
    Temporary64kB <wchar_t> buffer;
    Implementation::DescFileTime (buffer.data (), buffer.size (), format, &ft);
    return buffer.data ();
}
inline std::wstring TRIMCORE::Describe (const SYSTEMTIME & st, DescriptionFormatting * format) {
    Temporary64kB <wchar_t> buffer;
    Implementation::DescSysTime (buffer.data (), buffer.size (), format, &st);
    return buffer.data ();
}

#endif
