#ifndef TRIMCORE_DLL_DESCRIBE_TIME_H
#define TRIMCORE_DLL_DESCRIBE_TIME_H

namespace TRIMCORE {

    // Describe std::tm/FILETIME/SYSTEMTIME
    //  - supported format parameters:
    //     - D<xxx> - display date, in xxx (optional) format, "db" format is YYYY-MM-DD
    //     - LD - display long date
    //     - T<xxx> - display time, in xxx (optional) format, "db" format is HH:MM:SS
    //     - T24<xxx> - time, force 24 hour format
    //     - ST - time without seconds
    //     - ms - display milliseconds
    //     - L - use user's default locale 
    //     - L<xxx> - use locale xxx, where xxx is either name (en-US) or number 0x0405
    //  - default format is "D<db>,T24<db>,ms"

    inline std::wstring Describe (const std::tm &, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (const FILETIME &, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (const SYSTEMTIME &, DescriptionFormatting * format = nullptr);

    // DescriptionLengthEst
    //  - length of default <db> formatted string: "%04u-%02u-%02u %02u:%02u:%02u.%03u"

    inline std::size_t DescriptionLengthEst (const std::tm &) { return 24; }
    inline std::size_t DescriptionLengthEst (const FILETIME &) { return 24; }
    inline std::size_t DescriptionLengthEst (const SYSTEMTIME &) { return 24; }
}

#include "TRIMCORE_Describe_Time.tcc"
#endif

