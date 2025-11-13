#ifndef TRIMCORE_DLL_SERIALIZE_H
#define TRIMCORE_DLL_SERIALIZE_H

namespace TRIMCORE {

    // Serialized
    //  - references binary content of an object serialized for Logging purposes
    //  - type represents kind-of FourCC code to distinguish underlying type
    //  - General currently defined types:
    //     - 't'/'f' - boolean true/false (no data)
    //     - 'i','u','r' - signed integer, unsigned integer, r - floating point number
    //     - 'a'/'w'/'u8' - strings
    //     - 'addr' - sockaddr type address
    //     - 'p' - pointer
    //     - 't' - time, either std::tm (size = 36), FILETIME (size = 8) or SYSTEMTIME (size = 16)
    //     - 'data' - raw data passed
    //     - 'HNDL'/'HMOD','HKEY' - handle/HMODULE,HKEY
    //     - 'LPN' - PROCESSOR_NUMBER
    //     - 'GUID'
    //     - 'err' - ApiError/DWORD (GetLastError() values)
    //  - Windows library defined types:
    //     - 'BMsf' - Windows::SaveBitmapFormat
    //  - TRIMCORE facilities defined types:
    //     - 'ver' - TRIMCORE::Rsrc::VersionNumber
    //     - 'con','conr' - ConsoleInfo, ConsoleInfo::Redirection
    //     - 'LogL','LogF','Log'+letter - used by TRIMCORE::Log, LogL = level, LogF = func
    //     - 'OSv' - SystemOverview::Version
    //  - SCADA facilities defined types:
    //     - 'Atom','Aerr' - Scada::Atom, Scada::Atom::InvalidInputReason
    //     - 'HMIu' - HMI::Layout::Unit
    //     - 'SC', 'SCid', 'SCt', 'SCi', 'SCop', 'SCcr' - Scada::Cell, Cell::ID, Cell::Type, Cell::Information, Cell:Conversion
    //     - 'SSsp' - Scada::Manifold::Socket::Specification
    //
    struct Serialized {
        std::uint32_t type = 0;
        const void *  data = nullptr;
        std::size_t   size = 0;

    public:
        inline explicit operator bool () const noexcept {
            return this->type != 0 || this->data != nullptr;
        }
    };

    // Serialize
    //  - 
    //  - implementation may either return pointer into referenced T
    //    or into scratch buffer, into which it had constructed the representation

    template <typename T>
    Serialized Serialize (T, Temporary64kB <std::uint8_t> & scratch) = delete;

    // SerializeTrivially
    //  - intentionally deleted, this usage is a bug!
    //  - 'value' must be a reference to prevent dangling pointer to invalid stack data
    //
    template <typename T>
    Serialized SerializeTrivially (std::uint32_t type, const T && value) = delete;

    // SerializeTrivially
    //  - 
    //
    template <typename T>
    Serialized SerializeTrivially (std::uint32_t type, const T & value) {
        return { type, &value, sizeof (T) };
    }
}

#include "TRIMCORE_Serialize_Address.h"
#include "TRIMCORE_Serialize_ApiError.h"
#include "TRIMCORE_Serialize_Data.h"
#include "TRIMCORE_Serialize_Fundamental.h"
#include "TRIMCORE_Serialize_ProcessorNumber.h"
#include "TRIMCORE_Serialize_String.h"
#include "TRIMCORE_Serialize_Time.h"
#include "TRIMCORE_Serialize_Windows.h"

#endif
