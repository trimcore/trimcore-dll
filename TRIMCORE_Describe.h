#ifndef TRIMCORE_DLL_DESCRIBE_H
#define TRIMCORE_DLL_DESCRIBE_H

#include "TRIMCORE_DescriptionFormatting.h"

namespace TRIMCORE {

    // Describe
    //  - generates text description for any given variable
    //  - must be implemented for every data type used in logging
    //     - return wstring (or compatible)
    //     - support: 'format' == nullptr

    template <typename T>
    inline std::wstring Describe (T, DescriptionFormatting * format = nullptr) = delete;

    // DescriptionLengthEst (default)
    //  - helps estimate how much space to initially reserve in temporary strings

    template <typename T>
    inline std::size_t DescriptionLengthEst (T) { return 0; }

    // Describe (T, fmt-string, locale)
    //  - simple forwarder for string -> DescriptionFormatting
    //
    template <typename T>
    inline std::wstring Describe (T && v, std::wstring_view fmt, LCID locale = 0) {
        DescriptionFormatting format;
        format.string = fmt.data ();
        format.length = fmt.size ();
        format.locale = locale;
        return Describe (v, &format);
    }
}

#include "TRIMCORE_Describe_String.h"
#include "TRIMCORE_Describe_Integer.h"
#include "TRIMCORE_Describe_Float.h"
#include "TRIMCORE_Describe_Bool.h"
#include "TRIMCORE_Describe_Time.h"

#include "TRIMCORE_Describe_Ptr.h"
#include "TRIMCORE_Describe_Handle.h"
#include "TRIMCORE_Describe_ApiError.h"

#endif

