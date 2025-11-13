#ifndef TRIMCORE_DLL_DESCRIPTIONFORMATTING_H
#define TRIMCORE_DLL_DESCRIPTIONFORMATTING_H

#include <string_view>
#include <vector>

namespace TRIMCORE {

    // DescriptionFormatting
    //  - provides view of format string to parse, and reports back conversion status (is NOT const)
    //  - allows algorithm to adjust presentation
    //  - format string format example: "title<value>,other:value,ABC<DEF>,GHI,J,K,L"
    //  - layout is ABI!
    //
    struct DescriptionFormatting {
    // in:
        HMODULE         module = NULL; // calling module, source of resources, optional
        const wchar_t * string = nullptr; // NOT NUL-terminated, use 'length'
        std::uint16_t   length = 0;
        std::uint8_t    skip_auto_string_formatting = 0; // skips internally subsequently passing to Descrie(string)
        std::uint8_t    reserved = 0;

    // out:
        std::uint8_t    quoted = 1;
        bool            invalid = false; // reports invalid input

    // temporary:
    public:
        struct UnpackedSubstring {
            std::uint16_t offset;
            std::uint16_t length;
        };
        struct UnpackedParameter {
            UnpackedSubstring name;
            UnpackedSubstring value;
        };
    private:
        mutable std::uint16_t                     unpacked_size = 0;
        mutable Temporary64kB <UnpackedParameter> unpacked_data;

    public:
        DescriptionFormatting () = default;
        inline DescriptionFormatting (const wchar_t * string, std::size_t length) noexcept
            : string (length ? string : nullptr)
            , length ((std::uint16_t ) ((length < 0x1'0000) ? length : 0xFFFF)) {}

        inline DescriptionFormatting (const DescriptionFormatting & other) noexcept
            : module (other.module)
            , string (other.string)
            , length (other.length)
            , quoted (other.quoted)
            , invalid (other.invalid)
            , reserved (other.reserved)
            , unpacked_size (0) {}

        inline DescriptionFormatting & operator = (const DescriptionFormatting & other) noexcept {
            this->module = other.module;
            this->string = other.string;
            this->length = other.length;
            this->quoted = other.quoted;
            this->invalid = other.invalid;
            this->reserved = other.reserved;
            this->unpacked_size = 0;
            this->unpacked_data.release ();
        }

        inline explicit DescriptionFormatting (std::wstring_view view)
            : DescriptionFormatting (view.size () ? view.data () : nullptr,
                                     view.size ()) {}

        DescriptionFormatting (DescriptionFormatting &&) noexcept = default;
        DescriptionFormatting & operator = (DescriptionFormatting &&) noexcept = default;

        // blank
        //  - true if unset and free to ignore
        //
        inline bool blank () const noexcept { return this->string == nullptr || this->length == 0; }

        // get
        //  - determines if a parameter in format string exists (returns true/false)
        //  - value, if not nullptr, is set to substring enclosed in < > (if any)
        //
        inline bool get (std::wstring_view name, std::wstring_view * value) const;

        // get
        //  - determines if any of the 'names' in format string exist (returns true/false)
        //  - value, if not nullptr, is set to substring enclosed in < > (if any) of the first found
        //  - which_name, if not nullptr, is set to 'name' that was found
        //
        template <std::size_t N>
        inline bool get (const std::wstring_view (&names) [N], std::wstring_view * value, std::wstring_view * which_name = nullptr) const;

        // get
        //  - determines if any of the 'names' in format string exist (returns true/false)
        //  - value, if not nullptr, is set to substring enclosed in < > (if any) of the first found
        //  - which_index, if not nullptr, is set to index (0 ... N-1) into 'names' that was found
        //
        template <std::size_t N>
        inline bool get (const std::wstring_view (&names) [N], std::wstring_view * value, std::size_t * which_index) const;

        // get
        //  - retrieves index-th parameter from the string, and writes it into 'name' and/or 'value'
        //  - if index < 0 then index counts from the last parameter
        //
        inline bool get (std::intptr_t index, std::wstring_view * name, std::wstring_view * value) const;

        // contains
        //  - returns true if parameter 'p' exists in the parameter string
        //
        inline bool contains (std::wstring_view p) const {
            return this->get (p, nullptr);
        }

        // contains
        //  - returns true if any of the 'names' exist in the parameter string
        //  - which_name, if not nullptr, is set to 'name' that was found
        //
        template <std::size_t N>
        inline bool contains (const std::wstring_view (&names) [N], std::wstring_view * which_name = nullptr) const {
            return this->get (names, nullptr, which_name);
        }

        // contains
        //  - returns true if any of the 'names' exist in the parameter string
        //  - which_index, if not nullptr, is set to index (0 ... N-1) into 'names' that was found
        //
        template <std::size_t N>
        inline bool contains (const std::wstring_view (&names) [N], std::size_t * which_index) const {
            return this->get (names, nullptr, which_index);
        }

        // number
        //  - attempts to convert 'title' of 'index'-th parameter to number
        //  - decimal, unless one of following prefixes:
        //     - x/X/0x/0X - hexadecimal (negative syntax is 0x-1A for now)
        //     - b/B/0b/0B - binary
        //
        inline long long number (int index, long long default_ = 0) const;

        // number
        //  - returns numeric value of parameter 'name' or default (0)
        //  - example syntax: "NAME<0x123>" or "NAME:123"
        //
        inline long long number (std::wstring_view name, long long default_ = 0) const;

        // number
        //  - returns numeric value of first valid parameter in 'inames' or default (0)
        //  - example syntax: "NAME<0x123>" or "NAME:123"
        //  - which_name, if not nullptr, is set to 'name' that was found
        //
        template <std::size_t N>
        inline long long number (const std::wstring_view (&names) [N], long long default_ = 0, std::wstring_view * which_name = nullptr) const;

        // operator [L"name"]
        //  - returns value of a parameter 'p' or empty view
        //
        inline std::wstring_view operator [] (std::wstring_view name) const {
            std::wstring_view value;
            this->get (name, &value);
            return value;
        }

        // operator [int]
        //  - returns value of a parameter by 'index' or empty view
        //
        inline std::wstring_view operator [] (std::intptr_t index) const {
            std::wstring_view value;
            this->get (index, nullptr, &value);
            return value;
        }
            
        // most common container operations

        [[nodiscard]] std::size_t size () const noexcept { this->ensure (); return this->unpacked_size; }
        [[nodiscard]] const UnpackedParameter *  begin () const noexcept { this->ensure (); return this->unpacked_data.data (); }
        [[nodiscard]] const UnpackedParameter * cbegin () const noexcept { this->ensure (); return this->unpacked_data.data (); }
        [[nodiscard]] const UnpackedParameter *    end () const noexcept { this->ensure (); return this->unpacked_data.data () + this->unpacked_size; }
        [[nodiscard]] const UnpackedParameter *   cend () const noexcept { this->ensure (); return this->unpacked_data.data () + this->unpacked_size; }
        
    public:
        void release () const {
            this->unpacked_data.release ();
            this->unpacked_size = 0;
        }

    private:
        void ensure () const;
        void unpack () const;
        static inline long long convert (std::wstring_view number, long long default_);
    };
}

#include "TRIMCORE_DescriptionFormatting.tcc"
#endif
