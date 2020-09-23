#ifndef TRIMCORE_DLL_LOG_EXCEPTION_TCC
#define TRIMCORE_DLL_LOG_EXCEPTION_TCC

inline std::string TRIMCORE::Log::Exception::What () { return std::string (); }

inline TRIMCORE::Log::Exception::Exception (Log::Provider * object)
    : std::runtime_error (What ())
    , identity (object ? object->identity : Identity {})
    , code (GetLastError ()) {

    if (object) {
        object->report (Log::Error, Log::LogSystemEvents::Exception);
    } else {
        ::TRIMCORE::log (Log::Error, Log::LogSystemEvents::Exception);
    }
}

inline TRIMCORE::Log::Exception::Exception (DWORD code, Log::Provider * object)
    : std::runtime_error (What ())
    , identity (object ? object->identity : Identity {})
    , code (code) {

    if (object) {
        object->report (Log::Error, Log::LogSystemEvents::Exception, ApiError (code));
    } else {
        ::TRIMCORE::log (Log::Error, Log::LogSystemEvents::Exception, ApiError (code));
    }
}

template <typename... Args>
inline TRIMCORE::Log::Exception::Exception (Log::Provider * object, Log::EventID event, Args ...args)
    : std::runtime_error (What ())
    , identity (object ? object->identity : Identity {})
    , code (GetLastError ()) {

    if (object) {
        object->report (Log::Error, event, args...);
        object->report (Log::Error, Log::LogSystemEvents::Exception);
    } else {
        ::TRIMCORE::log (Log::Error, event, args...);
        ::TRIMCORE::log (Log::Error, Log::LogSystemEvents::Exception);
    }
}
template <typename... Args>
inline TRIMCORE::Log::Exception::Exception (DWORD code, Log::Provider * object, Log::EventID event, Args ...args)
    : std::runtime_error (What ())
    , identity (object ? object->identity : Identity {})
    , code (code) {

    if (object) {
        object->report (Log::Error, event, args..., ApiError (code));
        object->report (Log::Error, Log::LogSystemEvents::Exception, ApiError (code));
    } else {
        ::TRIMCORE::log (Log::Error, event, args..., ApiError (code));
        ::TRIMCORE::log (Log::Error, Log::LogSystemEvents::Exception, ApiError (code));
    }
}

#endif
