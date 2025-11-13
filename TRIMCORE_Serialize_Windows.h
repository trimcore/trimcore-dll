#ifndef TRIMCORE_DLL_SERIALIZE_WINDOWS_H
#define TRIMCORE_DLL_SERIALIZE_WINDOWS_H

namespace TRIMCORE {
    inline Serialized Serialize (HKEY hKey, Temporary64kB <std::uint8_t> & scratch) {
        Implementation::DescHKEY ((wchar_t *) scratch.data (), scratch.size () / sizeof (wchar_t), nullptr, hKey);
        return { 'HKEY', scratch.data (), std::wcslen ((wchar_t *) scratch.data ()) * sizeof (wchar_t) };
    }

    inline Serialized Serialize (const HMODULE & hModule, Temporary64kB <std::uint8_t> &) { return { 'HMOD', &hModule, sizeof hModule }; }
}

#endif

