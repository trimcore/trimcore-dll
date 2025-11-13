#ifndef TRIMCORE_DLL_APIERRORMESSAGE_H
#define TRIMCORE_DLL_APIERRORMESSAGE_H

namespace TRIMCORE {

    // ApiErrorMessage
    //  - retrieves string for error 'code' in 'language' or close enough (0 means default thread/user/system settings)
    //  - writes NUL-terminated string into 'buffer' of maximum 'length' size
    //  - returns true on success, false if no such string could've been retrieved
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY ApiErrorMessage (DWORD code, wchar_t * buffer, std::size_t length, DWORD language = 0);

    // ApiError
    //  - abstracts GetLastError into separate type
    //  - used with Log:
    //      - every log report automatically logs last error
    //      - to override (e.g. for Registry functions), pass ApiError(new_value) as log call argument
    //
    struct ApiError {
        unsigned long code;
    public:
        inline ApiError () : code (GetLastError ()) {}
        inline ApiError (DWORD code) : code (code) {}
        ApiError (const ApiError &) = default;
        ApiError & operator = (const ApiError &) = default;
    };

}

#endif
