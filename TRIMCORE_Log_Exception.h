#ifndef TRIMCORE_DLL_LOG_EXCEPTION_H
#define TRIMCORE_DLL_LOG_EXCEPTION_H

#include <cstddef>
#include <stdexcept>

// Log::Exception
//  - 
//
class TRIMCORE::Log::Exception
    : public std::runtime_error {

public:
    const Log::Identity identity;
    const unsigned int  code;

public:

    // constructors
    //  - logs Level::Error exception code (<0C>) on 'object' (if provided)
    //     - optionally adding second Level::Error 'message' with 'parameters' (same as this->report would)
    //  - usage: throw Log::Exception ([ERROR_WHATEVER_CODE,] [this,] [message, parameters...]);
    //     - GetLastError() is used when no error code is provided

    inline explicit Exception (Log::Provider * object = nullptr);
    inline explicit Exception (DWORD error, Log::Provider * object = nullptr);

    template <typename... Args>
    inline Exception (Log::Provider * object, Log::EventID event, Args ...args);
    template <typename... Args>
    inline Exception (DWORD error, Log::Provider * object, Log::EventID event, Args ...args);

    // movable

    inline Exception (Exception && from) noexcept = default;
    inline Exception & operator = (Exception && from) noexcept = default;

private:
    static inline std::string What ();
};

#include "TRIMCORE_Log_Exception.tcc"
#endif
