#ifndef TRIMCORE_DLL_CONSOLEINFO_H
#define TRIMCORE_DLL_CONSOLEINFO_H

#include <cstdint>

#pragma warning (disable:26812) // hint to prefer 'enum class'

namespace TRIMCORE {

    // ConsoleInfo
    //  - handle and possible state of console output redirection
    //
    struct ConsoleInfo {
        enum Redirection : std::uint8_t {
            None = 0, // not redirected, output displayed on console
            File = 1, // redirected to disk file
            Pipe = 3, // redirected to pipe (other program)
            NoConsole = 0xFF // the process is not attached to any console
        };

    public:
        HANDLE      handle = NULL;
        Redirection redirection = None;
        bool        vt = false;
        bool        cmdexe = false; // true if the console is shared with 'cmd.exe'
        DWORD       processes = 0; // number of processes that share the console
        COLORREF    colors [16] = { 0 };

    public:
        // update
        //  - refreshes public variables according to current process state
        //
        inline void update () noexcept;

        // redirected
        //  - returns false, when console output goes to console or nowhere, otherwise true
        //
        inline bool redirected () const noexcept {
            return this->redirection != None
                && this->redirection != NoConsole;
        }
    };

    // Describe
    //  - converts 'ConsoleInfo::Redirection' to text
    //
    inline std::wstring Describe (ConsoleInfo::Redirection r, DescriptionFormatting * format = nullptr) {
        switch (r) {
            case ConsoleInfo::None: return Describe (L"-", format);
            case ConsoleInfo::File: return Describe (L"file", format);
            case ConsoleInfo::Pipe: return Describe (L"pipe", format);
            case ConsoleInfo::NoConsole: return Describe (L"null", format);
        }
        return Describe ((std::uint8_t) r, format);
    }

    // Describe ConsoleInfo
    //  - describe current console info, handle value, vt support and redirection status
    //
    inline std::wstring Describe (const ConsoleInfo & info, DescriptionFormatting * format = nullptr) {
        auto s = Describe ((const void *) info.handle, format);
        if (info.redirection != ConsoleInfo::None) {
            s += L" (" + Describe (info.redirection, format) + L")";
        }
        s += L", vt:" + Describe (info.vt, format);
        return s;
    }

    // EnableConsoleVTMode
    //  - attempts to enable VT mode for RGB colors in console
    //  - returns true if supported and successfully enabled
    //
    TRIMCORE_DLL_IMPORT bool EnableConsoleVTMode ();

    // UpdateConsoleInfo
    //  - 
    //
    TRIMCORE_DLL_IMPORT void UpdateConsoleInfo (ConsoleInfo *) noexcept;
}

// console
//  - global instance of ConsoleInfo, only one is ever required
//
TRIMCORE_DLL_IMPORT TRIMCORE::ConsoleInfo console;

// forwarding call

inline void TRIMCORE::ConsoleInfo::update () noexcept {
    return TRIMCORE::UpdateConsoleInfo (this);
}

#endif
