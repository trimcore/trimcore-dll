#ifndef TRIMCORE_DLL_LOG_PROVIDER_TCC
#define TRIMCORE_DLL_LOG_PROVIDER_TCC

// constructors

template <std::size_t N>
inline TRIMCORE::Log::Provider::Provider (Log * logptr, const char (&object) [N], std::wstring name, Rsrc::StructuredStringTable::ID prefix)
    : logptr (logptr)
    , identity { object, prefix, std::move (name) } {

    this->report (Log::Level::Info, Log::LogSystemEvents::ObjectConstructed);
}

template <std::size_t N>
inline TRIMCORE::Log::Provider::Provider (Log * logptr, const char (&object) [N], std::uint64_t l1_or_offset, std::wstring name)
    : logptr (logptr)
    , identity { object,
                 (l1_or_offset > 255) ? Rsrc::StructuredStringTable::ID ((std::uint64_t) EventID::Type::RsrcStringID, l1_or_offset)
                                      : Rsrc::StructuredStringTable::ID ((std::uint8_t) l1_or_offset),
                 std::move (name) } {

    this->report (Log::Level::Info, Log::LogSystemEvents::ObjectConstructed);
}

// move and copy

inline TRIMCORE::Log::Provider::Provider (Provider && from) noexcept {
    from.report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectMovedFrom);
    this->report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectMovedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;
    from.identity.instance.insert (0, 1, L'\x2020');
}

inline TRIMCORE::Log::Provider::Provider (const Provider & from) {
    from.report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectCopiedFrom);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;

    this->report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectCopiedTo);
}

inline TRIMCORE::Log::Provider & TRIMCORE::Log::Provider::operator = (Provider && from) noexcept {
    from.report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectMovedFrom);
    this->report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectMovedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;
    from.identity.instance.insert (0, 1, L'\x2020');

    return *this;
}

inline TRIMCORE::Log::Provider & TRIMCORE::Log::Provider::operator = (const Provider & from) {
    from.report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectCopiedFrom);
    this->report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectCopiedTo);

    this->identity.object = from.identity.object;
    this->identity.prefix = from.identity.prefix;
    this->identity.instance = from.identity.instance;

    return *this;
}

inline TRIMCORE::Log::Provider::~Provider () noexcept {
    if (this->identity.instance.empty () || this->identity.instance [0] != L'\x2020') {
        this->report (Log::Level::Info, Log::LogSystemEvents::ObjectDestroyed);
    } else {
        this->report (Log::Level (Log::Level::Info - 1), Log::LogSystemEvents::ObjectDestroyed);
    }
}

// actual reporting forwards

template <typename... Args>
inline bool TRIMCORE::Log::Provider::report (Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    if (this->logptr)
        return (*this->logptr) (level, this, this->identity.prefix + event, std::forward <Args> (args)...);
    else
        return ::TRIMCORE::log (level, this, this->identity.prefix + event, std::forward <Args> (args)...);
}

template <typename... Args>
inline bool TRIMCORE::Log::Provider::report (const Log::Context & context, Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    if (this->logptr)
        return (*this->logptr) (level, &context, this, context.prefix + event, std::forward <Args> (args)...);
    else
        return ::TRIMCORE::log (level, &context, this, context.prefix + event, std::forward <Args> (args)...);
}

template <typename... Args>
inline bool TRIMCORE::Log::Provider::report (const void * this_, Log::Level level, Log::EventID event, Args &&... args) const noexcept {
    Log::Context ctx;
    if (Implementation::LogGetCtx (this_, &ctx.object, reinterpret_cast <Log::EventID *> (&ctx.prefix))) {
        return this->report (ctx, level, event, std::forward <Args> (args)...);
    } else
        return this->report (level, event, std::forward <Args> (args)...);
}

#endif
