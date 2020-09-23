#ifndef TRIMCORE_DLL_RSRC_DESCRIBE_H
#define TRIMCORE_DLL_RSRC_DESCRIBE_H

namespace TRIMCORE {

    // Describe Rsrc::VersionNumber
    //  - TODO: short/long format
    //
    inline std::wstring Describe (Rsrc::VersionNumber number, DescriptionFormatting * format = nullptr) {
        return Describe (Describe (number.major, format) + L"."
                       + Describe (number.minor, format) + L"."
                       + Describe (number.patch, format) + L"."
                       + Describe (number.build, format), format);
    }

    inline std::size_t DescriptionLengthEst (Rsrc::VersionNumber) { return 23; }
}

#endif
