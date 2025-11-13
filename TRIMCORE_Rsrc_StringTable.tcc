#ifndef TRIMCORE_DLL_RSRC_STRING_TCC
#define TRIMCORE_DLL_RSRC_STRING_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY RsrcStr (HMODULE, UINT, LANGID, const wchar_t **) noexcept;

    // RsrcStrBlock(A/W)
    //  - attempts to find string resource block (a base ID) for the provided

    TRIMCORE_DLL_IMPORT UINT        TRIMCORE_APIENTRY RsrcStrBlockA (HMODULE, LANGID, const char *, std::size_t) noexcept;
    TRIMCORE_DLL_IMPORT UINT        TRIMCORE_APIENTRY RsrcStrBlockW (HMODULE, LANGID, const wchar_t *, std::size_t) noexcept;
}

inline std::wstring_view TRIMCORE::Rsrc::String (HMODULE module, unsigned int id, LANGID language) noexcept {
    const wchar_t * data = nullptr;
    std::size_t size = Implementation::RsrcStr (module, id, language, &data);
    return std::wstring_view (data, size);
}
inline std::wstring_view TRIMCORE::Rsrc::String (unsigned int id, LANGID language) noexcept {
    return String (NULL, id, language);
}

#endif
