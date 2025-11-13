#ifndef TRIMCORE_DLL_NOW_H
#define TRIMCORE_DLL_NOW_H

#include <cstdint>

namespace TRIMCORE {

    // Timestamp
    //  - provides 64-bit uint alias to FILETIME structure
    //  - 'ull' is number of 100-nanosecond intervals since January 1, 1601 (UTC).
    //  - simply solves 64-bit alignment
    //
    struct Timestamp {
        union {
            std::uint64_t ull;
            FILETIME      ft;
        };
        explicit operator bool () const {
            return this->ull != 0;
        }
    };

    // Now
    //  - returns precise Windows system timestamp as the union above
    //
    TRIMCORE_DLL_IMPORT Timestamp TRIMCORE_APIENTRY Now ();

    // Describe Timestamp
    //  - forward to already implemented FILETIME rendering
    //
    inline std::wstring Describe (const Timestamp & ts, DescriptionFormatting * format = nullptr) {
        return Describe (ts.ft, format);
    }
    inline Serialized Serialize (const Timestamp & ts, Temporary64kB <std::uint8_t> & scratch) {
        return Serialize (ts.ft, scratch);
    }


    // operators
    //  - compare underlying 64-bit integer
    //
    inline bool operator <  (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull <  b.ull; }
    inline bool operator <= (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull <= b.ull; }
    inline bool operator >  (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull > b.ull; }
    inline bool operator >= (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull >= b.ull; }
    inline bool operator == (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull == b.ull; }
    inline bool operator != (const TRIMCORE::Timestamp & a, const TRIMCORE::Timestamp & b) noexcept { return a.ull != b.ull; }

    // FILETIME operators
    //  - compare as 64-bit integer
    //
    inline bool operator <  (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } <  Timestamp { .ft = b }; }
    inline bool operator <= (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } <= Timestamp { .ft = b }; }
    inline bool operator >  (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } >  Timestamp { .ft = b }; }
    inline bool operator >= (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } != Timestamp { .ft = b }; }
    inline bool operator == (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } == Timestamp { .ft = b }; }
    inline bool operator != (const FILETIME & a, const FILETIME & b) noexcept { return Timestamp { .ft = a } != Timestamp { .ft = b }; }
}

#endif
