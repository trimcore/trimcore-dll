#ifndef TRIMCORE_DLL_DESCRIPTIONFORMATTING_H
#define TRIMCORE_DLL_DESCRIPTIONFORMATTING_H

#include <string_view>
#include <vector>

namespace TRIMCORE {

    // DescriptionFormatting
    //  - provides view of format string to parse
    //  - allows algorithm to adjust presentation
    //  - format string format example: "title<value>,other:value,ABC<DEF>,GHI,J,K,L"
    //
    struct DescriptionFormatting {
    // in:
        const wchar_t * string = nullptr; // NOT NUL-terminated, use 'length'
        std::size_t     length = 0;
        LCID            locale = 0; // 0 usually means exactly (or technical variation of) LOCALE_INVARIANT
        DWORD           flags = 0; // value-specific flags
        HMODULE         module = NULL; // calling module, source of resources, if any

    // out:
        std::uint8_t    quoted = 1;
        bool            invalid = false; // reports invalid input

    public:

        // Unpacked
        //  - format string converted to pairs of name/value
        //
        struct Unpacked {
            struct Parameter {
                std::wstring_view name;
                std::wstring_view value;
            };
        public:
            Unpacked () = default;
            Unpacked (Unpacked &&) = default;
            Unpacked (const Unpacked &) = default;
            Unpacked & operator = (Unpacked &&) = default;
            Unpacked & operator = (const Unpacked &) = default;

            explicit Unpacked (const wchar_t *, std::size_t);

            // get
            //  - determines if a parameter in format string exists (returns true/false)
            //  - value, if not nullptr, is set to substring enclosed in < > (if any)
            //
            inline bool get (std::wstring_view name, std::wstring_view * value) const;
            
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

            // number
            //  - attempts to convert 'title' of 'index'-th parameter to number
            //  - decimal, unless one of following prefixes:
            //     - x/X/0x/0X - hexadecimal (negative syntax is 0x-1A for now)
            //     - b/B/0b/0B - binary
            //
            inline long long number (int index, long long default_ = 0) const;

            // number
            //  - returns numeric value of parameter 'p' or default (0)
            //  - example syntax: "NAME<0x123>" or "NAME:123"
            //
            inline long long number (std::wstring_view name, long long default_ = 0) const;

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

            [[nodiscard]] std::size_t size () const noexcept { return this->data.size (); }

            [[nodiscard]] const Parameter *  begin () const noexcept { return this->data.data (); }
            [[nodiscard]] const Parameter * cbegin () const noexcept { return this->data.data (); }
            [[nodiscard]] const Parameter *    end () const noexcept { return this->data.data () + this->data.size (); }
            [[nodiscard]] const Parameter *   cend () const noexcept { return this->data.data () + this->data.size (); }

        private:
            std::vector <Parameter> data;

            static inline long long convert (std::wstring_view number, long long default_);
        };

        // unpack
        //  - breaks comma-separated parameter string "title<value>,title<value>,title<value>"
        //    into pairs of of title/value
        //
        Unpacked unpack () const { return Unpacked (this->string, this->length); }

        // accessing functions
        //  - same as in Unpacked above

        inline bool parameter (std::wstring_view name, std::wstring_view * value) const {
            return this->unpack ().get (name, value);
        }
        inline bool parameter (std::intptr_t index, std::wstring_view * name, std::wstring_view * value) const {
            return this->unpack ().get (index, name, value);
        }
        inline bool contains (std::wstring_view p) const {
            return this->unpack ().contains (p);
        }
    };
}

#include "TRIMCORE_DescriptionFormatting.tcc"
#endif
