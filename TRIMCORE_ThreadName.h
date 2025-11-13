#ifndef TRIMCORE_DLL_THREADNAME_H
#define TRIMCORE_DLL_THREADNAME_H

namespace TRIMCORE {
    namespace Implementation {
        TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY SetThreadNameImpl (HANDLE h, const wchar_t * name) noexcept;
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY GetThreadNameImpl (HANDLE h, wchar_t * buffer, std::size_t length) noexcept;
    }

    // SetThreadName
    //  - 
    //
    inline bool SetThreadName (HANDLE h, const wchar_t * name) noexcept {
        return Implementation::SetThreadNameImpl (h, name);
    }
    
    // SetThreadName
    //  - sets current thread description
    //
    inline bool SetThreadName (const wchar_t * name) noexcept {
        return Implementation::SetThreadNameImpl (GetCurrentThread (), name);
    }

    // SetThreadName
    //  - opens thread handle and attempts to set it's description
    //
    inline bool SetThreadName (DWORD id, const wchar_t * name) noexcept {
        bool r = false;
        if (auto h = OpenThread (THREAD_SET_LIMITED_INFORMATION, FALSE, id)) {
            r = Implementation::SetThreadNameImpl (h, name);
            CloseHandle (h);
        }
        return r;
    }

    // GetThreadName
    //  - retrieves thread description either from Windows or internal map
    //  - NUL-terminates ONLY if there is extra space for the character in the buffer
    //
    inline std::size_t GetThreadName (HANDLE h, wchar_t * buffer, std::size_t length) noexcept {
        return Implementation::GetThreadNameImpl (h, buffer, length);
    }

#ifdef TRIMCORE_DLL_LOCAL
    std::wstring GetThreadName (HANDLE h);
#else
    inline std::wstring GetThreadName (HANDLE h) {
        Temporary64kB <wchar_t> buffer;
        return std::wstring (buffer.data (), GetThreadName (h, buffer.data (), buffer.size ()));
    }
#endif

    inline std::wstring GetThreadName (DWORD id) {
        std::wstring s;
        if (auto h = OpenThread (THREAD_QUERY_LIMITED_INFORMATION, FALSE, id)) {
            s = GetThreadName (h);
            CloseHandle (h);
        }
        return s;
    }

    inline std::wstring GetThreadName () {
        return GetThreadName (GetCurrentThread ());
    }
}

#endif
