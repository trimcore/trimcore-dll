#ifndef TRIMCORE_DLL_H
#define TRIMCORE_DLL_H

#ifndef RC_INVOKED

#ifndef TRIMCORE_DLL_IMPORT
#define TRIMCORE_DLL_IMPORT extern __declspec(dllimport)
#endif

#ifdef _WIN64
#define TRIMCORE_APIENTRY APIENTRY
#else
#define TRIMCORE_APIENTRY __cdecl
#endif

#include <WinSock2.h>
#include <Windows.h>
#include <ATLbuild.h>
#include <cstdint>

#if (defined(__GNUC__) && (__GNUC__ < 8))
#include <experimental/string_view>
#else
#include <string_view>
#endif

extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

#define TRIMCORE TRIMCORE

// TODO: Licensing query API
// TODO: Telemetry API
// TODO: CreateProcess parameters API? through registry

#ifdef _WIN32
#if defined(_M_ARM64) || defined(__aarch64__)
#define ARCHITECTURE "AArch64"
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE                     IMAGE_FILE_MACHINE_ARM64 // 0xAA64
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64       IMAGE_FILE_MACHINE_ARMNT
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64_HOST  IMAGE_FILE_MACHINE_ARM64
#elif defined(_M_ARM) || defined(__arm__)
#define ARCHITECTURE "AArch32" // not really supported
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE                     IMAGE_FILE_MACHINE_ARMNT // 0x01C4
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64       IMAGE_FILE_MACHINE_ARMNT
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64_HOST  IMAGE_FILE_MACHINE_ARM64
#elif defined(_M_X64) || defined(_WIN64) 
#define ARCHITECTURE "x86-64"
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE                     IMAGE_FILE_MACHINE_AMD64 // 0x8664
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64       IMAGE_FILE_MACHINE_I386
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64_HOST  IMAGE_FILE_MACHINE_AMD64
#else
#define ARCHITECTURE "x86-32"
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE                     IMAGE_FILE_MACHINE_I386  // 0x014C
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64       IMAGE_FILE_MACHINE_I386
#define IMAGE_FILE_MACHINE_COMPILE_ARCHITECTURE_NATURAL_WOW64_HOST  IMAGE_FILE_MACHINE_AMD64
#endif
#endif

#ifndef RC_INVOKED

// assert Windows ABI invariants

static_assert (sizeof (long)    == 4, "Windows ABI invariant mismatch");
static_assert (sizeof (wchar_t) == 2, "Windows ABI invariant mismatch");

// constant of the invariant system

#define CURRENT_PROCESS_PSEUDOHANDLE ((HANDLE) -1)
#define CURRENT_THREAD_PSEUDOHANDLE ((HANDLE) -2)
#define PAGE_SIZE 4096uL
#define LARGE_PAGE_SIZE 2097152uL

#if _MSC_VER >= 1929 // VS2019 v16.10 and later (_MSC_FULL_VER >= 192829913 for VS 2019 v16.9)
// Works with /std:c++14 and /std:c++17, and performs optimization
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
// no-op in MSVC v14x ABI
#define NO_UNIQUE_ADDRESS /* [[no_unique_address]] */
#endif

#define OMIT_OVERFLOW_CHECKS __declspec(safebuffers)

