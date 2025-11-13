#ifndef TRIMCORE_DLL_DESCRIBE_PTR_H
#define TRIMCORE_DLL_DESCRIBE_PTR_H

namespace TRIMCORE {

    // Describe pointer
    //  - supported format parameters:
    //     - details - attempts to localize the pointer, appends results as follow:
    //        - global/function/resource pointers: "!module.dll[.section+offset]"
    //           - sections names are retrieved from module header, typically are:
    //              - .text for code,
    //              - .rdata for constants
    //              - .data for global
    //              - .rsrc for resources
    //           - offset is hexadecimal offset into the section
    //        - pointer to stack: "!module.dll:12345-offset" - module that created the thread and thread info
    //           - number represents thread ID, offset is hexadecimal offset into the stack
    //           - if present, thread's description is appended enclosed in double quotes
    //        - process heap pointer: ":heap"

    inline std::wstring Describe (const void * ptr, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (void * ptr, DescriptionFormatting * format = nullptr) {
        return Describe (const_cast <const void *> (ptr), format);
    }
}

#include "TRIMCORE_Describe_Ptr.tcc"
#endif
