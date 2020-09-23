#ifndef TRIMCORE_DLL_RSRC_STRING_TCC
#define TRIMCORE_DLL_RSRC_STRING_TCC

namespace TRIMCORE::Implementation {
    TRIMCORE_DLL_IMPORT std::size_t RsrcStr (HMODULE, UINT, LANGID, const wchar_t **) noexcept;
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
