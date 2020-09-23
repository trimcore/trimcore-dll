#ifndef TRIMCORE_DLL_H
#define TRIMCORE_DLL_H

#ifndef TRIMCORE_DLL_IMPORT
#define TRIMCORE_DLL_IMPORT extern __declspec(dllimport)
#endif

#include <Windows.h>
#include <cstdint>
#include <string_view>

extern "C" IMAGE_DOS_HEADER __ImageBase;

#define TRIMCORE TRIMCORE

#ifdef _WIN32
#ifdef _MSC_VER
#if defined(_M_ARM64)
#define ARCHITECTURE "arm-64"
#elif defined(_M_ARM)
#define ARCHITECTURE "arm-32"
#elif defined(_M_X64)
#define ARCHITECTURE "x86-64"
#else
#define ARCHITECTURE "x86-32"
#endif
#else
#if defined(_WIN64)
#define ARCHITECTURE "x86-64"
#else
#define ARCHITECTURE "x86-32"
#endif
#endif
#endif

static const char BUILD_TIMESTAMP [] = {
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

#include "TRIMCORE_Alloc64kB.h"

#include "TRIMCORE_a2w.h"
#include "TRIMCORE_u2w.h"
#include "TRIMCORE_w2u.h"

#include "TRIMCORE_ApiErrorMessage.h"
#include "TRIMCORE_Rsrc_VersionInfo.h"
#include "TRIMCORE_Rsrc_StringTable.h"

#include "TRIMCORE_Describe.h"
#include "TRIMCORE_ConsoleInfo.h"

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
    //
    using Exception = Log::Exception;

    // Terminate
    //  - gracefully flushes various buffers used in the DLL (logs, files, ...)
    //
    void Terminate ();
}

// deferred implementation details

namespace TRIMCORE::Implementation {
    inline const Log::Identity * GetObjectIdentity (const Log::Provider * object) {
        return &object->identity;
    }
}

// imports fixes follow
//  - update these when mangling changes for some reason (e.g. namespace rename)
//  - because importing some things needs repeating this (already present in .exp/.lib files)

__pragma(comment (linker, "/alternatename:__imp_?log@TRIMCORE@@3VLog@1@A=__imp_log"))

#endif
