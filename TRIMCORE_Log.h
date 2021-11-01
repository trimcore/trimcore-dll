#ifndef TRIMCORE_DLL_LOG_H
#define TRIMCORE_DLL_LOG_H

#include <cstddef>
#include <cwchar>
#include <vector>

#ifndef CP_UTF16
#define CP_UTF16 1200
#endif
#ifndef CP_UTF16_BE
#define CP_UTF16_BE 1201
#endif
#ifndef CP_UTF32
#define CP_UTF32 12000
#endif
#ifndef CP_UTF32_BE
#define CP_UTF32_BE 12001
#endif

#pragma warning (disable:26812) // hint to prefer 'enum class'

namespace TRIMCORE {

    // Log
    //  - logging facility with memory mapped backlog and console output
    //
    class Log {
    public:

        // Provider
        //  - base class for simple reporting support
        //  - see TRIMCORE_Log_Provider.hpp
        //
        class Provider;

        // Exception
        //  - 
        //  - see TRIMCORE_Log_Exception.hpp
        //
        class Exception;

        // Internal

        struct Internals; // implementation details, see TRIMCORE_Log.cpp
        struct Options;
        struct ColorScheme;
        struct Context;
        class EventID; // converts various things into log mesage string

        // Level
        //  - log levels, 1...15
        //  - can be adjusted by +/-1, i.e. Error + 1 is more important than Error
        //  - use 'Everything' and 'Disabled' only as loglevel settings
        //
        enum Level : std::int8_t {
            Everything  = 0, // only for setting loglevel
            Trace       = 1, // code trace: codepath, branch, iteration, line
            InfoLow     = 2,
            Info        = 3, // regular notable cyclic behavior
            InfoSubstantial = 4,
            EventLow    = 5,
            Event       = 6, // data or user initiated event
            EventCrucial = 7,
            WarningLow  = 8,
            Warning     = 9, // invalid data is type of Warning
            WarningHigh = 10,
            ErrorLow    = 11,
            Error       = 12,
            ErrorSevere = 13,
            Stop        = 14, // above critical errors when functionality or process stops
            Banner      = 15, // displayed always - startup info, version banner, exit status
            Disabled    = 16, // only for setting loglevel, Levels above 15 will not get anywhere
        };

        // Functionality
        //  - different outputs, each has it's own log 'level', see below
        //
        enum Functionality : std::int8_t {
            Memory,     // maintains memory mapping for remote view and dump function
            Console,    // condensed output to console
            File,       // detailed output to standard log file
            Dump,       // detailed output...
            FunctionalitiesCount
        };

    private:
        Internals * internals = nullptr;

    public:
        // options
        //  - reference to internal options for this log instance
        //
        Options & options;

        // level
        //  - only reports of set level or higher are written into specified facility
        //  
        //
        Level level [FunctionalitiesCount] = {
            Everything, // Memory; do not reduce unless it significantly affects performance
            Event,      // Console
            Warning,    // File
            Disabled,   // Dump level
        };

    public:
        // Sharding
        //  - specifies when should new log file be created
        //  - IMPORTANT: works only when used with "<>" or "<???>" template in log name
        //
        enum class Sharding : std::uint8_t {
            None = 0,
            Yearly,
            Monthly,
            Daily,
        };

        // Log::Color
        //  - console color definition
        //  - if VT mode is enabled, RGB color is used, otherwise 'attr'
        //  - 'inherit' flag tells to search elsewhere or fallback to absolute default
        //
        union Color {
            std::uint32_t raw = 0xC000'0000; // default is 'inherit' and 'resolve'
            struct {
                std::uint8_t r;
                std::uint8_t g;
                std::uint8_t b;
                std::uint8_t attr : 4; // 0bIRGB used if console doesn't support VT color
                std::uint8_t reserved : 3;
                std::uint8_t inherit : 1; // ignore all other members
            };
        };

        // Options
        //  - definition below
        //
        struct Options;

