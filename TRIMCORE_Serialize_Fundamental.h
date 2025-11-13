#ifndef TRIMCORE_DLL_SERIALIZE_FUNDAMENTAL_H
#define TRIMCORE_DLL_SERIALIZE_FUNDAMENTAL_H

#include "double_integer.hpp"

namespace TRIMCORE {

    inline Serialized Serialize (bool v, Temporary64kB <std::uint8_t> &) { return { (unsigned) (v ? 't' : 'f'), nullptr, 0 }; }

    inline Serialized Serialize (const signed char & v,      Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('i', v); }
    inline Serialized Serialize (const signed short & v,     Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('i', v); }
    inline Serialized Serialize (const signed int & v,       Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('i', v); }
    inline Serialized Serialize (const signed long & v,      Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('i', v); }
    inline Serialized Serialize (const signed long long & v, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('i', v); }

    inline Serialized Serialize (const unsigned char & v,      Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('u', v); }
    inline Serialized Serialize (const unsigned short & v,     Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('u', v); }
    inline Serialized Serialize (const unsigned int & v,       Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('u', v); }
    inline Serialized Serialize (const unsigned long & v,      Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('u', v); }
    inline Serialized Serialize (const unsigned long long & v, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('u', v); }

    inline Serialized Serialize (const float & v,       Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('r', v); }
    inline Serialized Serialize (const double & v,      Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('r', v); }
    inline Serialized Serialize (const long double & v, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('r', v); }

    inline Serialized Serialize (const std::nullptr_t & v, Temporary64kB <std::uint8_t> &) { return { 0, &v, 0 }; }
    inline Serialized Serialize (void *& v, Temporary64kB <std::uint8_t> &) { return { 'p', &v, sizeof v }; }
    inline Serialized Serialize (const void *& v, Temporary64kB <std::uint8_t> &) { return { 'p', &v, sizeof v }; }
    
    template <typename LO, typename HI>
    inline Serialized Serialize (const double_integer <LO, HI> & v, Temporary64kB <std::uint8_t> &) {
        return SerializeTrivially (std::numeric_limits <double_integer <LO, HI>>::is_signed ? 'i' : 'u', v);
    }
}


#endif

