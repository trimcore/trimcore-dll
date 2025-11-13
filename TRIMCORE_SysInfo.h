#ifndef TRIMCORE_DLL_SYSINFO_H
#define TRIMCORE_DLL_SYSINFO_H

#include "TRIMCORE_Rsrc_VersionInfo.h"
#include "TRIMCORE_Topology.h"

namespace TRIMCORE {

    // SystemOverview, global 'os' object
    //  - quick operating system overview gathered by our service
    //
    struct SystemOverview {
        struct Version {
            union {
                std::uint64_t word; // 0xMM'mm'BBBBBBB'UUUUUuLL
                struct {
                    std::uint64_t update : 20; // UBR
                    std::uint64_t build  : 28;
                    std::uint64_t minor  : 8;
                    std::uint64_t major  : 8;
                };
            };
        } version; // UBR is dynamically updated by TRIMCORE Service (if installed)

        std::uint16_t kernel; // true ntoskrnl.exe (NTDLL.DLL) build number
        std::uint16_t reserved10;
        std::uint32_t reserved12;

        std::uint64_t trimcore   : 1; // 1 if connected to trimcore service created section
        std::uint64_t invariant  : 1;
        std::uint64_t vm         : 1; // running within VM (not a primary partition)
        std::uint64_t container  : 1; // running in container/silo
        std::uint64_t server     : 1; // server OS, prefer higher memory usage to offer stability
        std::uint64_t client_lts : 1; // client OS on LTSB or LTSC channel
        std::uint64_t checked    : 1; // checked build of Windows
        std::uint64_t evaluation : 1; // known evaluation build of determined by heuristics
        std::uint64_t wow        : 1; // WOW64 layer is active
        std::uint64_t safemode   : 2; // 0 (normal), 1, 2 - see GetSystemMetrics (SM_CLEANBOOT)

        WORD          machine; // IMAGE_FILE_MACHINE_XXX of the underlying OS
        std::uint8_t  bitness; // 31 (x86), 32 (x86 3GT/WoW64), 43 (x64 7/8), 47 (x64 8.1+), 56 (x64 LA57 W10)
        // std::uint8_t  reserved_byte_1;

        DWORD         services; // services session ID for the container (non-zero) or standard 0
        DWORD         product;  // GetProductInfo

        std::uint8_t  reserved_for_system_info [92];

        // 128 B boundary:
        // The following values are changed dynamically by TRIMCORE Service (if installed)

        ULONGLONG     memory;   // maximum installed physical memory or upper limit for dynamic memory from Hyper-V
        enum class Memory : std::uint8_t {
            Unknown              = 0, // unresolved yet, or all methods of checking failed
            PhysicallyInstalled  = 1, // successfully retrieved by GetPhysicallyInstalledSystemMemory and NOT a virtual machine
            DynamicVirtual       = 2, // successfully retrieved from "Hyper-V Dynamic Memory Integration Service" / "Maximum Memory, Mbytes" performance counter
            AbstractedVirtual    = 3, // fallback to GlobalMemoryStatusEx
            AssignedFixedVirtual = 4, // successfully retrieved by GetPhysicallyInstalledSystemMemory under Virtual Machine
        } memory_type;

        std::uint8_t  reserved_for_memory_info [119];
        std::uint8_t  reserved_for_additional_data [512];

        // 768 B boundary:
        // The following values require mutex locking

        std::uint32_t telemetry_invariant_offset;
        std::uint32_t telemetry_invariant_length;
        std::uint32_t telemetry_dynamic_length; // telemetry_dynamic follows immediately after telemetry_invariant

        std::uint8_t  reserved_for_indices [116];

        // Processor -> NUMA mapping

        struct NumaMap {
            std::uint16_t n;
            std::uint16_t flags;
#pragma pack(push,2)
            struct NumaMapEntry {
                std::uint16_t group;
                std::uint16_t node;
                std::uint64_t affinity;
            } entries [256];
#pragma pack(pop)
        } numa_map;

        std::uint8_t  reserved_for_additional_data2 [120];
    };

    static_assert (10 == offsetof (SystemOverview, reserved10));
    static_assert (12 == offsetof (SystemOverview, reserved12));
    static_assert (128 == offsetof (SystemOverview, memory));
    static_assert (768 == offsetof (SystemOverview, telemetry_invariant_offset));
    static_assert (4096 == sizeof (SystemOverview));

    // SessionOverview
    //  - NOT IMPLEMENTED NOT IMPLEMENTED NOT IMPLEMENTED NOT IMPLEMENTED NOT IMPLEMENTED NOT IMPLEMENTED
    //
    struct SessionOverview {
        DWORD id;       // service ID
        DWORD services; // services session ID, typically 0, non-zero for containers

        // TODO: shell paths
        // TODO: info retrieved by notification app from user32, gdi32, shell32
        // TODO: NULL for services

    }; // shared section distinguished by session ID and user