        // Log constructor
        //  - name
        //     - simple name (e.g.: "user input") or full path to .log file
         //    - may contain "{D<YYYY-MM-DD>,T<HH-mm>,ms}" template; use empty "{}" for locale-based defaults
        //     - by default name of module (exe/dll) without extension is used
        //  - options, if any, are copied to internal structure which can be modified on fly
        //
        inline explicit Log (const wchar_t * name = nullptr, const Options * = nullptr);
        inline ~Log ();

#ifdef TRIMCORE_DLL_INTERNAL
        inline explicit Log (Internals *);
#endif

        // operator ()
        //  - main logging operation, for example: log(Log::Level::Warning, "warning {1:x}", 123); -> "warning `7b`"
        //  - TODO: message string parameter syntax
        //  - predefined parameters:
        //     - {ERR} - last error (or overriden by ApiError, if provider), both number and error message
        //     - {PID} - process ID (unsigned int)
        //     - {TID} - thread ID (unsigned int)
        //     - {PTR} - object pointer (Provider)
        //     - {CLASS} - object type name (provider->identity.object)
        //     - {OBJ} - object name (provider->identity.instance)
        //     - {DLL} - calling module base pointer
        //     - {T} - current date/time

        template <typename... Args>
        inline bool operator () (Level lvl, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool operator () (Level lvl, const Provider * object, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool operator () (Level lvl, const Context *, const Provider * object, EventID message, Args &&... args) noexcept;

        // dump
        //  - causes all memory log level sections to be spilled to disk with timestamp and optional comment
        //
        void dump (const wchar_t * comment = nullptr);

    public:

        // ConsoleOutputPart
        //  - names for console output components used with SetColor below

        enum class ConsoleOutputPart {
            Time,
            Process,
            Class,
            Object,
            Separator,
            Message,
            Value,
            N
        };

        // SetColor/SetBackground
        //  - set to "ColorWithAttr | (0bIRGB << 24)..." to additionally set 16-color palette
        //  - when set to 'InheritColor', the search goes as follows:
        //     - Banner (15) -> Event (6) (default)
        //     - Stop (14) -> 13 -> Error (12) -> 11 -> 10 -> Warning (9) -> 8 -> 7 -> Event (6)
        //     - Trace (1) -> 2 -> Info (3) -> 4 -> 5 -> Event (6)
        //     - levels Everything and Disabled have no effect
        //  - if background color is not set, per-log background (options) is preferred
        //
        static inline void SetColor (Level, ConsoleOutputPart, COLORREF);
        static inline void SetBackground (Level, ConsoleOutputPart, COLORREF);

        static constexpr COLORREF ColorWithAttr = 0x80'00'00'00;
        static constexpr COLORREF InheritColor = 0xE0'00'00'00;

        // SetTextDumpHistoryDepth
        //  - sets how many messages are saved in text file dump backlog
        //  - note that history depth is shared between all logs in process
        //
        static inline void SetTextDumpHistoryDepth (std::size_t);

    public:

        // Internals, see .tcc

        struct Report;
        enum class LogSystemEvents : std::intptr_t;

        // Context
        //  - sub-object logging context, enables sub-objects of virtually composed objects
        //    to provide custom string tree prefix (and class name)
        //
        struct Context {

            // object
            //  - log-friendly object class name, ASCII characters only
            //  - points into static read-only storage, Provider's constructor ensures that
            //
            const char * object = nullptr;

            // prefix
            //  - string tree prefix used when reporting by compound string ID
            //
            Rsrc::StructuredStringTable::ID prefix;
        };
        
        // Identity
        //  - hierarchy-final object identity information, used by Log::Provider
        //  - as separate sub-object to minimize overlapping with object's members
        //
        struct Identity : Context {

            // instance
            //  - short description of the object
            //     - for example: IP address for TCP connection object instances
            //  - should be updated whenever appropriate
            //  - string that start with "\x2020" represents moved-from instances
            //
            std::wstring instance;
        };

    private:
        Log (const Log &) = delete;
        Log & operator = (const Log &) = delete;

        inline Report preinitialize (Level lvl, const char * contextclass, const Provider * object, const Report * report_template) noexcept; // minimizes code bloat

        template <typename... Args>
        inline bool operator () (Level lvl, const Report * report_template, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool process (Report && report, const EventID &, Args&&... args) noexcept;

        template <typename T>
        inline void replace_argument (HMODULE, std::wstring &, std::wstring, T &&);
        inline void replace_argument (HMODULE, std::wstring &, std::wstring, std::nullptr_t);
        inline void replace_arguments (HMODULE, std::wstring &, std::size_t) {}
        template <typename T, typename... Args>
        inline void replace_arguments (HMODULE, std::wstring & string, std::size_t total, T && argument, Args && ...remaining);

        inline void override_api_error (ApiError &) {}
        template <typename... Args>
        inline void override_api_error (ApiError & e, ApiError ee, Args && ...remaining);
        template <typename T, typename... Args>
        inline void override_api_error (ApiError & e, T &&, Args && ...remaining);

        inline void remove_unwanted_codepoints (std::wstring &);
    };

    // Options
    //  - optional options to Log instance; if not provided, defaults are assumed
    //  - most of these can be changed at runtime and apply immediately
    //  - part of the ABI, after generally released, all new items must be appended (or add padding before?)
    //
    struct Log::Options {
        struct GeneralFunctionalityOptions {
            LANGID locale = 0; // default formatting of date/time templates in log name, and some other places
        };
        struct HandleOutputOptions {
            USHORT codepage = CP_UTF8; // log file encoding, typically CP_UTF8
        };

        struct LogFileOptions : HandleOutputOptions, GeneralFunctionalityOptions {
            bool     columns = true;   // space-align to columns (dynamically auto-sized)
            wchar_t  separator = L' '; // separator to use when not space-aligning

            // TODO: max file size?, deletion policy (from beginning?)?

            // path components
            //  - if log cannot be created in executable directory, we use appdata folders with sub-path
            //    consisting of company/product (component is skipped if empty string (L"") is used)

            struct {
                bool try_module_directory = true; // try to create log in the same directory as EXE/DLL
                bool try_common_appdata = true; // try ProgramData
                bool try_user_appdata = true; // try User/AppData/Local
                bool try_current_directory = false; // try current directory
                
                const wchar_t * suffix = nullptr; // appends to log file name
                const wchar_t * company = nullptr; // nullptr == VersionInfo (NULL) [L"CompanyName"], set to L"" to skip
                const wchar_t * product = nullptr; // nullptr == VersionInfo (NULL) [L"ProductName"], set to L"" to skip
            } path;
        };

        struct ConsoleOptions : GeneralFunctionalityOptions {
            COLORREF background = InheritColor;
        };
        struct RedirectedConsoleOptions : HandleOutputOptions, GeneralFunctionalityOptions {
            wchar_t separator = L' ';
        };

    public: // finally actual data

        const wchar_t * process = nullptr; // process name, added when two or more processes share console output

        struct : LogFileOptions {
            Sharding sharding = Sharding::None;
            bool     detailed = false;
        } file;
        
        struct : LogFileOptions {
            const wchar_t * suffix = nullptr; // nullptr means default = timestamp
        } dump;

        struct : ConsoleOptions {
            struct {
                RedirectedConsoleOptions file;
                RedirectedConsoleOptions pipe;
            } redirected;
        } console;

        struct : GeneralFunctionalityOptions {

            // autodump
            //  - automatically dump memory data on event of level equal or higher
            //
            Level autodump = Disabled;// (Level) (Error + 1);

            // TODO: memory size limits
        } memory;

    public:
        Options () {
            this->file.locale = LOCALE_USER_DEFAULT;
            this->console.locale = LOCALE_USER_DEFAULT;

            this->dump.codepage = CP_UTF16;
            this->console.redirected.pipe.codepage = CP_ACP;
        }

        // per-functionality query functions

        inline LCID   locale (Log::Functionality) const noexcept;
        inline USHORT codepage (Log::Functionality) const noexcept;
    };

    // Describe
    //  - converts 'Log::Level' to text
    //
    inline std::wstring Describe (Log::Level level, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (Log::Functionality func, DescriptionFormatting * format = nullptr);

    inline std::size_t DescriptionLengthEst (Log::Level) { return 12; }
    inline std::size_t DescriptionLengthEst (Log::Functionality) { return 9; }

    TRIMCORE_DLL_IMPORT Log log;
}

#include "TRIMCORE_Log.tcc"
#endif
