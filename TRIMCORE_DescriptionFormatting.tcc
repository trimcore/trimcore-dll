#ifndef TRIMCORE_DLL_DESCRIPTIONFORMATTING_TCC
#define TRIMCORE_DLL_DESCRIPTIONFORMATTING_TCC

#include <charconv>

namespace TRIMCORE::Implementation {

    // private implementation details
    //  - static exports to maintain ABI stability

    TRIMCORE_DLL_IMPORT std::uint16_t TRIMCORE_APIENTRY FmtUnpack (TRIMCORE::DescriptionFormatting::UnpackedParameter * buffer, const wchar_t * data, std::size_t size);
    TRIMCORE_DLL_IMPORT std::int64_t  TRIMCORE_APIENTRY FmtCvt (const wchar_t * data, std::size_t size, std::int64_t default_);
}

inline void TRIMCORE::DescriptionFormatting::ensure () const {
    if (!this->unpacked_data.allocated ()) {
        this->unpack ();
    }
}
inline void TRIMCORE::DescriptionFormatting::unpack () const {
    this->unpacked_size = TRIMCORE::Implementation::FmtUnpack (this->unpacked_data.data (), this->string, this->length);
}

inline bool TRIMCORE::DescriptionFormatting::get (std::wstring_view name, std::wstring_view * value) const {
    this->ensure ();
    for (auto & [n, v] : *this) {
        if (std::wstring_view (this->string + n.offset, n.length) == name) {
            if (value) {
                *value = std::wstring_view (this->string + v.offset, v.length);
            }
            return true;
        }
    }
    return false;
}

template <std::size_t N>
inline bool TRIMCORE::DescriptionFormatting::get (const std::wstring_view (&names) [N], std::wstring_view * value, std::wstring_view * which_name) const {
    this->ensure ();
    for (auto & [n, v] : *this) {
        std::wstring_view nn (this->string + n.offset, n.length);
        std::wstring_view vv (this->string + v.offset, v.length);

        for (auto & name : names) {
            if (nn == name) {
                if (value) {
                    *value = vv;
                }
                if (which_name) {
                    *which_name = name;
                }
                return true;
            }
        }
    }
    return false;
}

template <std::size_t N>
inline bool TRIMCORE::DescriptionFormatting::get (const std::wstring_view (&names) [N], std::wstring_view * value, std::size_t * which_index) const {
    this->ensure ();

    std::size_t index = 0;
    for (auto & [n, v] : *this) {
        std::wstring_view nn (this->string + n.offset, n.length);
        std::wstring_view vv (this->string + v.offset, v.length);

        for (auto & name : names) {
            if (nn == name) {
                if (value) {
                    *value = vv;
                }
                if (which_index) {
                    *which_index = index;
                }
                return true;
            }
            ++index;
        }
    }
    return false;
}

inline bool TRIMCORE::DescriptionFormatting::get (std::intptr_t index, std::wstring_view * name, std::wstring_view * value) const {
    this->ensure ();
    if (index < 0) {
        index = this->unpacked_size + index;
    }
    if ((std::size_t) index < this->unpacked_size) {
        if (name) {
            *name = std::wstring_view (this->string + this->unpacked_data [index].name.offset,
                                                      this->unpacked_data [index].name.length);
        }
        if (value) {
            *value = std::wstring_view (this->string + this->unpacked_data [index].value.offset,
                                                       this->unpacked_data [index].value.length);
        }
        return true;
    } else
        return false;
}

inline long long TRIMCORE::DescriptionFormatting::convert (std::wstring_view number, long long result) {
    return TRIMCORE::Implementation::FmtCvt (number.data (), number.size (), result);
}

inline long long TRIMCORE::DescriptionFormatting::number (int index, long long default_) const {
    std::wstring_view name;
    if (this->get (index, &name, nullptr)) {
        return this->convert (name, default_);
    } else
        return default_;
}

inline long long TRIMCORE::DescriptionFormatting::number (std::wstring_view name, long long default_) const {
    std::wstring_view value;
    if (this->get (name, &value)) {
        return this->convert (value, default_);
    } else
        return default_;
}

template <std::size_t N>
inline long long TRIMCORE::DescriptionFormatting::number (const std::wstring_view (&names) [N], long long default_, std::wstring_view * which_name) const {
    std::wstring_view value;
    if (this->get (names, &value, which_name)) {
        return this->convert (value, default_);
    } else
        return default_;
}

#endif
