#ifndef TRIMCORE_DLL_DESCRIBE_PTR_H
#define TRIMCORE_DLL_DESCRIBE_PTR_H

namespace TRIMCORE {

    // Describe pointer
    //  - supported format parameters:
    //     - details - attempts to localize the pointer, appends results as follow:
    //        - global/function/resource pointers: "!module.dll[.section]"
    //           - sections names are retrieved from module header, typically are:
    //              - .text for code,
    //              - .rdata for constants
    //              - .data for global
    //              - .rsrc for resources
    //        - pointer to stack: "!module.dll:12345" - module that created the thread and thread ID
    //           - if present, thread's description is appended enclosed in double quotes
    //        - process heap pointer: ":heap"

    inline std::wstring Describe (const void * ptr, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (void * ptr, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const void *> (ptr), format);
    }

    // DescriptionLengthEst pointer
    //  - 64-bit is truncated to 48 actually used bits

    inline std::size_t DescriptionLengthEst (const void *) {
        return (sizeof (void *) > sizeof (std::uint32_t)) ? 12 : 8;
    }
    inline std::size_t DescriptionLengthEst (void * p) {
        return DescriptionLengthEst (const_cast <const void *> (p));
    }
}

#include "TRIMCORE_Describe_Ptr.tcc"
#endif
