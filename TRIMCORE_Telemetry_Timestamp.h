#ifndef TRIMCORE_DLL_TELEMETRY_TIMESTAMP_H
#define TRIMCORE_DLL_TELEMETRY_TIMESTAMP_H

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY TlmTsInit (std::uint8_t * raw, const FILETIME *) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY TlmTsToFT (const std::uint8_t * raw, FILETIME *) noexcept;
}

namespace TRIMCORE::Telemetry {

    // Timestamp
    //  - 5 bytes long compact but precise timestamp
    //
    struct Timestamp {
        std::uint8_t raw [5] = { 0,0,0,0,0 };

    public:
        Timestamp () = default;
        Timestamp (const Timestamp &) = default;
        Timestamp & operator = (const Timestamp &) = default;

        inline Timestamp (const FILETIME & ft) noexcept {
            TRIMCORE::Implementation::TlmTsInit (this->raw, &ft);
        }
        inline Timestamp (const SYSTEMTIME & st) noexcept {
            FILETIME ft;
            if (SystemTimeToFileTime (&st, &ft)) {
                TRIMCORE::Implementation::TlmTsInit (this->raw, &ft);
            }
        }
        inline Timestamp & operator = (const FILETIME & ft) noexcept {
            return *this = Timestamp (ft);
        }
        inline Timestamp & operator = (const SYSTEMTIME & st) noexcept {
            return *this = Timestamp (st);
        }

        inline explicit operator FILETIME () const noexcept {
            FILETIME ft;
            TRIMCORE::Implementation::TlmTsToFT (this->raw, &ft);
            return ft;
        }

        inline explicit operator SYSTEMTIME () const noexcept {
            SYSTEMTIME st {};
            FILETIME ft = (FILETIME) *this;
            FileTimeToSystemTime (&ft, &st);
            return st;
        }

        inline bool valid () const {
            return this->raw [0]
                 | this->raw [1]
                 | this->raw [2]
                 | this->raw [3]
                 | this->raw [4]
                 ;
        }

        static inline Timestamp Now () {
            FILETIME ft;
            GetSystemTimeAsFileTime (&ft);
            return Timestamp (ft);
        }
    };
}

#endif
