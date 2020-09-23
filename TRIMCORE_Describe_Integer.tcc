#ifndef TRIMCORE_DLL_DESCRIBE_INTEGER_TCC
#define TRIMCORE_DLL_DESCRIBE_INTEGER_TCC

#include <charconv>
#include <cstdint>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t DescSInt (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, std::intmax_t value) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t DescUInt (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, std::uintmax_t value) noexcept;

    inline std::wstring DescribeSInt (std::intmax_t number, DescriptionFormatting * format) {
        if (format && format->length) {
            Temporary64kB <wchar_t> buffer;
            auto length = Implementation::DescSInt (buffer.data (), buffer.size (), format, number);
            return Describe (std::wstring_view (buffer.data (), length), format);
        } else {
            wchar_t buffer [96];
            auto length = Implementation::DescSInt (buffer, sizeof buffer / sizeof buffer [0], format, number);
            return std::wstring (buffer, length);
        }
    }
    inline std::wstring DescribeUInt (std::uintmax_t number, DescriptionFormatting * format) {
        if (format && format->length) {
            Temporary64kB <wchar_t> buffer;
            auto length = Implementation::DescUInt (buffer.data (), buffer.size (), format, number);
            return Describe (std::wstring_view (buffer.data (), length), format);
        } else {
            wchar_t buffer [96];
            auto length = Implementation::DescUInt (buffer, sizeof buffer / sizeof buffer [0], format, number);
            return std::wstring (buffer, length);
        }
    }
}

inline std::wstring TRIMCORE::Describe (signed char v, DescriptionFormatting * parameter) { return Implementation::DescribeSInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed short v, DescriptionFormatting * parameter) { return Implementation::DescribeSInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed int v, DescriptionFormatting * parameter) { return Implementation::DescribeSInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed long v, DescriptionFormatting * parameter) { return Implementation::DescribeSInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed long long v, DescriptionFormatting * parameter) { return Implementation::DescribeSInt (v, parameter); }

inline std::wstring TRIMCORE::Describe (unsigned char v, DescriptionFormatting * parameter) { return Implementation::DescribeUInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned short v, DescriptionFormatting * parameter) { return Implementation::DescribeUInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned int v, DescriptionFormatting * parameter) { return Implementation::DescribeUInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned long v, DescriptionFormatting * parameter) { return Implementation::DescribeUInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned long long v, DescriptionFormatting * parameter) { return Implementation::DescribeUInt (v, parameter); }

#endif
