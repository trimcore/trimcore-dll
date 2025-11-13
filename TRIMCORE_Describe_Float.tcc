#ifndef TRIMCORE_DLL_DESCRIBE_FLOAT_TCC
#define TRIMCORE_DLL_DESCRIBE_FLOAT_TCC

#include <cstdint>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescF32 (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, float value) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescF64 (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, double value) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescF64L (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, long double value) noexcept;
}

inline std::wstring TRIMCORE::Describe (float number, DescriptionFormatting * format) {
    if (format && format->length) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescF32 (buffer.data (), buffer.size (), format, number);
        return std::wstring (buffer.data (), length);
    } else {
        wchar_t buffer [96];
        auto length = Implementation::DescF32 (buffer, sizeof buffer / sizeof buffer [0], format, number);
        return std::wstring (buffer, length);
    }
}

inline std::wstring TRIMCORE::Describe (double number, DescriptionFormatting * format) {
    if (format && format->length) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescF64 (buffer.data (), buffer.size (), format, number);
        return std::wstring (buffer.data (), length);
    } else {
        wchar_t buffer [384];
        auto length = Implementation::DescF64 (buffer, sizeof buffer / sizeof buffer [0], format, number);
        return std::wstring (buffer, length);
    }
}

inline std::wstring TRIMCORE::Describe (long double number, DescriptionFormatting * format) {
    if (format && format->length) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescF64L (buffer.data (), buffer.size (), format, number);
        return std::wstring (buffer.data (), length);
    } else {
        wchar_t buffer [384];
        auto length = Implementation::DescF64L (buffer, sizeof buffer / sizeof buffer [0], format, number);
        return std::wstring (buffer, length);
    }
}

#endif
