#ifndef TRIMCORE_DLL_DESCRIBE_APIERROR_H
#define TRIMCORE_DLL_DESCRIBE_APIERROR_H

namespace TRIMCORE {

    // Describe ApiError
    //  - supported format parameters:

    inline std::wstring Describe (ApiError error, DescriptionFormatting * format = nullptr);
}

#include "TRIMCORE_Describe_ApiError.tcc"
#endif
