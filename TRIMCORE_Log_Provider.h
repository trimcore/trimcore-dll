#ifndef TRIMCORE_DLL_LOG_PROVIDER_H
#define TRIMCORE_DLL_LOG_PROVIDER_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// Log::Provider
//  - short object identification for improved logging
//  - enables quick status reporting through 'report' function to process log
//  - use virtual inheritance so that final object has only one Log::Provider
//
class TRIMCORE::Log::Provider {
    Log * logptr = nullptr;
public:
    Log::Identity identity;
   
public:

    // constructor - usage: Object::Object () : Log::Provider ( [&log], "object", { [L1],[L2],[L3],[L4],[L5],[L6] },  L"short object description");
    //  - 'object' type name stays fixed, static constant string literal required
    //  - single-level parameter is special, when L1 >= 256, then L1 is Rsrc STRINGTABLE ID offset, EventID in this->report must be < 256
    //  - optional uint8_t L1...L6 specify string tree prefix for the object
    //  - optional Log* specifies target log object; process log is used by default

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID ()) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint64_t l1_or_id_offset, std::wstring name = std::wstring ())
        : Provider (nullptr, object, l1_or_id_offset, name) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID (l1, l2)) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3)) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4)) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::uint8_t l5, std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4, l5)) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::uint8_t l5, std::uint8_t l6, std::wstring name = std::wstring ())
        : Provider (nullptr, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4, l5, l6)) {};

    // same constructors, but with different (non-main) log

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID ()) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint64_t l1_or_id_offset, std::wstring name = std::wstring ());

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID (l1, l2)) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3)) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4)) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::uint8_t l5, std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4, l5)) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint8_t l1, std::uint8_t l2, std::uint8_t l3, std::uint8_t l4, std::uint8_t l5, std::uint8_t l6, std::wstring name = std::wstring ())
        : Provider (log, object, name, Rsrc::StructuredStringTable::ID (l1, l2, l3, l4, l5, l6)) {};

private:
    friend class Exception;

    template <std::size_t N>
    inline Provider (Log *, const char (&object) [N], std::wstring name, Rsrc::StructuredStringTable::ID prefix);

protected:

    // RegisterLogProviderContext
    //  - registers additional 'classname' and overriding 'prefix' to use for "this->report (this, ...)" calls
    //  - enabled different sub-objects in inheritance hierarchies to use their own 'prefix' and differentiate them in log file
    //
    template <std::size_t N>
    static inline void RegisterLogProviderContext (const void * this_, const char (&classname) [N], Log::EventID prefix) noexcept {
        Implementation::LogSetCtx (this_, classname, prefix);
    }
    static inline void ReleaseLogProviderContext (const void * this_) noexcept {
        Implementation::LogClrCtx (this_);
    }

public:

    // move semantics, moved-from objects report on log level 'Info - 1' instead 'Info'

    inline Provider (Provider && from) noexcept;
    inline Provider (const Provider & from);

    inline Provider & operator = (Provider && from) noexcept;
    inline Provider & operator = (const Provider & from);

    inline ~Provider () noexcept;

    // report
    //  - calls log with additional information about this object
    //  - usage: this->report (Log::Error, 1, ...), always returns false
    //
    template <typename... Args>
    bool report (Log::Level, Log::EventID, Args &&...) const noexcept;

    // report
    //  - calls log with additional information about this object, and explicit Context for subobject
    //  - usage: this->report (context, Log::Error, 1, ...), always returns false
    //
    template <typename... Args>
    bool report (const Log::Context &, Log::Level, Log::EventID, Args &&...) const noexcept;

    // report
    //  - calls log with additional information about this object, and 'this' context for subobject
    //  - usage: this->report (this, Log::Error, 1, ...), always returns false
    //
    template <typename... Args>
    bool report (const void * context, Log::Level, Log::EventID, Args &&...) const noexcept;
};

#include "TRIMCORE_Log_Provider.tcc"
#endif
