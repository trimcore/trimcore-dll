#ifndef TRIMCORE_DLL_RSRC_VERSIONINFO_H
#define TRIMCORE_DLL_RSRC_VERSIONINFO_H

#include <winver.h>
#include <cstdint>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT const void * TRIMCORE_APIENTRY VerInfo (HMODULE, const wchar_t **, std::size_t *);
    TRIMCORE_DLL_IMPORT const wchar_t * TRIMCORE_APIENTRY VerStrName (const wchar_t *, std::size_t, const wchar_t *);
    TRIMCORE_DLL_IMPORT const wchar_t * TRIMCORE_APIENTRY VerStrIndex (const wchar_t *, std::size_t, unsigned int);
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY VerInfoIsValid (const void *);
}

namespace TRIMCORE::Rsrc {

    // VersionNumber
    //  - layout of file/product semantic version numbers stored in VS_FIXEDFILEINFO
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
        //
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
            //
            struct {
                std::uint32_t high;
                std::uint32_t low;

                operator FILETIME () const { return { this->low, this->high }; }
            } timestamp;
        };
    };

    // VersionInfo
    //  - provides quick and easy access to VERSIONINFO resource of the module
    //  - searches for RT_VERSION block in .rsrc section and simply maps pointers here
    //  - better than 'GetFileVersionInfo' which loads the module and copies all the data
    //
    class VersionInfo {
        const wchar_t * data;
        std::size_t     size;
        const void *    ffi;

    public:
        inline VersionInfo ()
            : data (nullptr), size (0), ffi (nullptr) {}
        inline explicit VersionInfo (HMODULE hModule)
            : ffi (Implementation::VerInfo (hModule, &this->data, &this->size)) {}

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
            return Implementation::VerStrName (this->data, this->size, name);
        }
        inline const wchar_t * operator [] (unsigned int index) const {
            return Implementation::VerStrIndex (this->data, this->size, index);
        }
    };
}

#endif
