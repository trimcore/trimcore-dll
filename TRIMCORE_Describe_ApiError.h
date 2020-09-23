#ifndef TRIMCORE_DLL_DESCRIBE_APIERROR_H
#define TRIMCORE_DLL_DESCRIBE_APIERROR_H

namespace TRIMCORE {

    // Describe ApiError
    //  - supported format parameters:

    inline std::wstring Describe (ApiError error, DescriptionFormatting * format = nullptr);

    // DescriptionLengthEst ApiError
    //  - 

    inline std::size_t DescriptionLengthEst (ApiError) { return 128; }
}

#include "TRIMCORE_Describe_ApiError.tcc"
#endif
