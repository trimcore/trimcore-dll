#ifndef TRIMCORE_DLL_RSRC_DESCRIBE_H
#define TRIMCORE_DLL_RSRC_DESCRIBE_H

namespace TRIMCORE {

    // Describe Rsrc::VersionNumber
    //  - TODO: short/long format
    //
    inline std::wstring Describe (TRIMCORE::Rsrc::VersionNumber number, TRIMCORE::DescriptionFormatting * format = nullptr) {
        return Describe (Describe (number.major, format) + L"."
                         + Describe (number.minor, format) + L"."
                         + Describe (number.patch, format) + L"."
                         + Describe (number.build, format), format);
    }

    inline Serialized Serialize (const TRIMCORE::Rsrc::VersionNumber & number, TRIMCORE::Temporary64kB <std::uint8_t> &) {
        return SerializeTrivially ('ver', number);
    }
}

#endif
