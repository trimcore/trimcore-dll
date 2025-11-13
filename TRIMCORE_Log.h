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
        //  - throwing it automatically reports the message
        //  - see TRIMCORE_Log_Exception.hpp
        //
        class Exception;

        // Internal

        struct Internals; // implementation details, see TRIMCORE_Log.cpp
        struct Options;
        struct ColorScheme;
        struct Context;
        struct FailStats;
        struct LvlChecks;
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
            Dump,       // one-time detailed output to isolated file triggered by loglevel message
            Online,     // events pushed through trimcore service to trimcore servers
            // Network,      // local network server?
            FunctionalitiesCount
        };

        // TODO: Value
        //  - detailed and compressed way to report data, that enables management on user side
        //
        //template <typename T>
        //struct Value {
            // T value;
            //
            // name (how should this object be named, "Temperature") - direct string or resource ID
            // description - direct string or resource ID
            // formatting - preferred? pointer to DescriptionFormatting?
        //};

        // OverrideTimestamp
        //  - when present as parameter to log call, replaces reported timestamp with 't'
        //  - similar to passing ApiError changes {ERR}
        //
        struct OverrideTimestamp {
            Timestamp t;

        public:
            inline OverrideTimestamp (Timestamp t) : t (t) {}
            inline OverrideTimestamp (FILETIME ft) { t.ft = ft; }
            inline OverrideTimestamp (SYSTEMTIME st) { SystemTimeToFileTime (&st, &this->t.ft); }

            OverrideTimestamp (const OverrideTimestamp &) = default;
            OverrideTimestamp & operator = (const OverrideTimestamp &) = default;
        };

    private:
        Internals * const internals = nullptr;
        LvlChecks * const lvlchecks = nullptr;
        FailStats * const failures = nullptr;

    public:
        // options
        //  - reference to internal options for this log instance
        //
        Options & options;

        // counters
        //  - incremented with every processed output event, per functionality
        //  - useful for combining multiple informative events
        //
        const std::size_t * const counters; // [FunctionalitiesCount]

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
        //  - main logging operation, for example: log(Log::Warning, "warning {1:x}", 123); -> "warning `7b`"
        //  - message string parameter syntax is simple:
        //     - "{1:" means 1st parameter is taken, and passed to TRIMCORE::Describe function
        //     - "x" substring is passed as "format" parameter to that Describe function
        //     - parameters can nest, but the one nested inside must be higher number
        //        - e.g.: "{1:pad<{2}}" will work, but "{2:pad<{1}}" will NOT work
        //  - predefined parameters:
        //     - {ERR} - last error (or overriden by ApiError, if provider), both number and error message

        template <typename... Args>
        inline bool operator () (Level lvl, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool operator () (Level lvl, const Provider * object, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool operator () (Level lvl, const Context *, const Provider * object, EventID message, Args &&... args) noexcept;

        // set
        //  - sets log level for any given facility
        //  - returns previous level
        //
        Level set (Functionality, Level) noexcept;

        // dump
        //  - causes all memory log level sections to be spilled to disk with timestamp and optional comment
        //
        void dump (const wchar_t * comment = nullptr) noexcept;

        // fence
        //  - waits until all currently buffered data are output to console/files
        //  - false means allocation failure
        //
        bool fence () noexcept;

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

        // SetLogOptionsFromCommandLine
        //  - 
        //
        static inline std::size_t SetLogOptionsFromCommandLine (std::size_t argc, wchar_t ** argw);
        static inline std::size_t SetLogOptionsFromCommandLine (std::size_t argc, const wchar_t ** argw);

        // ParseLevel
        //  - attempts to parse 'string' for Log::Level
        //  - on success sets 'level' to the value and returns true; on failure returns false
        //  - supported string formats (case-insensitive):
        //     - Log::Level::Everything: "All", "Everything"
        //     - Log::Level::Trace: "Trace"
        //     - Log::Level::InfoLow: "InfoLow", "LowInfo", "Info-", 
        //     - ...
        //     - Log::Level::EventCrucial: "Event+", "EventCrucial", "EventsCrucial", "CrucialEvent", "CrucialEvents"
        //     - ...
        //     - Log::Level::Banner: "Banner", "Banners"
        //     - Log::Level::Disabled: "Off", "Disabled"
        //  - supported integer values:
        //     - 0 - disabled, ... 16 - everything
        //
        static inline bool ParseLevel (std::wstring_view string, Log::Level * level) noexcept;

    public:

        // Internals, see .tcc

        struct Report;
        struct BasicReport;
        struct BinarySegmentStringRef;
        enum class LogSystemEvents : std::uint8_t;
        enum class BinaryMessageType : std::int8_t;

        // Context
        //  - sub-object logging context
        //
        struct Context {

            // object
            //  - log-friendly object class name, ASCII characters only
            //  - points into static read-only storage, Provider's constructor ensures that
            //
            const char * object = nullptr;

            // prefix
            //  - string resource base
            //
            std::uint32_t prefix = 0;
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

        template <typename... Args>
        inline bool operator () (Level lvl, const BasicReport * report_template, EventID message, Args&&... args) noexcept;

        template <typename... Args>
        inline bool process (Level lvl, EventID, const Log::Context *, const Provider *, const BasicReport *, Args&&... args) noexcept;
        template <typename... Args>
        inline void process_data (Report *, Args&&... args) noexcept;
        template <typename... Args>
        inline void process_text (Report *, Args&&... args) noexcept;

        inline void serialize_arguments (Report *, Temporary64kB <std::uint8_t> & scratch, std::size_t index) noexcept {}
        template <typename T, typename... Args>
        inline void serialize_arguments (Report *, Temporary64kB <std::uint8_t> & scratch, std::size_t index, T && argument, Args && ...remaining) noexcept;

        template <typename T>
        inline void replace_argument (HMODULE, wchar_t *, std::size_t & length, std::size_t, T &&) noexcept;
        inline void replace_argument (HMODULE, wchar_t *, std::size_t & length, std::size_t, std::nullptr_t) noexcept;
        inline void replace_arguments (HMODULE, wchar_t *, std::size_t & length, std::size_t) noexcept  {}
        template <typename T, typename... Args>
        inline void replace_arguments (HMODULE, wchar_t *, std::size_t & length, std::size_t total, T && argument, Args && ...remaining) noexcept;

        inline void override_timestamp (Timestamp &) noexcept {}
        template <typename... Args>
        inline void override_timestamp (Timestamp & e, OverrideTimestamp ee, Args && ...remaining) noexcept;
        template <typename T, typename... Args>
        inline void override_timestamp (Timestamp & e, T &&, Args && ...remaining) noexcept;

        inline void override_api_error (ApiError &) noexcept {}
        template <typename... Args>
        inline void override_api_error (ApiError & e, ApiError ee, Args && ...remaining) noexcept;
        template <typename T, typename... Args>
        inline void override_api_error (ApiError & e, T &&, Args && ...remaining) noexcept;
    };

    // Options
    //  - optional options to Log instance; if not provided, defaults are assumed
    //  - most of these can be changed at runtime and apply immediately
    //  - part of the ABI, after generally released, all new items must be appended (or add padding before?)
    //
    struct Log::Options {
        struct HandleOutputOptions {
            USHORT codepage = CP_UTF8; // log file encoding, typically CP_UTF8
            USHORT reserved_HandleOutputOptions [7] = {};
        };

        struct LogPathOptions {

            // path components
            //  - if log/dump file cannot be created in executable directory, we use appdata folders with sub-path
            //    consisting of company/product (component is skipped if empty string (L"") is used)

            bool try_module_directory = true; // try to create log in the same directory as EXE/DLL
            bool try_common_appdata = true; // try ProgramData
            bool try_user_appdata = true; // try User/AppData/Local
            bool try_current_directory = false; // try current directory
            bool reserved_path_LogFileOptions [4] = {};
                
            const wchar_t * suffix = nullptr; // appends to log file name
            const wchar_t * company = nullptr; // nullptr == VersionInfo (NULL) [L"CompanyName"], set to L"" to skip
            const wchar_t * product = nullptr; // nullptr == VersionInfo (NULL) [L"ProductName"], set to L"" to skip
            const wchar_t * subdir = nullptr; // additional subdirectory, for Dump and Memory nullptr means "Log Dumps", set to L"" to disable
        };

        struct LogFileOptions : HandleOutputOptions {
            bool     columns = true;   // space-align to columns (dynamically auto-sized)
            bool     reserved_bool_LogFileOptions = false;
            wchar_t  separator = L' '; // separator to use when not space-aligning

            std::uint8_t reserved_LogFileOptions [28] = {};

            // TODO: max file size?, deletion policy (from beginning?)?

            LogPathOptions path;

            std::uint8_t reserved_LogFileOptions_2 [40] = {};
        };

        struct ConsoleOptions {
            COLORREF background = InheritColor;
            DWORD    reserved_ConsoleOptions [3] = {};
        };
        struct RedirectedConsoleOptions : HandleOutputOptions {
            wchar_t separator = L' ';
            std::uint16_t reserved_RedirectedConsoleOptions [7] = {};
        };

    public: // finally actual data

        const wchar_t * process = nullptr; // process name, added when two or more processes share console output

        struct : LogFileOptions {
            Sharding sharding = Sharding::None;
            bool     detailed = false;
            // TODO: option for colored symbols like U+1F534 'large red circle'.
            // TODO: option for HTML color formatting, and independend table formatting
            
            std::uint8_t reserved_file_options [30] = {};
        } file;
        
        struct : LogFileOptions {
            const wchar_t * suffix = nullptr; // nullptr means default = timestamp

            std::uint8_t reserved_dump_options [30] = {};
        } dump;

        struct : ConsoleOptions {
            struct {
                RedirectedConsoleOptions file;
                RedirectedConsoleOptions pipe;
            } redirected;

            std::uint8_t reserved_console_options [32] = {};
        } console;

        struct {

            // autodump
            //  - automatically dump memory data on event of level equal or higher
            //
            Level autodump = Disabled;// (Level) (Error + 1);

            std::uint8_t reserved_memory_options1 [7] = {};

            // capacity
            //  - memory section size limits for memory log data
            //  - leave 0 to keep defaults

            struct {
                std::uint32_t events = 0;  // 0 = 4 MB or 64 MB committed immediately
                std::uint32_t strings = 0; // 0 = 64 MB or 2 GB max, but allocated on demand
            } capacity;

            LogPathOptions dump_path;

            std::uint8_t reserved_memory_options2 [8] = {};
        } memory;

    public:
        Options () {
            this->dump.codepage = CP_UTF16;
            this->console.redirected.pipe.codepage = CP_ACP;
        }

        // per-functionality query functions

        inline USHORT codepage (Log::Functionality) const noexcept;
    };

#ifdef _WIN64
    static_assert (sizeof (Log::Options) == 512, "Change of size would violate ABI guarantees!");
#else
    static_assert (sizeof (Log::Options) == 456, "Change of size would violate ABI guarantees!");
#endif

    // Describe
    //  - converts 'Log::Level' to text
    //
    inline std::wstring Describe (Log::Level level, DescriptionFormatting * format = nullptr);
    inline std::wstring Describe (Log::Functionality func, DescriptionFormatting * format = nullptr);

    inline Serialized Serialize (const Log::Level & level, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('LogL', level); }
    inline Serialized Serialize (const Log::Functionality & func, Temporary64kB <std::uint8_t> &) { return SerializeTrivially ('LogF', func); }

    TRIMCORE_DLL_IMPORT Log log;
}

#include "TRIMCORE_Log.tcc"
#endif
