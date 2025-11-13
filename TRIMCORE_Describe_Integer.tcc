#ifndef TRIMCORE_DLL_DESCRIBE_INTEGER_TCC
#define TRIMCORE_DLL_DESCRIBE_INTEGER_TCC

#include <cstdint>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescInt (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const void *, std::size_t, std::size_t flags) noexcept;

    template <typename T>
    static inline std::wstring DescribeInt (const T & number, DescriptionFormatting * format) {
        std::size_t flags = 0;

        if (std::numeric_limits <T>::is_signed) flags |= 1;
        if (std::is_same_v <T, unsigned long>) flags |= 2;

        if (format && format->length) {
            Temporary64kB <wchar_t> buffer;
            auto length = Implementation::DescInt (buffer.data (), buffer.size (), format, &number, sizeof number, flags);
            return std::wstring (buffer.data (), length);
        } else {
            wchar_t buffer [96];
            auto length = Implementation::DescInt (buffer, sizeof buffer / sizeof buffer [0], format, &number, sizeof number, flags);
            return std::wstring (buffer, length);
        }
    }
}

inline std::wstring TRIMCORE::Describe (signed char v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed short v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed int v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed long v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (signed long long v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }

inline std::wstring TRIMCORE::Describe (unsigned char v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned short v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned int v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned long v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }
inline std::wstring TRIMCORE::Describe (unsigned long long v, DescriptionFormatting * parameter) { return Implementation::DescribeInt (v, parameter); }

template <typename LO, typename HI>
inline std::wstring TRIMCORE::Describe (const double_integer <LO, HI> & v, DescriptionFormatting * parameter) {
    static_assert (sizeof (double_integer <LO, HI>) == 16
                || sizeof (double_integer <LO, HI>) == 32
                || sizeof (double_integer <LO, HI>) == 64, "Only 128-bit, 256-bit and 512-bit large integers are supported.");

    return Implementation::DescribeInt (v, parameter);
}

#endif
