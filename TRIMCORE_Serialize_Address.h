#ifndef TRIMCORE_DLL_SERIALIZE_ADDRESS_H
#define TRIMCORE_DLL_SERIALIZE_ADDRESS_H

#include <ws2def.h>
#include <ws2ipdef.h>
#include <afunix.h>
#include <hvsocket.h>

namespace TRIMCORE {
#define DEFINE_SERIALIZE_FOR_SOCKADDR(T) \
    inline Serialized Serialize (const T & addr, Temporary64kB <std::uint8_t> &) {  \
        return SerializeTrivially ('addr', addr);                                   \
    }                                                                               \
    inline Serialized Serialize (const T * addr, Temporary64kB <std::uint8_t> &) {  \
        return SerializeTrivially ('addr', *addr);                                  \
    }                                                                               \
    inline Serialized Serialize (T * addr, Temporary64kB <std::uint8_t> &) {        \
        return SerializeTrivially ('addr', *const_cast <const T *> (addr));         \
    }

    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr);
    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr_gen);
    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr_storage);
    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr_storage_xp);
    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr_in);
    DEFINE_SERIALIZE_FOR_SOCKADDR (sockaddr_in6);
    DEFINE_SERIALIZE_FOR_SOCKADDR (SOCKADDR_HV);
    DEFINE_SERIALIZE_FOR_SOCKADDR (SOCKADDR_UN);

#undef DEFINE_DESCRIBE_FOR_SOCKADDR

    inline Serialized Serialize (const in_addr & addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in { AF_INET, 0, addr }, scratch);
    }
    inline Serialized Serialize (const in_addr * addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in { AF_INET, 0, *addr }, scratch);
    }
    inline Serialized Serialize (in_addr * addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in { AF_INET, 0, *addr }, scratch);
    }

    inline Serialized Serialize (const in6_addr & addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in6 { AF_INET6, 0, 0, addr, 0 }, scratch);
    }
    inline Serialized Serialize (const in6_addr * addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in6 { AF_INET6, 0, 0, *addr, 0 }, scratch);
    }
    inline Serialized Serialize (in6_addr * addr, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (sockaddr_in6 { AF_INET6, 0, 0, *addr, 0 }, scratch);
    }
}

#endif

