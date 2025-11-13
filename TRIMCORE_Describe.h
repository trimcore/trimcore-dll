#ifndef TRIMCORE_DLL_DESCRIBE_H
#define TRIMCORE_DLL_DESCRIBE_H

#include "TRIMCORE_DescriptionFormatting.h"

namespace TRIMCORE {

    // Describe
    //  - generates text description for any given variable
    //  - must be implemented for every data type used in logging
    //     - return wstring, wstring_view, or compatible
    //     - support: 'format' == nullptr

    template <typename T>
    inline std::wstring Describe (T, DescriptionFormatting * format = nullptr) = delete;

    // Describe (T, fmt-string)
    //  - simple forwarder for string -> DescriptionFormatting
    //
    template <typename T>
    inline auto Describe (T && v, std::wstring_view fmt) {
        DescriptionFormatting format (fmt);
        return Describe (v, &format);
    }
}

#include "TRIMCORE_Describe_Bool.h"
#include "TRIMCORE_Describe_Float.h"
#include "TRIMCORE_Describe_Integer.h"
#include "TRIMCORE_Describe_Time.h"
#include "TRIMCORE_Describe_Data.h"
#include "TRIMCORE_Describe_String.h"

#include "TRIMCORE_Describe_Ptr.h"
#include "TRIMCORE_Describe_Handle.h"
#include "TRIMCORE_Describe_Address.h"
#include "TRIMCORE_Describe_ApiError.h"
#include "TRIMCORE_Describe_ProcessorNumber.h"

#endif

