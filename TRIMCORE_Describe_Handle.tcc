#ifndef TRIMCORE_DLL_DESCRIBE_HANDLE_TCC
#define TRIMCORE_DLL_DESCRIBE_HANDLE_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY DescHKEY (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, HKEY) noexcept;
    // TRIMCORE_DLL_IMPORT void WINAPI DescHANDLE (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, HANDLE) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY DescHMODULE (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, HMODULE) noexcept;
}

inline std::wstring TRIMCORE::Describe (HMODULE module, DescriptionFormatting * format) {
    wchar_t buffer [256 + 18];
    Implementation::DescHMODULE (buffer, 256 + 18, format, module);
    return Describe (buffer, format);
}
/*inline std::wstring TRIMCORE::Describe (HANDLE handle , DescriptionFormatting * format) {
    Temporary64kB <wchar_t> buffer;
    Implementation::DescHANDLE (buffer.data (), buffer.size (), format, handle);
    return Describe (buffer.data (), format);
}*/
inline std::wstring TRIMCORE::Describe (HKEY key, DescriptionFormatting * format) {
    Temporary64kB <wchar_t> buffer;
    Implementation::DescHKEY (buffer.data (), buffer.size (), format, key);
    return Describe (buffer.data (), format);
}

#endif