    // ProcessOverview
    //  - 
    //
    struct ProcessOverview {
        HANDLE  heap;       // GetProcessHeap
        DWORD   id;         // GetCurrentProcessId
        DWORD   parent;     // parent process ID
        DWORD   session;    // session ID
        DWORD   reserved [3];

        std::uint64_t service : 1;
        //std::uint64_t remoted : 1; // retrieved by telemetry.exe

        Rsrc::VersionInfo crt;
    };

    TRIMCORE_DLL_IMPORT const SystemOverview * os;
    TRIMCORE_DLL_IMPORT const SessionOverview * session; // NULL currently
    TRIMCORE_DLL_IMPORT const ProcessOverview * process;

    // ABI

    // IconSizeAttributes
    //  - 
    //
    struct IconSizeAttributes {
        std::uint8_t small_mul; // SM_CXSMICON
        std::uint8_t small_div;
        std::uint8_t large_mul; // SM_CXICON
        std::uint8_t large_div;
    };

    namespace Implementation {

        // ProductStrIndex
        //  - returns STRINGTABLE index in .rsrc in TRIMCORE.dll of 'product' name
        //  - if provided, 'count' receives number of next product strings in the block
        //    so that directly using [product .. product + count - 1] range is valid
        //
        TRIMCORE_DLL_IMPORT WORD TRIMCORE_APIENTRY ProductStrIndex (DWORD product, DWORD * count = NULL);

        // IconSizeAttr
        //  - 
        //
        TRIMCORE_DLL_IMPORT IconSizeAttributes TRIMCORE_APIENTRY IconSizeAttr (WPARAM type);
    }

    // GetWindowsProductName
    //  - returns approximate name of current OS edition (SKU), or empty string if not known
    //
    inline std::wstring_view GetWindowsProductName () {
        if (auto id = Implementation::ProductStrIndex (os->product)) {
            return TRIMCORE::Rsrc::String (GetModuleHandle (L"TRIMCORE"), id);
        } else
            return std::wstring_view ();
    }

    // GetWindowsProductName (DWORD)
    //  - returns approximate name of OS edition (SKU) for 'product' type code returned by GetProductInfo
    //    or empty string if not known
    //
    inline std::wstring_view GetWindowsProductName (DWORD product) {
        if (auto id = Implementation::ProductStrIndex (product)) {
            return TRIMCORE::Rsrc::String (GetModuleHandle (L"TRIMCORE"), id);
        } else
            return std::wstring_view ();
    }

    // GetIconSize
    //  - computes proper size for window icons, depending on OS version
    //  - SM_C[X|Y]ICON is no longer valid and MS doesn't bother to provide API
    //  - parameters:
    //     - type - ICON_BIG, ICON_SMALL or ICON_SMALL2
    //     - dpi - monitor DPI
    //     - dpiNULL - desktop DPI
    //
    inline SIZE GetIconSize (WPARAM type, LPARAM dpi, LPARAM dpiNULL) {
        auto small_x = dpi * GetSystemMetrics (SM_CXSMICON) / dpiNULL;
        auto small_y = dpi * GetSystemMetrics (SM_CYSMICON) / dpiNULL;
        auto large_x = dpi * GetSystemMetrics (SM_CXICON) / dpiNULL;
        auto large_y = dpi * GetSystemMetrics (SM_CYICON) / dpiNULL;

        auto attrs = Implementation::IconSizeAttr (type);
        return {
            (LONG) (attrs.small_mul * small_x / attrs.small_div + attrs.large_mul * large_x / attrs.large_div),
            (LONG) (attrs.small_mul * small_y / attrs.small_div + attrs.large_mul * large_y / attrs.large_div),
        };
    }

    inline SIZE GetIconSize (WPARAM type = ICON_BIG) {
        return GetIconSize (type, 1, 1);
    }

    // GetMsvcrtDLLs
    //  - TODO
    //
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY GetMsvcrtDLLs (HMODULE * buffer, std::size_t capacity) noexcept;

    // GetCurrentProcessorNumaNode
    //  - retrieves cached information rather than doing syscall to GetNumaProcessorNodeEx
    //
    TRIMCORE_DLL_IMPORT USHORT TRIMCORE_APIENTRY GetCurrentProcessorNumaNode () noexcept;
    
    // GetProcessorNumaNode
    //  - retrieves cached NUMA number for PROCESSOR_NUMBER rather than doing syscall to GetNumaProcessorNodeEx
    //
    TRIMCORE_DLL_IMPORT USHORT TRIMCORE_APIENTRY GetProcessorNumaNode (const PROCESSOR_NUMBER *) noexcept;

    // Describe SystemOverview::Version
    //  - TODO: short/long format
    //
    inline std::wstring Describe (SystemOverview::Version number, DescriptionFormatting * format = nullptr) {
        return Describe (Describe (number.major, format) + L"."
                       + Describe (number.minor, format) + L"."
                       + Describe (number.build, format) + L"."
                       + Describe (number.update, format), format);
    }
    inline Serialized Serialize (const SystemOverview::Version & number, Temporary64kB <std::uint8_t> &) {
        return SerializeTrivially ('OSv', number);
    }
}

#endif
