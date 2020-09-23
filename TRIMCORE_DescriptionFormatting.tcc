#ifndef TRIMCORE_DLL_DESCRIPTIONFORMATTING_TCC
#define TRIMCORE_DLL_DESCRIPTIONFORMATTING_TCC

#include <charconv>

inline TRIMCORE::DescriptionFormatting::Unpacked::Unpacked (const wchar_t * string, std::size_t length) {
    if (string && length) {
        std::wstring_view s (string, length);
        std::wstring_view::size_type o = 0;
        std::wstring_view::size_type e = 0;

        this->data.reserve (std::count (s.begin (), s.end (), L',') + 1);

        while (auto v = s.find_first_of (L":<,", o) + 1) {
            auto c = s [v - 1];
            e = v;

            switch (c) {
                case L'<': e = s.find (L'>', v); break;
                case L':': e = s.find (L',', v); break;
            }

            this->data.push_back ({ s.substr (o, v - 1 - o), s.substr (v, e - v) });

            if (e == std::wstring_view::npos)
                break;

            if (c == L'<') {
                ++e; // skip '>'
            }
            if (c != L',') {
                ++e; // skip ','
            }
            o = e;
        }
        if ((e != std::wstring_view::npos) && (o < s.size ())) {
            this->data.push_back ({ s.substr (o, s.size () - o), std::wstring_view () });
        }
    }
}

inline bool TRIMCORE::DescriptionFormatting::Unpacked::get (std::wstring_view name, std::wstring_view * value) const {
    for (auto & [n, v] : this->data) {
        if (n == name) {
            if (value) {
                *value = v;
            }
            return true;
        }
    }
    return false;
}

inline bool TRIMCORE::DescriptionFormatting::Unpacked::get (std::intptr_t index, std::wstring_view * name, std::wstring_view * value) const {
    if (index < 0) {
        index = this->data.size () + index;
    }
    if ((std::size_t) index < this->data.size ()) {
        if (name) {
            *name = this->data [index].name;
        }
        if (value) {
            *value = this->data [index].value;
        }
        return true;
    } else
        return false;
}

inline long long TRIMCORE::DescriptionFormatting::Unpacked::convert (std::wstring_view number, long long result) {
    char a [68];
    auto o = 0u;
    auto base = 10;

    if (number.starts_with (L"b") || number.starts_with (L"B")) { base = 2; o = 1; }
    if (number.starts_with (L"0b") || number.starts_with (L"0B")) { base = 2; o = 2; }
    if (number.starts_with (L"x") || number.starts_with (L"X")) { base = 16; o = 1; }
    if (number.starts_with (L"0x") || number.starts_with (L"0X")) { base = 16; o = 2; }

    auto i = 0u;
    for (; i != std::min (sizeof a, number.size () - o); ++i) {
        if (number [i + o] < 128) {
            a [i] = (char) number [i + o];
        } else
            break;
    }

    std::from_chars (a, a + i, result, base);
    return result;
}

inline long long TRIMCORE::DescriptionFormatting::Unpacked::number (int index, long long default_) const {
    std::wstring_view name;
    if (this->get (index, &name, nullptr)) {
        return this->convert (name, default_);
    } else
        return default_;
}

inline long long TRIMCORE::DescriptionFormatting::Unpacked::number (std::wstring_view name, long long default_) const {
    std::wstring_view value;
    if (this->get (name, &value)) {
        return this->convert (value, default_);
    } else
        return default_;
}

#endif
