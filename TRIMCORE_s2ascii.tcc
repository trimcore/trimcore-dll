#ifndef TRIMCORE_DLL_S2ASCII_TCC
#define TRIMCORE_DLL_S2ASCII_TCC

#include "TRIMCORE_s2ascii.h"

template <typename C>
std::size_t TRIMCORE::s2ascii (std::basic_string_view <C> input, char * buffer, std::size_t length, int replacement) {
    bool good = true;
    std::size_t n = 0;

    for (auto c : input) {
        if (c < 0 || c > 127) {
            switch (replacement) {
                case -1:
                    return n;
                case -2:
                    return 0;

                default:
                    if (good) {
                        good = false;
                        if (buffer) {
                            *buffer++ = replacement;
                        }
                        ++n;
                    }
            }
        } else {
            if (buffer) {
                *buffer++ = (char) c;
            }
            good = true;
            ++n;
        }
        if (buffer) {
            // TODO: fail on insufficient buffer
            if (n == length)
                break;
        }
    }
    return n;
}

template <typename C>
std::string TRIMCORE::s2ascii (std::basic_string_view <C> input, int replacement) {
    std::string result;
    if (auto length = s2ascii (input, nullptr, 0, replacement)) {
        result.resize (length);
        s2ascii (input, result.data (), length, replacement);
    }
    return result;
}

template <typename C>
std::string TRIMCORE::s2ascii (const std::basic_string <C> & input, int replacement) {
    return s2ascii (std::basic_string_view <C> (input), replacement);
}

#endif
