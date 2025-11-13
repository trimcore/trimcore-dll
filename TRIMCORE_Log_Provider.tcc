#ifndef TRIMCORE_DLL_LOG_PROVIDER_TCC
#define TRIMCORE_DLL_LOG_PROVIDER_TCC

#include "TRIMCORE.h"

// constructors

template <std::size_t N>
inline TRIMCORE::Log::Provider::Provider (Log * logptr, const char (&object) [N], std::uint32_t rsrc_offset, std::wstring name)
    : logptr (logptr)
    , identity { object, rsrc_offset, std::move (name) } {

    this->report (identity.instance.empty () ? Trace : InfoLow, Log::LogSystemEvents::ObjectConstructed);
}

// move and copy

inline TRIMCORE::Log::Provider::Provider (Provider && from) noexcept {
    auto level = Log::Level::InfoLow;
    if (this->identity.instance.empty ()) {
        level = Log::Level::Trace;
    }

    from.report (level, Log::LogSystemEvents::ObjectMovedFrom);
    this->logptr = from.logptr;
    this->report (level, Log::LogSystemEvents::ObjectMovedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;
    from.identity.instance.insert (0, 1, L'\x2020');
}

inline TRIMCORE::Log::Provider::Provider (const Provider & from) {
    auto level = Log::Level::InfoLow;
    if (this->identity.instance.empty ()) {
        level = Log::Level::Trace;
    }

    from.report (level, Log::LogSystemEvents::ObjectCopiedFrom);

    this->logptr = from.logptr;
    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;

    this->report (level, Log::LogSystemEvents::ObjectCopiedTo);
}

inline TRIMCORE::Log::Provider & TRIMCORE::Log::Provider::operator = (Provider && from) noexcept {
    auto level = Log::Level::InfoLow;
    if (this->identity.instance.empty ()) {
        level = Log::Level::Trace;
    }

    from.report (level, Log::LogSystemEvents::ObjectMovedFrom);
    this->logptr = from.logptr;
    this->report (level, Log::LogSystemEvents::ObjectMovedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;
    from.identity.instance.insert (0, 1, L'\x2020');

    return *this;
}

inline TRIMCORE::Log::Provider & TRIMCORE::Log::Provider::operator = (const Provider & from) {
    auto level = Log::Level::InfoLow;
    if (this->identity.instance.empty ()) {
        level = Log::Level::Trace;
    }

    from.report (level, Log::LogSystemEvents::ObjectCopiedFrom);
    this->logptr = from.logptr;
    this->report (level, Log::LogSystemEvents::ObjectCopiedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;

    return *this;
}

inline TRIMCORE::Log::Provider::~Provider () noexcept {
    this->report (identity.instance.empty () ? Trace : InfoLow, Log::LogSystemEvents::ObjectDestroyed);
    Implementation::LogFlush (this->logptr->internals, this);
}

// actual reporting forwards

template <typename... Args> __forceinline inline
bool TRIMCORE::Log::Provider::report (Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    return (*this->logptr) (level, nullptr, this, event, std::forward <Args> (args)...);
}

template <typename... Args> __forceinline inline
bool TRIMCORE::Log::Provider::report (const Log::Context & context, Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    return (*this->logptr) (level, &context, this, event, std::forward <Args> (args)...);
}

template <typename... Args> __forceinline inline
bool TRIMCORE::Log::Provider::report (const void * this_, Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    Log::Context ctx;
    Log::Context * pctx = &ctx;
    if (!Implementation::LogGetCtx (this_, &ctx.object, NULL, &ctx.prefix)) {
        pctx = nullptr;
    }
    return (*this->logptr) (level, pctx, this, event, std::forward <Args> (args)...);
}

#endif
