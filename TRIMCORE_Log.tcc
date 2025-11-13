#ifndef TRIMCORE_DLL_LOG_TCC
#define TRIMCORE_DLL_LOG_TCC

#include "TRIMCORE.h"

namespace TRIMCORE::Implementation {

    // private log implementation details
    //  - static exports to maintain ABI stability

    TRIMCORE_DLL_IMPORT Log::Internals * TRIMCORE_APIENTRY LogInit (HMODULE, void *, const wchar_t *, const Log::Options *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT void *           TRIMCORE_APIENTRY LogInitPtr (Log::Internals *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT Log::Report *    TRIMCORE_APIENTRY LogSnap (Log::Internals *, Log::Level lvl, Log::EventID event,
                                                                    const Log::Context * context, const Log::Provider * object, const Log::BasicReport * report_template,
                                                                    HMODULE) noexcept;
    TRIMCORE_DLL_IMPORT wchar_t *        TRIMCORE_APIENTRY LogInitMsg (const Log::Report * report, std::size_t *) noexcept;
    TRIMCORE_DLL_IMPORT void             TRIMCORE_APIENTRY LogInitBin (Log::Internals *, Log::Report * report, const Log::Context * context) noexcept;
    TRIMCORE_DLL_IMPORT std::size_t      TRIMCORE_APIENTRY LogFmtOff (const wchar_t * string, std::size_t length, std::size_t start, std::size_t name,
                                                                      std::size_t * count, DescriptionFormatting * fmt) noexcept;

    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY LogNested (const wchar_t * string, std::size_t offset) noexcept;
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY LogAppend (Log::Internals *, Log::Report * report, std::uint32_t type, const void * data, std::size_t size) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogRepChr (wchar_t * string, std::size_t * length, wchar_t character, wchar_t replacement) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogRepStr (wchar_t * string, std::size_t * length, std::size_t offset, std::size_t count,
                                                          const wchar_t * rep_data, std::size_t rep_length, const DescriptionFormatting * fmt) noexcept;

    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogFree (Log::Internals *, HMODULE) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogText (Log::Internals *, Log::Report *, const wchar_t *) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogData (Log::Internals *, Log::Report *, const void *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogDump (Log::Internals *, Log::Report *, HMODULE, const wchar_t *, bool memory, bool file) noexcept;
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY LogFlush (Log::Internals *, Log::Provider * this_) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogArgExcept (Log::Internals *, std::size_t type, std::size_t, const char *) noexcept;
    TRIMCORE_DLL_IMPORT UINT TRIMCORE_APIENTRY LogGetString (std::size_t type, std::size_t index, const wchar_t **) noexcept;
    TRIMCORE_DLL_IMPORT Log::Level TRIMCORE_APIENTRY LogSetLevel (Log::Internals *, Log::Functionality, Log::Level) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogSetParam (void *, std::size_t, std::size_t, std::size_t value) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogSetCtx (const void * this_, const char * classname, HMODULE module, std::uint32_t prefix) noexcept;
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY LogGetCtx (const void * this_, const char ** classname, HMODULE * module, std::uint32_t * prefix) noexcept;
    TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LogClrCtx (const void * this_) noexcept;

    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY LogInitCmdLine (std::size_t argc, wchar_t ** argw) noexcept;
    TRIMCORE_DLL_IMPORT bool        TRIMCORE_APIENTRY LogLvlParse (const wchar_t * buffer, std::size_t length, Log::Level * level) noexcept;
}

// LogSystemEvents
//  - internal, for objects derived from Log::Provider
//  - IDs are part of ABI, do not change
//
enum class TRIMCORE::Log::LogSystemEvents : std::uint8_t {
    ObjectConstructed   = 0x00,
    ObjectDestroyed,
    ObjectMovedFrom,
    ObjectMovedTo,
    ObjectCopiedFrom,
    ObjectCopiedTo,

    ThreadCreated       = 0x0A,
    ThreadFinished      = 0x0B,
    Exception           = 0x0C,
    ProcessTerminating  = 0x0E,
    ExitProcess         = 0x0F,

    LogCreated          = 0x10,
    LogCreatedFile,
    LogClosed,
    LogCreateFailed,
    LogCreateFailedAlloc,
    LogCreateFailedRegistry,
    LogWriteFailed,
    LogCompressionFailed,
    LogDescribeException,
    LogProcessingFailure,

    LogDumped           = 0x1A,
    LogDumpedMain       = 0x1B,
    LogMemoryDumpFailed = 0x1C,
    LogSerializeException = 0x1D,
    LogConsoleFailed    = 0x1F,

    ExeRequirementsUnknown    = 0x20,
    ExeRequirementNotMet      = 0x21,
    ExeRequirementLevelNotMet = 0x22,
    CrtVersionNotMet          = 0x23,
    CrtVersionNotMetTRIMCORE  = 0x24,

    Max = 0x9F,
};

// EventID
//  - internal
//  - consolidates supported types of message types (either text string or form of ID)
//  - loads/decodes message template to wstring on demand
//
class TRIMCORE::Log::EventID {
public:
    enum class Type : std::uint8_t {
        RsrcStringID   = 0, // STRINGTABLE ID in 'payload'
        AString        = 1, // char* string pointer in 'payload'
        WString        = 2, // wchar_t* string pointer in 'payload'
        LogSystemEvent = 3,
        AStringView    = 4, // not yet implemented
        WStringView    = 5, // not yet implemented
        Fence          = 0xFF, // synchronization fence event, payload is HANDLE
    };

    union {
        struct {

            // payload
            //  - should fit user-mode pointer even on LA57 architecture
            //
            std::uint64_t payload : 56;

            // type
            //  - non-zero type means the 56-bit value is NOT a String Resource ID
            //  - note that the code below doesn't verify that assumption
            //
            std::uint64_t type : 8;
        };

        std::uint64_t raw;
    };
    /*union {
        std::uint16_t length;
        HANDLE        image;
    };// */

    inline EventID (Type type, std::uintptr_t data) {
        this->raw = data;
        this->type = (int) type;
    }
    inline EventID (Type type, std::intptr_t data) : EventID (type, (std::uintptr_t) data) {}
    inline EventID (const char * data)             : EventID (Type::AString, (std::intptr_t) data) {}
    inline EventID (const wchar_t * data)          : EventID (Type::WString, (std::intptr_t) data) {}
    inline EventID (LogSystemEvents data)          : EventID (Type::LogSystemEvent, (std::uintptr_t) data) {};

    inline EventID (  signed       int data) : EventID (Type::RsrcStringID,  (std::intptr_t) data) {};
    inline EventID (unsigned       int data) : EventID (Type::RsrcStringID, (std::uintptr_t) data) {};
    inline EventID (  signed      long data) : EventID (Type::RsrcStringID,  (std::intptr_t) data) {};
    inline EventID (unsigned      long data) : EventID (Type::RsrcStringID, (std::uintptr_t) data) {};
    inline EventID (  signed long long data) : EventID (Type::RsrcStringID,  (std::intptr_t) data) {};
    inline EventID (unsigned long long data) : EventID (Type::RsrcStringID, (std::uintptr_t) data) {};
        
    EventID () = default;
    EventID (const EventID &) = default;
    EventID & operator = (const EventID &) = default;
};

// FailStats
//  - ABI
//
struct TRIMCORE::Log::FailStats {
    union {
        std::uint64_t any = 0;
        struct {
            std::uint64_t serialize : 1;
            std::uint64_t describe : 1;
            std::uint64_t snap64kB : 1;
            std::uint64_t msg64kB : 1;
            std::uint64_t console : 1;
            std::uint64_t console_buffer_overflow : 1;
            std::uint64_t file : 1;
            std::uint64_t file_buffer_overflow : 1;
            std::uint64_t context : 1;
            std::uint64_t memory_buffer_overflow : 1;
            std::uint64_t memory_buffer_alloc : 1;
        };
    };
    Timestamp timestamp;
};

// LvlChecks
//  - ABI
//
struct TRIMCORE::Log::LvlChecks {
    LONG any;
    LONG data;
    LONG text;
    LONG dump;
};

// BinarySegmentStringRef
//  - references immediate string copied to string segment for memory logging
//  - 'offset' is byte-wise offset into the segment
//     - first 2 bytes are length, in characters
//  - 'utf16' is true for wide (wchar_t) string
//
struct TRIMCORE::Log::BinarySegmentStringRef {
    std::uint32_t offset : 31;
    std::uint32_t utf16 : 1;
};
enum class TRIMCORE::Log::BinaryMessageType : std::int8_t {
    RsrcString,
    SectionString,
    DynamicString,
    SystemEventCode
};

// Report
//  - internal, current status for data collection, when log event is reported
//  - 'BasicReport' is used throughout whole log() call to pass data snaphot
//  - full 'Report' is copied to memory/binary segments

struct TRIMCORE::Log::BasicReport {
    EventID          event;
    HMODULE          image;

    const Provider * object = nullptr;
    const char *     classname = nullptr;
    const char *     contextclassname = nullptr;
    
    Level            level = Everything;
    Level            levels [7] = { Everything };

    // binary segment event header

    Timestamp        timestamp;
    ApiError         error;
    DWORD            thread;
 };

struct TRIMCORE::Log::Report
     : TRIMCORE::Log::BasicReport {

    struct BinarySegmentPart {
        std::int8_t level : 4;
        std::int8_t message_type : 4; // BinaryMessageType
        std::int8_t params_count;
        WORD        dll; // index to DllEntries for RsrcString & SectionString, main EXE is always 0

        BinarySegmentStringRef object;
        BinarySegmentStringRef classname;
        BinarySegmentStringRef contextclassname;

        union {
            struct {
                WORD id;
                WORD base;
            } rsrc;
            struct {
                std::uint32_t offset : 31;
                std::uint32_t utf16 : 1;

                WORD length; // string length
                WORD number; // section number/index, WAS: char name [IMAGE_SIZEOF_SHORT_NAME];
            } section;

            BinarySegmentStringRef  string; // into bump-allocated second memory section
            Log::LogSystemEvents    system;
        } message;

        std::int32_t size;
    } binary;

    std::int32_t reserved_for_alignment;

    union {
        std::uint8_t inline_buffer [65536 - (sizeof (BasicReport) + sizeof (BinarySegmentPart) + sizeof reserved_for_alignment)];
        std::uint8_t * allocated_buffer;
    } parameters;
};

static_assert (sizeof (TRIMCORE::Log::Report) == 65536);

template <typename... Args> __forceinline inline
bool TRIMCORE::Log::operator () (Log::Level lvl, Log::EventID message, Args&&... args) noexcept {
    return this->process (lvl, message, nullptr, nullptr, nullptr, args...);
}
template <typename... Args> __forceinline inline
bool TRIMCORE::Log::operator () (Log::Level lvl, const Log::Provider * object, Log::EventID message, Args&&... args) noexcept {
    return this->process (lvl, message, nullptr, object, nullptr, args...);
}
template <typename... Args> __forceinline inline
bool TRIMCORE::Log::operator () (Log::Level lvl, const Log::Context * context, const Log::Provider * object, Log::EventID message, Args &&... args) noexcept {
    return this->process (lvl, message, context, object, nullptr, args...);
}
template <typename... Args> __forceinline inline
bool TRIMCORE::Log::operator () (Log::Level lvl, const Log::BasicReport * report_template, Log::EventID message, Args&&... args) noexcept {
    return this->process (lvl, message, nullptr, nullptr, report_template, args...);
}

inline TRIMCORE::Log::Log (const wchar_t * name, const Options * options)
    : internals (TRIMCORE::Implementation::LogInit ((HMODULE) &__ImageBase, this, name, options, sizeof (Options)))
    , options (*(Log::Options *)   TRIMCORE::Implementation::LogInitPtr (internals, 1))
    , lvlchecks((Log::LvlChecks *) TRIMCORE::Implementation::LogInitPtr (internals, 2))
    , counters ((std::size_t *)    TRIMCORE::Implementation::LogInitPtr (internals, 3))
    , failures ((Log::FailStats *) TRIMCORE::Implementation::LogInitPtr (internals, 4)) {

    if (!this->internals)
        throw std::bad_alloc (); // Log::Exception
}

inline TRIMCORE::Log::~Log () {
    TRIMCORE::Implementation::LogFlush (this->internals, nullptr);
    TRIMCORE::Implementation::LogFree (this->internals, (HMODULE) &__ImageBase);
}

inline TRIMCORE::Log::Level TRIMCORE::Log::set (Functionality func, Level level) noexcept {
    return TRIMCORE::Implementation::LogSetLevel (this->internals, func, level);
}

inline void TRIMCORE::Log::dump (const wchar_t * comment) noexcept {
    TRIMCORE::Implementation::LogDump (this->internals, nullptr, (HMODULE) &__ImageBase, comment, true, true);
}

inline bool TRIMCORE::Log::fence () noexcept {
    return TRIMCORE::Implementation::LogFlush (this->internals, nullptr);
}

template <typename... Args> __forceinline inline
bool TRIMCORE::Log::process (Level lvl, EventID event,
                             const Log::Context * context, const Provider * object, const BasicReport * report_template,
                             Args&&... args) noexcept {

    if (_bittest (&this->lvlchecks->any, lvl)) {

        // TODO: allocate report here, move all LogSnap args assignments that are plainly assigned here
        //       because it will take less space and instructions than just putting them on stack to call LogSnap

        auto report = TRIMCORE::Implementation::LogSnap (this->internals, lvl, event, context, object, report_template, (HMODULE) &__ImageBase);
        if (!report)
            return false;

        if (_bittest (&this->lvlchecks->data, lvl)) {
            TRIMCORE::Implementation::LogInitBin (this->internals, report, context);
            this->process_data (report, args...);
        }
        if (_bittest (&this->lvlchecks->text, lvl)) {
            this->process_text (report, args...);
        }
        if (_bittest (&this->lvlchecks->dump, lvl)) {
            TRIMCORE::Implementation::LogDump (this->internals, report, (HMODULE) &__ImageBase, nullptr,
                                               report->level >= this->options.memory.autodump,
                                               report->level >= report->levels [Log::Dump]);
        }

        TRIMCORE::Free64kB (report);
    }
    return false;
}

template <typename... Args> inline
void TRIMCORE::Log::process_data (Report * report, Args&&... args) noexcept {
    auto n = sizeof... (args);
    if (n > 255) {
        n = 255;
    }

    //TRIMCORE::Implementation::LogInitData (

    report->binary.params_count = (std::uint8_t) n;

    std::memset (report->parameters.inline_buffer, 0, sizeof report->parameters.inline_buffer);

    Temporary64kB <std::uint8_t> scratch;
    this->serialize_arguments (report, scratch, 0, args...);

    if (report->binary.size <= sizeof report->parameters.inline_buffer) {
        TRIMCORE::Implementation::LogData (this->internals, report, report->parameters.inline_buffer, report->binary.size);
    } else {
        TRIMCORE::Implementation::LogData (this->internals, report, report->parameters.allocated_buffer, report->binary.size);
    }
    if (report->binary.size > sizeof report->parameters.inline_buffer) {
        HeapFree (TRIMCORE::process->heap, 0, report->parameters.allocated_buffer);
    }
}
template <typename... Args> inline
void TRIMCORE::Log::process_text (Report * report, Args&&... args) noexcept {

    std::size_t length = 0;
    auto string = TRIMCORE::Implementation::LogInitMsg (report, &length);

    this->override_timestamp (report->timestamp, args...);
    this->override_api_error (report->error, args...);

    if (string) {
        TRIMCORE::Implementation::LogRepChr (string, &length, L'[', L'{');
        TRIMCORE::Implementation::LogRepChr (string, &length, L']', L'}');

        this->replace_arguments (report->image, string, length, sizeof... (args), args...);
        this->replace_argument (report->image, string, length, 0x0000'FFFF, report->error);

        string [length] = L'\0';
    } else {
        this->failures->msg64kB = 1;
        this->failures->timestamp = report->timestamp;
    }

    TRIMCORE::Implementation::LogText (this->internals, report, string);
    TRIMCORE::Free64kB (string);
}

template <typename T, typename... Args>
inline void TRIMCORE::Log::serialize_arguments (Report * report, Temporary64kB <std::uint8_t> & scratch, std::size_t index, T && argument, Args && ...remaining) noexcept {
    using namespace TRIMCORE;
    using namespace Implementation;

    bool next = false;
    try {
        auto serialized = Serialize (argument, scratch);
        next = LogAppend (this->internals, report, serialized.type, serialized.data, serialized.size);

    } catch (const std::exception & x) {
        next = LogAppend (this->internals, report, 0, nullptr, 0);
        LogArgExcept (this->internals, 0, index + 1, x.what ());
    } catch (...) {
        next = LogAppend (this->internals, report, 0, nullptr, 0);
        LogArgExcept (this->internals, 0, index + 1, nullptr);
    }

    if (next && (index < 255)) {
        this->serialize_arguments (report, scratch, index + 1, remaining...);
    }
}

template <typename T, typename... Args>
inline void TRIMCORE::Log::replace_arguments (HMODULE module, wchar_t * string, std::size_t & length, std::size_t total, T && argument, Args && ...remaining) noexcept {
    this->replace_arguments (module, string, length, total, remaining...);
    this->replace_argument (module, string, length, total - sizeof... (remaining), argument);
}
template <typename... Args>
inline void TRIMCORE::Log::override_timestamp (Timestamp & t, OverrideTimestamp tt, Args && ...remaining) noexcept {
    t = tt;
    this->override_timestamp (t, remaining...);
}
template <typename T, typename... Args>
inline void TRIMCORE::Log::override_timestamp (Timestamp & t, T &&, Args && ...remaining) noexcept {
    this->override_timestamp (t, remaining...);
}
template <typename... Args>
inline void TRIMCORE::Log::override_api_error (ApiError & e, ApiError ee, Args && ...remaining) noexcept {
    e = ee;
    this->override_api_error (e, remaining...);
}
template <typename T, typename... Args>
inline void TRIMCORE::Log::override_api_error (ApiError & e, T &&, Args && ...remaining) noexcept {
    this->override_api_error (e, remaining...);
}

inline void TRIMCORE::Log::replace_argument (HMODULE, wchar_t * string, std::size_t & length, std::size_t number, std::nullptr_t) noexcept {
    using namespace Implementation;

    // if argument is nullptr, just erase placeholders
    //  - TODO: check format for something like: "consume:7,15" and delete 7 chars in front and 15 after the placeholder
    //  - TODO: do the same as 'opt' ???

    std::size_t count = 0u;
    std::size_t offset = 0u;

    while ((offset = LogFmtOff (string, length, offset, number, &count, nullptr)) != std::wstring_view::npos) {
        LogRepStr (string, &length, offset, count, nullptr, 0, nullptr);
    }
}

template <typename T>
inline void TRIMCORE::Log::replace_argument (HMODULE module, wchar_t * const string, std::size_t & length, std::size_t number, T && argument) noexcept {
    using namespace std::literals;
    using namespace Implementation;

    std::size_t count = 0u;
    std::size_t offset = 0u;

    DescriptionFormatting format;
    format.module = module;

    while ((offset = LogFmtOff (string, length, offset, number, &count, &format)) != std::wstring_view::npos) {
        try {
            auto translated = Describe (argument, &format);

            LogRepStr (string, &length,
                       offset, count,
                       translated.data (), translated.size (),
                       &format);

            offset += translated.size ();
        } catch (const std::exception & x) {
            LogArgExcept (this->internals, 1, number, x.what ());
            ++offset;
        } catch (...) {
            LogArgExcept (this->internals, 1, number, nullptr);
            ++offset;
        }
    }
}

inline USHORT TRIMCORE::Log::Options::codepage (Log::Functionality func) const noexcept {
    switch (func) {
        case Log::File: return this->file.codepage;
        case Log::Dump: return this->dump.codepage;

        case Log::Console:
            switch (::console.redirection) {
                case ConsoleInfo::Redirection::File: return this->console.redirected.file.codepage;
                case ConsoleInfo::Redirection::Pipe: return this->console.redirected.pipe.codepage;
            }
    }
    return CP_UTF16;
}

inline void TRIMCORE::Log::SetColor (Level level, ConsoleOutputPart part, COLORREF c) {
    TRIMCORE::Implementation::LogSetParam (nullptr, 1, (std::size_t (level) << 8) | std::size_t (part) | 0x8000'0000, c);
}
inline void TRIMCORE::Log::SetBackground (Level level, ConsoleOutputPart part, COLORREF c) {
    TRIMCORE::Implementation::LogSetParam (nullptr, 1, (std::size_t (level) << 8) | std::size_t (part), c);
}
inline void TRIMCORE::Log::SetTextDumpHistoryDepth (std::size_t n) {
    TRIMCORE::Implementation::LogSetParam (nullptr, 2, 0, n);
}
inline std::size_t TRIMCORE::Log::SetLogOptionsFromCommandLine (std::size_t argc, wchar_t ** argw) {
    return TRIMCORE::Implementation::LogInitCmdLine (argc, argw);
}
inline std::size_t TRIMCORE::Log::SetLogOptionsFromCommandLine (std::size_t argc, const wchar_t ** argw) {
    return TRIMCORE::Implementation::LogInitCmdLine (argc, const_cast <wchar_t **> (argw));
}
inline bool TRIMCORE::Log::ParseLevel (std::wstring_view string, Log::Level * level) noexcept {
    return TRIMCORE::Implementation::LogLvlParse (string.data (), string.size (), level);
}

inline std::wstring TRIMCORE::Describe (Log::Level level, DescriptionFormatting * format) {
    const wchar_t * p = nullptr;
    if (auto n = Implementation::LogGetString (1, level, &p)) {
        return std::wstring (p, n);
    } else
        return Describe ((std::uint8_t) level, format);
}

inline std::wstring TRIMCORE::Describe (Log::Functionality func, DescriptionFormatting * format ) {
    const wchar_t * p = nullptr;
    if (auto n = Implementation::LogGetString (2, func, &p)) {
        return std::wstring (p, n);
    } else
        return Describe ((std::uint8_t) func, format);
}

#endif
