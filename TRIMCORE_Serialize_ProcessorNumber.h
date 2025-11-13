#ifndef TRIMCORE_DLL_SERIALIZE_PROCESSOR_NUMBER_H
#define TRIMCORE_DLL_SERIALIZE_PROCESSOR_NUMBER_H

namespace TRIMCORE {
    inline Serialized Serialize (const PROCESSOR_NUMBER & p, Temporary64kB <std::uint8_t> &) {
        return SerializeTrivially ('LPN', p);
    }
}

#endif

