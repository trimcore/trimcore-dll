#ifndef TRIMCORE_DLL_ALLOC64KB_H
#define TRIMCORE_DLL_ALLOC64KB_H

#include <new>
#include <array>
#include <memory>

namespace TRIMCORE {
    
    // Alloc64kB
    //  - retrieves 64kB temporary buffer from quick pre-allocated store
    //  - block is NOT explicitly zeroed
    //  - 32 slots for 32-bit code, 64 slots for 64-bit code available
    //  - when exhausted, calls HeapAlloc
    //
    _Ret_maybenull_
    _Post_writable_byte_size_ (65536)
    TRIMCORE_DLL_IMPORT void * TRIMCORE_APIENTRY Alloc64kB ();

    // Free64kB
    //  - marks the slot in 64kB temporary buffer allocator for reuse
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY Free64kB (void *);

    // Is64kB
    //  - determines if pointer points into quick allocation store
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY Is64kB (void *);

    // Decommit64kB
    //  - attempts to release all currently unused committed pages
    //  - returns number of freed pages
    //
    TRIMCORE_DLL_IMPORT std::uint32_t TRIMCORE_APIENTRY Decommit64kB ();

    // Usage64kB
    //  - 
    //
    struct Usage64kB {
        std::uint32_t usage;      // number of 64kB slots currently in use
        std::uint32_t capacity;   // pool capacity, total number of 64kB slots available
        std::uint32_t highwater;  // highest number of 64kB slots used at any point
        std::uint32_t reserved;
        
        std::uint64_t count;      // total number of successful pool allocations performed by Alloc64kB
        std::uint64_t fallbacks;  // number of times the pool was full and HeapAlloc was called instead
    };

    // GetUsage64kB
    //  - 
    //  - only 'cb' number of bytes is copied to 'usage' for backward compatibility if new members are added
    //     - call example: GetUsage64kB (sizeof (Usage64kB), &usage);
    //  - returns actual size of Usage64kB structure
    //     - 'usage' may be NULL to determine structure/trimcore.dll version
    //
    TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY GetUsage64kB (std::size_t cb, Usage64kB * usage = nullptr);

    // BadAlloc64kB
    //  - thrown by 'Temporary64kB' on failure to allocate
    //
    class BadAlloc64kB
        : public std::bad_alloc {

    public:
        BadAlloc64kB () = default;

        // copy

        inline BadAlloc64kB (const BadAlloc64kB &) = default;

        // movable

        inline BadAlloc64kB (BadAlloc64kB && from) noexcept = default;
        inline BadAlloc64kB & operator = (BadAlloc64kB && from) noexcept = default;
    };
    
    // Temporary64kB
    //  - block of 64kB of memory, movable
    //  - memory buffer is acquired on first use and released on destruction
    //
    template <typename T>
    class Temporary64kB {
        mutable T * ptr = nullptr;

    public:
        Temporary64kB () = default;
        Temporary64kB (Temporary64kB && other) noexcept
            : ptr (other.ptr) {

            other.ptr = nullptr;
        }
        Temporary64kB & operator = (Temporary64kB && other) noexcept {
            this->release ();
            this->ptr = other.ptr;
            other.ptr = nullptr;
            return *this;
        }
        ~Temporary64kB () noexcept {
            this->release ();
        }

        // allocated
        //  - determines whether the object has allocated buffer
        //
        inline bool allocated () const noexcept {
            return this->ptr != nullptr;
        }

        // release
        //  - manually release the buffer before the lifetime of the object expires
        //
        inline void release () noexcept {
            Free64kB (this->ptr);
            this->ptr = nullptr;
        }

        // most common container operations

        [[nodiscard]] constexpr static std::size_t size () noexcept { return 65536 / sizeof (T); }

        [[nodiscard]] constexpr       T * operator & ()       { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr const T * operator & () const { this->ensure (); return this->ptr; }

        [[nodiscard]] constexpr       T * data ()         { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr const T * data ()   const { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr       T *  begin ()       { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr const T *  begin () const { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr const T * cbegin () const { this->ensure (); return this->ptr; }
        [[nodiscard]] constexpr       T *  end ()         { this->ensure (); return this->ptr + this->size (); }
        [[nodiscard]] constexpr const T *  end ()   const { this->ensure (); return this->ptr + this->size (); }
        [[nodiscard]] constexpr const T * cend ()   const { this->ensure (); return this->ptr + this->size (); }
        [[nodiscard]] constexpr       T & operator [] (std::size_t i)       { this->ensure (); return this->ptr [i]; }
        [[nodiscard]] constexpr const T & operator [] (std::size_t i) const { this->ensure (); return this->ptr [i]; }

    private:
        inline void ensure () const  {
            if (this->ptr == nullptr) {
                this->ptr = static_cast <T *> (Alloc64kB ());
                if (this->ptr == nullptr)
                    throw BadAlloc64kB ();
            }
        }

        Temporary64kB (Temporary64kB &) = delete;
        Temporary64kB & operator = (Temporary64kB &) = delete;
    };

    // SPrintf64kB
    //  - formats parameteters per 'format' printf template string
    //  - returns the final string as Temporary64kB buffer of wchar_t
    //
    TRIMCORE_DLL_IMPORT Temporary64kB <wchar_t> TRIMCORE_APIENTRY SPrintf64kB (const wchar_t * format, ...);

    // LoadRsrc64kB
    //  - loads string 'id' from 'module' into 'buffer'
    //  - returns number of characters written to the buffer (not including NUL character), 0 on failure
    //
    TRIMCORE_DLL_IMPORT std::uint16_t TRIMCORE_APIENTRY LoadRsrc64kB (Temporary64kB <wchar_t> & buffer, HMODULE module, unsigned int id, LANGID language = 0);

    // Allocator64kB
    //  - 
    //
    template <typename T>
    struct Allocator64kB {
        typedef T value_type;
        typedef std::true_type is_always_equal;

        Allocator64kB () = default;

        template <class U>
        constexpr Allocator64kB (const Allocator64kB <U> &) noexcept {}

        constexpr std::size_t max_size () const noexcept {
            return 65536 / sizeof (T);
        }

        [[nodiscard]] T * allocate (std::size_t n) const {
            if (n <= 65536 / sizeof (T)) {
                if (auto p = Alloc64kB ())
                    return (T *) p;
            }
            throw BadAlloc64kB ();
        }

        [[nodiscard]] std::allocation_result<T *> allocate_at_least (std::size_t n) const {
            if (n <= 65536 / sizeof (T)) {
                if (auto p = Alloc64kB ())
                    return { (T *) p, 65536 / sizeof (T) };
            }
            throw BadAlloc64kB ();
        }
        
        void deallocate (T * p, std::size_t) const noexcept {
            Free64kB (p);
        }
    };
}

#endif
