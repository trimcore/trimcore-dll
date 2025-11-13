#ifndef TRIMCORE_DLL_POINTERINFO_H
#define TRIMCORE_DLL_POINTERINFO_H

namespace TRIMCORE {

    enum class PointerTarget : std::uint8_t {
        Unknown,
        Image, // section of EXE or DLL
        Stack,
        Heap,
    };

    // PointerInfo
    //  - 
    //  - initialize by memsetting to 0
    //
    struct PointerInfo {
        PointerTarget   type;
        std::ptrdiff_t  offset;
        wchar_t         filename [MAX_PATH];
        std::size_t     filename_length;

        union {
            struct Image {
                HMODULE                      module;
                const IMAGE_SECTION_HEADER * section;
                WORD                         section_number;
                const char *                 exportname;
            } image;

            struct Stack {
                DWORD        id; // thread ID
                const void * base;
                const void * limit;
                const void * procedure;
            } stack;
        };
    };

    // GetPointerInfo
    //  - 
    //
    bool GetPointerInfo (const void * ptr, PointerInfo * result);
}

#endif
