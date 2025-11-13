#ifndef TRIMCORE_DLL_DESCRIBE_ADDRESS_H
#define TRIMCORE_DLL_DESCRIBE_ADDRESS_H

#include <ws2def.h>
#include <ws2ipdef.h>
//#include <ws2bth.h>

#include <afunix.h>
//#include <af_irda.h>
#include <hvsocket.h>

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY DescAddr (wchar_t * buffer, std::size_t length, DescriptionFormatting * fmt, const void * data) noexcept;

    inline std::wstring DescribeAddress (const void * data, DescriptionFormatting * format) {
        Temporary64kB <wchar_t> buffer;
        auto length = Implementation::DescAddr (buffer.data (), buffer.size (), format, data);
        return std::wstring (buffer.data (), length);
    }
}

namespace TRIMCORE {

    // Describe (socket address)
    //  - displays socket address
    //  - supports: IPv6, IPv4, Hyper-V socket, and Unix socket
    //  - parameters:
    //     - "GUID" - for AF_HYPERV, spell out known GUIDs, doen't shorten to "parent", "broadcast", "loopback", "silohost" and "children"
    //     - 
    //  - after conversion, the string is internally passed to Describe (string) for case change, padding, trimming, etc.
    
#define DEFINE_DESCRIBE_FOR_SOCKADDR(T) \
    inline std::wstring Describe (const T & value, DescriptionFormatting * format = nullptr) {  \
        return Implementation::DescribeAddress (&value, format);                                \
    }                                                                                           \
    inline std::wstring Describe (const T * value, DescriptionFormatting * format = nullptr) {  \
        return Implementation::DescribeAddress (value, format);                                 \
    }                                                                                           \
    inline std::wstring Describe (T * value, DescriptionFormatting * format = nullptr) {        \
        return Implementation::DescribeAddress (value, format);                                 \
    }

    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr);
    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr_gen);
    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr_storage);
    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr_storage_xp);
    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr_in);
    DEFINE_DESCRIBE_FOR_SOCKADDR (sockaddr_in6);
    DEFINE_DESCRIBE_FOR_SOCKADDR (SOCKADDR_HV);
    DEFINE_DESCRIBE_FOR_SOCKADDR (SOCKADDR_UN);
    // DEFINE_DESCRIBE_FOR_SOCKADDR (SOCKADDR_BTH);
    // DEFINE_DESCRIBE_FOR_SOCKADDR (SOCKADDR_IRDA);

#undef DEFINE_DESCRIBE_FOR_SOCKADDR

    inline std::wstring Describe (const in_addr & addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in value = { AF_INET, 0, addr };
        return Implementation::DescribeAddress (&value, format);
    }
    inline std::wstring Describe (const in_addr * addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in value = { AF_INET, 0, *addr };
        return Implementation::DescribeAddress (&value, format);
    }
    inline std::wstring Describe (in_addr * addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in value = { AF_INET, 0, *addr };
        return Implementation::DescribeAddress (&value, format);
    }

    inline std::wstring Describe (const in6_addr & addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in6 value = { AF_INET6, 0, 0, addr, 0 };
        return Implementation::DescribeAddress (&value, format);
    }
    inline std::wstring Describe (const in6_addr * addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in6 value = { AF_INET6, 0, 0, *addr, 0 };
        return Implementation::DescribeAddress (&value, format);
    }
    inline std::wstring Describe (in6_addr * addr, DescriptionFormatting * format = nullptr) {
        sockaddr_in6 value = { AF_INET6, 0, 0, *addr, 0 };
        return Implementation::DescribeAddress (&value, format);
    }
}

#endif