static constexpr char BUILD_TIMESTAMP [] = {
    __DATE__ [7], __DATE__ [8], __DATE__ [9], __DATE__ [10],
    '-',
    (__DATE__ [0] == 'O' || __DATE__ [0] == 'N' || __DATE__ [0] == 'D') ? '1' : '0',

    (__DATE__ [0] == 'J' && __DATE__ [1] == 'a' && __DATE__ [2] == 'n') ? '1' :
    (__DATE__ [0] == 'F' && __DATE__ [1] == 'e' && __DATE__ [2] == 'b') ? '2' :
    (__DATE__ [0] == 'M' && __DATE__ [1] == 'a' && __DATE__ [2] == 'r') ? '3' :
    (__DATE__ [0] == 'A' && __DATE__ [1] == 'p' && __DATE__ [2] == 'r') ? '4' :
    (__DATE__ [0] == 'M' && __DATE__ [1] == 'a' && __DATE__ [2] == 'y') ? '5' :
    (__DATE__ [0] == 'J' && __DATE__ [1] == 'u' && __DATE__ [2] == 'n') ? '6' :
    (__DATE__ [0] == 'J' && __DATE__ [1] == 'u' && __DATE__ [2] == 'l') ? '7' :
    (__DATE__ [0] == 'A' && __DATE__ [1] == 'u' && __DATE__ [2] == 'g') ? '8' :
    (__DATE__ [0] == 'S') ? '9' :
    (__DATE__ [0] == 'O') ? '0' :
    (__DATE__ [0] == 'N') ? '1' : '2',
    '-',
    (__DATE__ [4] != ' ') ? __DATE__ [4] : '0', __DATE__ [5],
    ' ',
    __TIME__ [0], __TIME__ [1], __TIME__ [2], __TIME__ [3], __TIME__ [4], __TIME__ [5], __TIME__ [6], __TIME__ [7]
};

#define VCRUNTIME_VERSION       (_LIBS_PRODUCT_VERSION_MAJOR * 100 + _LIBS_PRODUCT_VERSION_MINOR)
#define VCRUNTIME_VERSION_FULL  (_LIBS_PRODUCT_VERSION_MAJOR * 10000000 + _LIBS_PRODUCT_VERSION_MINOR * 100000 + _LIBS_BUILD)

#endif

// LANG_TRIMCORE_LOG_PROVIDER_NAMES
//  - use to introduce block of names for string resources that apply to Log::Provider derived objects
//  - example:
//     - xxx.rc
//       STRINGTABLE
//       LANGUAGE LANG_TRIMCORE_LOG_PROVIDER_NAMES, SUBLANG_DEFAULT
//           0x123 "component.class.tool" // structured title for strings beginning with 0x1230 (note no 0 there)
//       STRINGTABLE
//       LANGUAGE LANG_ENGLISH, SUBLANG_DEFAULT
//           0x1230 "first string"
//           0x1237 "string with ID 7"
//     - xxx.cpp
//       class Abc : TRIMCORE::Log::Provider {
//           Abc () : TRIMCORE::Log::Provider ("Abc", "component.class.tool") {}
//           void func () { this->report (TRIMCORE::Log::Error, 7); }; // writes "string with ID 7" (string with ID 0x1237)
//       }
//
#define LANG_TRIMCORE_LOG_PROVIDER_NAMES 0x03F0

#ifndef RC_INVOKED

// if EXE dll-exports DWORD named 'TRIMCORE_Requirements' set to following values
// then TRIMCORE.DLL check those before EXE starts, and reports if the HW doesn't meet those,
// closing "gracefully" immediately, instead of crashing later

#if defined(_M_ARM64)
#define EXE_REQUIREMENTS_ISA_LEVEL_MASK         0x0000FF00uL
#define EXE_REQUIREMENTS_ISA_LEVEL_8_1          0x00000100uL // atomics
#define EXE_REQUIREMENTS_ISA_LEVEL_8_2          0x00000200uL // DP
#define EXE_REQUIREMENTS_ISA_LEVEL_8_3          0x00000300uL // JSCVT, LRCPC

#define EXE_REQUIREMENTS_SIMD_LEVEL_MASK        0x0000000FuL
#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE         0x00000001uL
#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE2        0x00000002uL
#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE2_1      0x00000003uL

#define EXE_REQUIREMENTS_SIMD_LEVEL_SSE3        0
#define EXE_REQUIREMENTS_SIMD_LEVEL_SSE4        0
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX         0
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX2        0
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX512      0

