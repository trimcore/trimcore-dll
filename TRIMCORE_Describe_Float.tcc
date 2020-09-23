#ifndef TRIMCORE_DLL_DESCRIBE_FLOAT_TCC
#define TRIMCORE_DLL_DESCRIBE_FLOAT_TCC

template <typename T>
inline std::wstring TRIMCORE::DescribeFundamentalFloat (T number, DescriptionFormatting * parameters) {
    char tmp [64];
    if (parameters) {
        // TODO: parse parameters, if L or $ call API

        std::chars_format fmt {};
        int precission = 0;
        if (false) { // if parsed format?

            std::to_chars (tmp, tmp + sizeof tmp, number, fmt, precission);
            return a2w (tmp);
        }
    }
    auto r = std::to_chars (tmp, tmp + sizeof tmp, number);
    return Describe (std::string_view (tmp, r.ptr - tmp), parameters);
}

#endif
