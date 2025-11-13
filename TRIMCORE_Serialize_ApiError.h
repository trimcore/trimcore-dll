#ifndef TRIMCORE_DLL_SERIALIZE_APIERROR_H
#define TRIMCORE_DLL_SERIALIZE_APIERROR_H

namespace TRIMCORE {
    inline Serialized Serialize (const TRIMCORE::ApiError & error, Temporary64kB <std::uint8_t> &) {
        return SerializeTrivially ('err', error.code);
    }
}

#endif