#define EXE_REQUIREMENTS_BIT_MANIP_MASK         0
#define EXE_REQUIREMENTS_BIT_MANIP_ABM          0
#define EXE_REQUIREMENTS_BIT_MANIP_BMI1         0
#define EXE_REQUIREMENTS_BIT_MANIP_BMI2         0

#else
#define EXE_REQUIREMENTS_ISA_LEVEL_MASK         0
#define EXE_REQUIREMENTS_ISA_LEVEL_8_1          0
#define EXE_REQUIREMENTS_ISA_LEVEL_8_2          0
#define EXE_REQUIREMENTS_ISA_LEVEL_8_3          0

#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE         0
#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE2        0
#define EXE_REQUIREMENTS_SIMD_LEVEL_SVE2_1      0

#define EXE_REQUIREMENTS_SIMD_LEVEL_MASK        0x0000000FuL
#define EXE_REQUIREMENTS_SIMD_LEVEL_SSE2        0x00000000uL // SSE2 is baseline because Win7 (oldest we support) requires SSE2
#define EXE_REQUIREMENTS_SIMD_LEVEL_SSE3        0x00000001uL
#define EXE_REQUIREMENTS_SIMD_LEVEL_SSE4        0x00000002uL // includes 4.2 and CRC32
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX         0x00000003uL
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX2        0x00000004uL
#define EXE_REQUIREMENTS_SIMD_LEVEL_AVX512      0x00000005uL

#define EXE_REQUIREMENTS_BIT_MANIP_MASK         0x00000070uL
#define EXE_REQUIREMENTS_BIT_MANIP_ABM          0x00000010uL // POPCNT/LZCNT
#define EXE_REQUIREMENTS_BIT_MANIP_BMI1         0x00000020uL
#define EXE_REQUIREMENTS_BIT_MANIP_BMI2         0x00000030uL // PDEP/PEXT/BZHI
#endif

#define EXE_REQUIREMENTS_AES                    0x00010000uL // AES-NI or ARM CRYPTO

#if defined(_M_ARM64)
#define EXE_REQUIREMENTS_F16C                   0
#define EXE_REQUIREMENTS_CMPXCHG16B             0
#define EXE_REQUIREMENTS_LRCPC                  0x00020000uL
#else
#define EXE_REQUIREMENTS_F16C                   0x00020000uL
#define EXE_REQUIREMENTS_CMPXCHG16B             0x00100000uL // _InterlockedCompareExchange128
#define EXE_REQUIREMENTS_LRCPC                  0
#endif

// EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT
//  - expands to one of the above according to compiler (MSVC only) options

#if defined(_M_ARM64)
#if __ARM_ARCH >= 803
#define EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT   EXE_REQUIREMENTS_ISA_LEVEL_8_3
#elif  __ARM_ARCH >= 802
#define EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT   EXE_REQUIREMENTS_ISA_LEVEL_8_2
#elif  __ARM_ARCH >= 801
#define EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT   EXE_REQUIREMENTS_ISA_LEVEL_8_1
#endif
#else
#if defined(__AVX512F__)
#define EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT  EXE_REQUIREMENTS_SIMD_LEVEL_AVX512
#elif defined(__AVX2__)
#define EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT  EXE_REQUIREMENTS_SIMD_LEVEL_AVX2
#elif defined(__AVX__)
#define EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT  EXE_REQUIREMENTS_SIMD_LEVEL_AVX
#endif
#endif

#if defined(_M_ARM64)
#ifdef __ARM_FEATURE_RCPC
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  EXE_REQUIREMENTS_LRCPC
#else
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  0
#endif
#else
#if defined (__AVX2__)
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  (EXE_REQUIREMENTS_BIT_MANIP_BMI2 | EXE_REQUIREMENTS_AES)
#elif defined(__AVX__)
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  EXE_REQUIREMENTS_BIT_MANIP_ABM
#elif defined(__SSE42__) // TODO: does not exist
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  EXE_REQUIREMENTS_BIT_MANIP_ABM
#else
#define EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT  0
#endif
#endif

