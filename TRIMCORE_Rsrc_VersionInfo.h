#ifndef TRIMCORE_DLL_RSRC_VERSIONINFO_H
#define TRIMCORE_DLL_RSRC_VERSIONINFO_H

#include <winver.h>
#include <cstdint>

namespace TRIMCORE::Implementation {
    struct StringSet {
        const wchar_t * data;
        std::uint16_t   size;
    };

    TRIMCORE_DLL_IMPORT const void * VerInfo (HMODULE, StringSet *);
    TRIMCORE_DLL_IMPORT const wchar_t * VerStrName (const StringSet *, const wchar_t *);
    TRIMCORE_DLL_IMPORT const wchar_t * VerStrIndex (const StringSet *, unsigned int);
    TRIMCORE_DLL_IMPORT bool VerInfoIsValid (const void *);
}

namespace TRIMCORE::Rsrc {

    // VersionNumber
    //  - layout of file/product semantic version numbers stored in VS_FIXEDFILEINFO
    //  - TODO: string parsing?
    //
    struct VersionNumber {
        std::uint16_t minor;
        std::uint16_t major;
        std::uint16_t build;
        std::uint16_t patch; // also known as 'release' number

    public:
        std::uint64_t as_number () const {
            return (std::uint64_t (this->major) << 48)
                 | (std::uint64_t (this->minor) << 32)
                 | (std::uint64_t (this->patch) << 16)
                 | (std::uint64_t (this->build) << 0);
        }
        bool operator == (const VersionNumber & other) const {
            return *reinterpret_cast <const std::uint64_t *> (this)
                == *reinterpret_cast <const std::uint64_t *> (&other);
        }
        bool operator < (const VersionNumber & other) const {
            return this->as_number () < other.as_number ();
        }
    };

    union FixedVersionInfo {

        // fixed file info
        //  - access through names defined by Windows API

        VS_FIXEDFILEINFO ffi;

        // named structure members
        //  - more descriptive access through simply named members

        struct {
            std::uint32_t header [2];

            VersionNumber file;
            VersionNumber product;

            struct {
                std::uint32_t mask;
                std::uint32_t file;
                std::uint32_t os;
                std::uint32_t type;
                std::uint32_t subtype;
            } flags;

            // timestamp
            //  - big endian, otherwise a FILETIME number

            struct {
                std::uint32_t high;
                std::uint32_t low;
            } timestamp;
        };
    };

    // VersionInfo
    //  - provides quick and easy access to VERSIONINFO resource of the module
    //  - searches for RT_VERSION block in .rsrc section and simply maps pointers here
    //  - better than 'GetFileVersionInfo' which loads the module and copies all the data
    //
    class VersionInfo {
        Implementation::StringSet strings;
        const void * ffi;
        
    public:
        inline explicit VersionInfo (HMODULE hModule)
            : ffi (Implementation::VerInfo (hModule, &strings)) {}

        VersionInfo (const VersionInfo &) = default;

        // valid
        //  - retrieves if the info is placeholder (false) or actually valid (true)
        //
        inline bool valid () const {
            return Implementation::VerInfoIsValid (this->ffi);
        }

        // access to fixed file info
        //  - version->file.major etc...
        //
        inline const FixedVersionInfo * operator -> () const {
            return static_cast <const FixedVersionInfo *> (this->ffi);
        }

        // access strings
        //  - version [L"CompanyName"]; or version [1]
        //  - returns NULL if no such string found

        inline const wchar_t * operator [] (const wchar_t * name) const {
            return Implementation::VerStrName (&this->strings, name);
        }
        inline const wchar_t * operator [] (unsigned int index) const {
            return Implementation::VerStrIndex (&this->strings, index);
        }
    };
}

#endif
