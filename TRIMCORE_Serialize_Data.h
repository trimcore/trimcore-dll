#ifndef TRIMCORE_DLL_SERIALIZE_DATA_H
#define TRIMCORE_DLL_SERIALIZE_DATA_H

#include <span>
#include <vector>

namespace TRIMCORE {

    template <typename T, std::size_t Extent>
    inline Serialized Serialize (const std::span <T, Extent> & ref, Temporary64kB <std::uint8_t> &) { return { 'data', ref.data (), ref.size () * sizeof (T) }; }

    template <typename T>
    inline Serialized Serialize (const std::pair <T *, std::size_t> & ref, Temporary64kB <std::uint8_t> &) { return { 'data', ref.first, ref.second }; }
    template <typename T>
    inline Serialized Serialize (const std::pair <const T *, std::size_t> & ref, Temporary64kB <std::uint8_t> &) { return { 'data', ref.first, ref.second }; }

    inline Serialized Serialize (const std::vector <std::byte> & value, Temporary64kB <std::uint8_t> &) { return { 'data', value.data (), value.size () }; }
    inline Serialized Serialize (const std::vector <std::uint8_t> & value, Temporary64kB <std::uint8_t> &) { return { 'data', value.data (), value.size () }; }

}

#endif
