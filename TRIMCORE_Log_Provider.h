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
    Log * logptr;
public:
    Log::Identity identity;
   
public:

    // constructor - usage: Object::Object () : Log::Provider ( [&log], "object", [offset | "string.rsrc.block.name"], L"description");
    //  - 'object' type name (this->identity.object)
    //     - short class/type name displayed in log/console prefix, as the source of the event
    //     - fixed, static constant string literal required
    //  - offset | "string.rsrc.block.name" (this->identity.prefix)
    //     - defines offset of log message resource string IDs
    //     - offset - fixed uint32_t at which the appropriate STRINGTABLE IDs start
    //     - "string.rsrc.block.name" - string resource block name
    //  - description (this->identity.instance)
    //     - name of a particular object (if 'object' was "Connection" then 'description' could be "192.168.1.2:123")
    //     - can be changed at any time
    //  - optional Log* specifies the log object, into which
    //     - by default, the main process log is used

    template <std::size_t N>
    inline Provider (const char (&object) [N])
        : Provider (&::TRIMCORE::log, object, 0, {}) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::wstring description)
        : Provider (&::TRIMCORE::log, object, 0, description) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint32_t rsrc_offset)
        : Provider (&::TRIMCORE::log, object, rsrc_offset, {}) {};

    template <std::size_t N, std::size_t M>
    inline Provider (const char (&object) [N], const char (&rsrc_block_name) [M])
        : Provider (&::TRIMCORE::log, object, Implementation::RsrcStrBlockA (NULL, 0, &rsrc_block_name [0], M - 1), {}) {};

    template <std::size_t N>
    inline Provider (const char (&object) [N], std::uint32_t rsrc_offset, std::wstring description)
        : Provider (&::TRIMCORE::log, object, rsrc_offset, description) {};

    template <std::size_t N, std::size_t M>
    inline Provider (const char (&object) [N], const char (&rsrc_block_name) [M], std::wstring description)
        : Provider (&::TRIMCORE::log, object, Implementation::RsrcStrBlockA (NULL, 0, &rsrc_block_name [0], M - 1), description) {};

    // same constructors, but with different (non-main) log

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N])
        : Provider (log, object, 0, {}) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::wstring description)
        : Provider (log, object, 0, description) {};

    template <std::size_t N>
    inline Provider (Log * log, const char (&object) [N], std::uint32_t rsrc_offset)
        : Provider (log, object, rsrc_offset, {}) {};

    template <std::size_t N, std::size_t M>
    inline Provider (Log * log, const char (&object) [N], const char (&rsrc_block_name) [M])
        : Provider (log, object, Implementation::RsrcStrBlockA ((HMODULE) &__ImageBase, 0, &rsrc_block_name [0], M - 1), {}) {};

    template <std::size_t N, std::size_t M>
    inline Provider (Log * log, const char (&object) [N], const char (&rsrc_block_name) [M], std::wstring description)
        : Provider (log, object, Implementation::RsrcStrBlockA ((HMODULE) &__ImageBase, 0, &rsrc_block_name [0], M - 1), description) {};

    // the main constructor, all other forward to this one

    template <std::size_t N>
    inline Provider (Log *, const char (&object) [N], std::uint32_t rsrc_offset, std::wstring description);

protected:
    friend class Exception;

    // RegisterLogProviderContext
    //  - registers additional 'classname' and overriding 'prefix' to use for "this->report (this, ...)" calls
    //  - enabled different sub-objects in inheritance hierarchies to use their own 'prefix' and differentiate them in log file
    //
    template <std::size_t N>
    static inline void RegisterLogProviderContext (const void * this_, const char (&classname) [N], std::uint32_t prefix) noexcept {
        Implementation::LogSetCtx (this_, classname, (HMODULE) &__ImageBase, prefix);
    }
    template <std::size_t N, std::size_t M>
    static inline void RegisterLogProviderContext (const void * this_, const char (&classname) [N], const char (&rsrc_prefix_name) [M]) noexcept {
        Implementation::LogSetCtx (this_, classname,
                                   (HMODULE) &__ImageBase,
                                   Implementation::RsrcStrBlockA ((HMODULE) &__ImageBase, 0, &rsrc_prefix_name [0], M - 1));
    }
    template <std::size_t N, std::size_t M>
    static inline void RegisterLogProviderContext (const void * this_, const char (&classname) [N], const wchar_t (&rsrc_prefix_name) [M]) noexcept {
        Implementation::LogSetCtx (this_, classname,
                                   (HMODULE) &__ImageBase,
                                   Implementation::RsrcStrBlockW ((HMODULE) &__ImageBase, 0, &rsrc_prefix_name [0], M - 1));
    }
    static inline void ReleaseLogProviderContext (const void * this_) noexcept {
        Implementation::LogClrCtx (this_);
    }

public:

    // move semantics, moved-from objects report on log level 'InfoLow' instead 'Info'

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
