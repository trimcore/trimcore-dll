#ifndef TRIMCORE_DLL_SERIALIZE_TIME_H
#define TRIMCORE_DLL_SERIALIZE_TIME_H

namespace TRIMCORE {
    inline Serialized Serialize (const std::tm & tm, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('t', tm); }
    inline Serialized Serialize (const FILETIME & ft, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('t', ft); }
    inline Serialized Serialize (const SYSTEMTIME & st, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('t', st); }
}

#endif
