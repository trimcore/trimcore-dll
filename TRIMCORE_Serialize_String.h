#ifndef TRIMCORE_DLL_SERIALIZE_STRING_H
#define TRIMCORE_DLL_SERIALIZE_STRING_H

#include <string_view>
#include <string>

namespace TRIMCORE {

    inline Serialized Serialize (const std::string & string, Temporary64kB <std::uint8_t> &) { return { 'a', string.data (), sizeof (char) * string.size () }; }
    inline Serialized Serialize (const std::wstring & string, Temporary64kB <std::uint8_t> &) { return { 'w', string.data (), sizeof (wchar_t) * string.size () }; }
    inline Serialized Serialize (const std::u8string & string, Temporary64kB <std::uint8_t> &) { return { 'u8', string.data (), sizeof (char8_t) * string.size () }; }

    inline Serialized Serialize (std::string_view string, Temporary64kB <std::uint8_t> &) { return { 'a', string.data (), sizeof (char) * string.size () }; }
    inline Serialized Serialize (std::wstring_view string, Temporary64kB <std::uint8_t> &) { return { 'w', string.data (), sizeof (wchar_t) * string.size () }; }
    inline Serialized Serialize (std::u8string_view & string, Temporary64kB <std::uint8_t> &) { return { 'u8', string.data (), sizeof (char8_t) * string.size () }; }

    inline Serialized Serialize (const char & c, Temporary64kB <std::uint8_t> &) { return { 'a', &c, sizeof (char) }; }
    inline Serialized Serialize (const wchar_t & c, Temporary64kB <std::uint8_t> &) { return { 'w', &c, sizeof (wchar_t) }; }
    inline Serialized Serialize (const char8_t & c, Temporary64kB <std::uint8_t> &) { return { 'u8', &c, sizeof (char8_t) }; }

    inline Serialized Serialize (char * string, Temporary64kB <std::uint8_t> &) { return { 'a', string, string ? sizeof (char) * std::strlen (string) : 0 }; }
    inline Serialized Serialize (wchar_t * string, Temporary64kB <std::uint8_t> &) { return { 'w', string, string ? sizeof (wchar_t) * std::wcslen (string) : 0 }; }
    inline Serialized Serialize (char8_t * string, Temporary64kB <std::uint8_t> &) { return { 'u8', string, string ? sizeof (char8_t) * std::strlen ((char *) string) : 0 }; }

    inline Serialized Serialize (const char * string, Temporary64kB <std::uint8_t> &) { return { 'a', string, string ? sizeof (char) * std::strlen (string) : 0 }; }
    inline Serialized Serialize (const wchar_t * string, Temporary64kB <std::uint8_t> &) { return { 'w', string, string ? sizeof (wchar_t) * std::wcslen (string) : 0 }; }
    inline Serialized Serialize (const char8_t * string, Temporary64kB <std::uint8_t> &) { return { 'u8', string, string ? sizeof (char8_t) * std::strlen ((char *) string) : 0 }; }
}

#endif
