#ifndef TRIMCORE_DLL_DESCRIBE_DATA_H
#define TRIMCORE_DLL_DESCRIBE_DATA_H

#include <span>
#include <vector>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescData (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const void * data, std::size_t size) noexcept;

    inline std::wstring DescribeData (const void * data, std::size_t size, DescriptionFormatting * format) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescData (buffer.data (), buffer.size (), format, data, size);
        return std::wstring (buffer.data (), length);
    }
}

namespace TRIMCORE {

    // Describe (vector/span of bytes)
    //  - displays bytes as a sequence of hexadecimal (default) numbers, with cofigurable separators
    //  - parameters
    //     - "decimal" - write decimal numbers, not hexadecimal
    //     - "plain" - remove default single space after every byte
    //     - separators example: 
    //        - "%1:-" - insert '-' every 1 byte
    //        - "%4: " - insert ' ' every 4 bytes
    //        - "%4+1< >" - insert ' ' every 4 bytes with offset +1 byte
    //        - "@3<: >" - insert ": " after 3rd byte (only once)
    //        - "%16:\r\n" - insert newline every 16 bytes    
    //  - after conversion, the string is internally passed to Describe (string) for case change, padding, trimming, etc.
    //
    template <typename T, std::size_t Extent>
    inline std::wstring Describe (const std::span <T, Extent> & value, DescriptionFormatting * format = nullptr) {
        return Implementation::DescribeData (value.data (), value.size () * sizeof (T), format);
    }

    template <typename T>
    inline std::wstring Describe (const std::pair <T *, std::size_t> & value, DescriptionFormatting * format = nullptr) {
        return Implementation::DescribeData (value.first, value.second, format);
    }
    template <typename T>
    inline std::wstring Describe (const std::pair <const T *, std::size_t> & value, DescriptionFormatting * format = nullptr) {
        return Implementation::DescribeData (value.first, value.second, format);
    }

    inline std::wstring Describe (const std::vector <std::byte> & value, DescriptionFormatting * format = nullptr) {
        return Implementation::DescribeData (value.data (), value.size (), format);
    }
    inline std::wstring Describe (const std::vector <std::uint8_t> & value, DescriptionFormatting * format = nullptr) {
        return Implementation::DescribeData (value.data (), value.size (), format);
    }
}

#endif

