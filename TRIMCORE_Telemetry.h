#ifndef TRIMCORE_DLL_TELEMETRY_H
#define TRIMCORE_DLL_TELEMETRY_H

#include "TRIMCORE_Telemetry_Value.h"
#include "TRIMCORE_Telemetry_Timestamp.h"

namespace TRIMCORE {
    namespace Implementation {
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmInv  (void ** buffer, std::size_t offset) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmHw   (void ** buffer, std::size_t offset) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmBase (void ** buffer, std::size_t offset) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmLive (void ** buffer, std::size_t offset) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmUser (void ** buffer, std::size_t offset) noexcept;

        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmAddType (void ** buffer, std::size_t * offset) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY TlmAddValue (void ** buffer, std::size_t * offset, const TRIMCORE::Telemetry::Value *) noexcept;
    }

    namespace Telemetry {

    }
}

#endif
