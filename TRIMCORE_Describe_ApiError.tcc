#ifndef TRIMCORE_DLL_DESCRIBE_TCC
#define TRIMCORE_DLL_DESCRIBE_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY DescApiError (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, DWORD error) noexcept;
}

inline std::wstring TRIMCORE::Describe (ApiError error, DescriptionFormatting * format) {
    if (format) {
        format->quoted = false;
    }
    Temporary64kB <wchar_t> buffer;
    Implementation::DescApiError (buffer.data (), buffer.size (), format, error.code);
    return Describe (buffer.data (), format);
}

#endif
