#ifndef TRIMCORE_DLL_DESCRIBE_BOOL_H
#define TRIMCORE_DLL_DESCRIBE_BOOL_H

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescBool (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, bool) noexcept;
}

namespace TRIMCORE {

    // Describe (bool)
    //  - without any additional parameter writes "True" or "False" into the buffer
    //  - parameters
    //     - "#" - bool value is converted to "1" or "0"
    //     - specific unicode symbols are chosen for "square", "circle", "checkbox", "markbox", "check" and "heavycheck"
    //     - "custom" - can specify pair of strings to render
    //         - "custom:aa:bb" - then 'aa' is used for true, and 'bb' for false
    //         - separator is ":" by default, can be chosen by "separator" parameter
    //            - "separator:x,custom:TxF" - then "T" is for true and "F" for false
    //  - after conversion, the string is internally passed to Describe (string) for case change, padding, trimming, etc.
    //
    inline std::wstring Describe (bool value, DescriptionFormatting * format = nullptr) {
        if (format && format->length) {
            Temporary64kB <wchar_t> buffer;
            auto length = Implementation::DescBool (buffer.data (), buffer.size (), format, value);
            return std::wstring (buffer.data (), length);
        } else {
            wchar_t buffer [16];
            auto length = Implementation::DescBool (buffer, sizeof buffer / sizeof buffer [0], format, value);
            return std::wstring (buffer, length);
        }
    }
}

#endif

