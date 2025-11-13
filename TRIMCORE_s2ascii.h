#ifndef TRIMCORE_DLL_S2ASCII_H
#define TRIMCORE_DLL_S2ASCII_H

#include <string_view>
#include <string>

namespace TRIMCORE {

    // s2ascii
    //  - converts string to plain ASCII by eliminating code points over 127
    //  - 'invalid' is replacement character that can't be represented in ASCII (default is BEL)
    //     - if -1 then string is terminated at that point
    //     - if -2 then conversion fails completely
    //
    template <typename C>
    std::size_t s2ascii (std::basic_string_view <C> input, char * buffer, std::size_t length, int replacement = 0x07);

    template <typename C>
    std::string s2ascii (std::basic_string_view <C> input, int replacement = 0x07);

    template <typename C>
    std::string s2ascii (const std::basic_string <C> & input, int replacement = 0x07);
}

#include "TRIMCORE_s2ascii.tcc"
#endif
