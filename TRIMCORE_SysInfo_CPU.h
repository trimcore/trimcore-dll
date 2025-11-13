#ifndef TRIMCORE_DLL_SYSINFO_CPU_H
#define TRIMCORE_DLL_SYSINFO_CPU_H

#include "TRIMCORE_Topology.h"

namespace TRIMCORE {

    // PrettyCPUName_XXX
    //  - masks for individual prettifying operations that 'PrettyCPUName' does (by default ALL of them)

    static constexpr auto PrettyCPUName_CopyrightSymbol     = 0x0000'0001u;
    static constexpr auto PrettyCPUName_RegisteredSymbol    = 0x0000'0002u;
    static constexpr auto PrettyCPUName_TrademarkSymbol     = 0x0000'0004u;
    static constexpr auto PrettyCPUName_NoCoresText         = 0x0000'0010u;
    static constexpr auto PrettyCPUName_NoAPUText           = 0x0000'0020u;
    static constexpr auto PrettyCPUName_NoGPUSpec           = 0x0000'0100u;
    static constexpr auto PrettyCPUName_NoCnGCoresSpec      = 0x0000'0200u;
    static constexpr auto PrettyCPUName_NoFrequency         = 0x0000'1000u;
    static constexpr auto PrettyCPUName_NoNthGen            = 0x0000'2000u;
    //static constexpr auto PrettyCPUName_NoOptimizedFor      = 0x0000'2000u;
    static constexpr auto PrettyCPUName_ColapseSpaces       = 0x1000'0000u;

    // ABI

    namespace Implementation {

        // PrettyCPUName
        //  - internal processings of the raw processor name string into "pretty" one
        //
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY PrettyCPUName (wchar_t * out, const char * name, std::size_t name_length, unsigned int mask = ~0);
    }

    // GetCPUName
    //  - uses the proper API to retrieve 'processor' (can be NULL for current) name, typically uses CPUID
    //  - returns number of bytes written to 'name' buffer, 'length' is size of the buffer (provide at least 48 bytes)
    //
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY GetCPUName (const PROCESSOR_NUMBER * processor, char * name, std::size_t length);

    // GetCPUName
    //  - uses the proper API to retrieve 'processor' (can be NULL for current) name, typically uses CPUID
    //  - returns the name as string
    //
    inline std::string GetCPUName (const PROCESSOR_NUMBER * processor, unsigned int mask = ~0) {
        Temporary64kB <char> name;
        auto length = GetCPUName (processor, name.data (), name.size ());
        return std::string (name.data (), length - 1);
    }

    // GetPrettyCPUName
    //  - converts raw processor name string into "pretty" one
    //  - returns wstring
    //
    inline std::wstring GetPrettyCPUName (const char * name, std::size_t length, unsigned int mask = ~0) {
        Temporary64kB <wchar_t> buffer;
        return std::wstring (buffer.data (),
                             Implementation::PrettyCPUName (buffer.data (), name, length, mask));
    }

    // GetPrettyCPUName
    //  - retrieves 'processor' (can be NULL for current) name, and converts it into "pretty" one
    //  - returns wstring
    //
    inline std::wstring GetPrettyCPUName (const PROCESSOR_NUMBER * processor, unsigned int mask = ~0) {
        Temporary64kB <char> name;
        auto length = GetCPUName (processor, name.data (), name.size ());
        return GetPrettyCPUName (name.data (), length, mask);
    }
}

#endif
