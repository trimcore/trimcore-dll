#ifndef TRIMCORE_DLL_SYSINFO_H
#define TRIMCORE_DLL_SYSINFO_H

namespace TRIMCORE {

    // SystemOverview, global 'os' object
    //  - quick operating system overview gathered by our service
    //
    struct SystemOverview {
        union Version {
            std::uint64_t word; // 0xMM'mm'BBBB'UUUUUUUUuLL
            struct {
                std::uint32_t update; // UBR
                std::uint16_t build;
                std::uint8_t  minor;
                std::uint8_t  major;
            };
        } version;

        bool vm;
        bool invariant;
        WORD machine;
    };

    // SessionOverview
    //  - 
    //
    struct SessionOverview {
        DWORD id;

        // TODO: shell paths
        // TODO: info retrieved by notification app from user32, gdi32, shell32
        // TODO: NULL for services (nebo spustit notifier k ziskani dat, a hned ho zavrit?)

    }; // shared section distinguished by session ID and user

    // ProcessOverview
    //  - 
    //
    struct ProcessOverview {
        // GetProcessHeap
        // USHORT machine;
        // USHORT wow;
        // parent PID
        bool service;
        bool remoted;
    };


    // NtCurrentPeb()->ImageBaseAddress

    TRIMCORE_DLL_IMPORT const SystemOverview * os;
    TRIMCORE_DLL_IMPORT const SessionOverview * session;
    TRIMCORE_DLL_IMPORT const ProcessOverview * process;

    // Implementation details

    namespace Implementation::SysInfo {
        const SystemOverview * Connect ();
        const SystemOverview * CreateGlobal ();
        const SystemOverview * CreateLocal ();
        void                   Collect (SystemOverview *);
    }

    // Describe SystemOverview::Version
    //  - TODO: short/long format
    //
    inline std::wstring Describe (SystemOverview::Version number, DescriptionFormatting * format = nullptr) {
        return Describe (Describe (number.major, format) + L"."
                       + Describe (number.minor, format) + L"."
                       + Describe (number.build, format) + L"."
                       + Describe (number.update, format), format);
    }

    inline std::size_t DescriptionLengthEst (SystemOverview::Version) { return 15; }
}

#endif
