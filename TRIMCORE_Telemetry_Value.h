#ifndef TRIMCORE_DLL_TELEMETRY_VALUE_H
#define TRIMCORE_DLL_TELEMETRY_VALUE_H

#include "TRIMCORE_Telemetry_Timestamp.h"

namespace TRIMCORE::Telemetry {
    struct Value;
}

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmValCb (const TRIMCORE::Telemetry::Value *) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmValWr (const TRIMCORE::Telemetry::Value *, std::uint8_t * buffer, std::size_t length) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmValRd (TRIMCORE::Telemetry::Value *, const std::uint8_t * data, std::size_t size) noexcept;
}

namespace TRIMCORE::Telemetry {

    // Value
    //  - single information in serialization block
    //
    struct Value {
        std::uint32_t   index = 0; // 0...16843008
        std::uint16_t   reserved = 0;
        Timestamp       timestamp;
        std::uint8_t    size = 0; // valid sizes: 0,1,2,4, 6,8,16, and 32 (reserved for keys)
        std::uint8_t    data [32];

        // valid
        //  - returns true if the structure contents can be serialized
        //
        inline bool valid () const noexcept {
            return this->footprint () != 0;
        }

        // serialize
        //  - serializes object into 'buffer' of max 'length' size
        //  - format: 
        //     - initial byte: 0tii0nnn, additional data follow in order: index, timestamp, data
        //        - 0   - means value (1 means datatype increment, but 0x80 means reset to 0)
        //        - ii  - 0..3 - number of 'index' bytes that follow initial byte
        //        - t   - 0/1  - if set then 5-byte timestamp follows
        //        - 0   - reserved
        //        - nnn - 0..7 - index to table of number of data bytes following [0,1,2,4,6,8,16,32]
        //  - returns: - on success number of bytes written
        //             - 0 if buffer is not large enough
        //  - if nullptr is provided for buffer, returns required buffer size
        //
        inline std::size_t serialize (std::uint8_t * buffer, std::size_t length) const noexcept {
            return TRIMCORE::Implementation::TlmValWr (this, buffer, length);
        }

        // parse
        //  - parses raw data buffer
        //  - returns: - number of bytes processed
        //             - 0 - if buffer does not contain complete information
        //                 - on error, current content of Value remains unchanged
        //
        inline std::size_t parse (const std::uint8_t * data, std::size_t size) noexcept {
            return TRIMCORE::Implementation::TlmValRd (this, data, size);
        }

        // footprint
        //  - computes final size of serialized Value
        //  - or 0 when invalid (unsupported size, or index too large)
        //
        inline std::uint8_t footprint () const noexcept {
            return (std::uint8_t) TRIMCORE::Implementation::TlmValCb (this);
        }
    };
}

#endif