#ifndef EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT
#define EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT 0
#endif
#ifndef EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT
#define EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT 0
#endif

#ifndef EXE_REQUIREMENTS_AUTODETECT
#define EXE_REQUIREMENTS_AUTODETECT (EXE_REQUIREMENTS_ISA_LEVEL_AUTODETECT | EXE_REQUIREMENTS_SIMD_LEVEL_AUTODETECT | EXE_REQUIREMENTS_ADDITIONAL_AUTODETECT)
#endif

#include "TRIMCORE_Alloc64kB.h"
#include "TRIMCORE_AllocLarge.h"
#include "TRIMCORE_AllocAppMsg.h"

#include "TRIMCORE_a2w.h"
#include "TRIMCORE_u2w.h"
#include "TRIMCORE_w2a.h"
#include "TRIMCORE_w2u.h"
#include "TRIMCORE_s2ascii.h"

#include "TRIMCORE_ApiErrorMessage.h"
#include "TRIMCORE_Rsrc_VersionInfo.h"
#include "TRIMCORE_Rsrc_StringTable.h"

#include "TRIMCORE_Describe.h"
#include "TRIMCORE_Serialize.h"
#include "TRIMCORE_ConsoleInfo.h"
#include "TRIMCORE_CommandLineOptions.h"

#include "TRIMCORE_SysInfo.h"
#include "TRIMCORE_SysInfo_CPU.h"
#include "TRIMCORE_ProcessorInfo.h"
#include "TRIMCORE_ProcessArchitecture.h"
#include "TRIMCORE_Telemetry.h"
#include "TRIMCORE_ThreadName.h"
#include "TRIMCORE_PointerInfo.h"
#include "TRIMCORE_Licensing.h"

#include "TRIMCORE_Now.h"
#include "TRIMCORE_Log.h"
#include "TRIMCORE_Log_Provider.h"
#include "TRIMCORE_Log_Exception.h"

#include "TRIMCORE_Rsrc_Describe.h"

// version
//  - current process executable version info
//
TRIMCORE_DLL_IMPORT TRIMCORE::Rsrc::VersionInfo version;

namespace TRIMCORE {

    // TRIMCORE::Exception == TRIMCORE::Log::Exception
    //  - shorter name, simpler usage
    //
    using Exception = Log::Exception;

    // Update
    //  - updates all system information that may change dynamically
    //     - current UBR version number in case the OS was hot-patched
    //     - maximum available physical memory
    //  - reinitializes processor mapping information that all APIs rely on
    //  - updates internal telemetry buffer (does not transmit anything)
    //
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY Update ();

    // Terminate
    //  - gracefully flushes various buffers used in the DLL (logs, files, ...)
    //
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY Terminate ();

    // FloodGate
    //  - set when DLL is being unloaded on process exit
    //  - mostly for internal purposes, to avoid useless deallocations
    //
    TRIMCORE_DLL_IMPORT bool FloodGate;
}

// imports fixes follow
//  - update these when mangling changes for some reason (e.g. namespace rename)
//  - because importing some things needs repeating this (already present in .exp/.lib files)

__pragma(comment (linker, "/alternatename:__imp_?log@TRIMCORE@@3VLog@1@A=__imp_log"))
__pragma(comment (linker, "/alternatename:__imp_?FloodGate@TRIMCORE@@3_NA=__imp_floodgate"))
__pragma(comment (linker, "/alternatename:__imp_?Update@TRIMCORE@@YAXXZ=__imp_Update"))

#ifdef _WIN64
__pragma(comment (linker, "/alternatename:__imp_?process@TRIMCORE@@3PEBUProcessOverview@1@EB=__imp_process"))
#else
__pragma(comment (linker, "/alternatename:__imp_?process@TRIMCORE@@3PBUProcessOverview@1@B=__imp_process"))
#endif

// __pragma(comment (linker, "/alternatename:__imp_?preferred_language@Rsrc@TRIMCORE@@3GA=__imp_preferred_language"))

#endif
#endif
