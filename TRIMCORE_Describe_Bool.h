#ifndef TRIMCORE_DLL_DESCRIBE_BOOL_H
#define TRIMCORE_DLL_DESCRIBE_BOOL_H

namespace TRIMCORE {

    // Describe (bool)
    //  - 
    //  - TODO: .cpp
    //  - TODO: numeric, symbolic
    //
    inline std::wstring Describe (bool value, DescriptionFormatting * format = nullptr) {
        return Describe (value ? L"True" : L"False", format);
    }

    // DescriptionLengthEst (default)
    //  - helps estimate how much space to initially reserve in temporary strings

    inline std::size_t DescriptionLengthEst (bool) { return 5; }
}

#endif

