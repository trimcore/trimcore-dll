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
    //     - ns - display nanoseconds
    //     - L - use user's default locale 
    //     - L<xxx> - use locale xxx, where xxx is empty (local), name (en-US), number (0x0405) or "system"
    //     - TZ - convert UTC to local time
    //     - TZ<+1:45> - convert to local time using provided timezone bias
    //     - duration - displays value in a duration format
    //        - numeric + and +! parameters can be used
    //        - duration<CPU> - displays duration in progressive format
    //     - U - upgrades ':' symbol to UNICODE one, which is slightly nicer
    //  - default format is "D<db>,T24<db>,ms"

    inline std::wstring Describe (const std::tm &, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (const FILETIME &, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (const SYSTEMTIME &, DescriptionFormatting * format = nullptr);
}

#include "TRIMCORE_Describe_Time.tcc"
#endif

