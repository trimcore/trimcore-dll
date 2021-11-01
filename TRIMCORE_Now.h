#ifndef TRIMCORE_DLL_NOW_H
#define TRIMCORE_DLL_NOW_H

#include <cstdint>

namespace TRIMCORE {

    // Timestamp
    //  - provides 64-bit uint alias to FILETIME structure
    //  - 'ull' is number of 100-nanosecond intervals since January 1, 1601 (UTC).
    //  - simply solves 64-bit alignment
    //
    union Timestamp {
        std::uint64_t ull;
        FILETIME      ft;
    };

    // Now
    //  - returns Windows system timestamp as the union above
    //
    TRIMCORE_DLL_IMPORT Timestamp Now ();

    // Describe Timestamp
    //  - forward to already implemented FILETIME rendering
    //
    inline std::wstring Describe (const Timestamp & ts, DescriptionFormatting * format = nullptr) {
        return Describe (ts.ft, format);
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
