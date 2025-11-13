#ifndef TRIMCORE_DLL_DESCRIBE_PROCESSOR_NUMBER_TCC
#define TRIMCORE_DLL_DESCRIBE_PROCESSOR_NUMBER_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescLP (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const PROCESSOR_NUMBER *) noexcept;
}

inline std::wstring TRIMCORE::Describe (const PROCESSOR_NUMBER & number, DescriptionFormatting * format) {
    if (format && format->length) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescLP (buffer.data (), buffer.size (), format, &number);
        return Describe (std::wstring_view (buffer.data (), length), format);
    } else {
        wchar_t buffer [12];
        auto length = Implementation::DescLP (buffer, sizeof buffer / sizeof buffer [0], format, &number);
        return Describe (std::wstring_view (buffer, length), format);
    }
}

#endif
