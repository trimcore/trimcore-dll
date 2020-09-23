#ifndef TRIMCORE_DLL_ALLOC64KB_H
#define TRIMCORE_DLL_ALLOC64KB_H

#include <array>
#include <new>

namespace TRIMCORE {
    
    // Alloc64kB
    //  - retrieves 64kB temporary buffer from quick pre-allocated store
    //  - block is NOT explicitly zeroed
    //  - 32 slots for 32-bit code, 64 slots for 64-bit code available
    //  - when exhausted, calls HeapAlloc
    //
    TRIMCORE_DLL_IMPORT void * Alloc64kB ();

    // Free64kB
    //  - marks the slot in 64kB temporary buffer allocator for reuse
    //
    TRIMCORE_DLL_IMPORT bool Free64kB (void *);

    // Is64kB
    //  - determines if pointer points into quick allocation store
    //
    TRIMCORE_DLL_IMPORT bool Is64kB (void *);

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
    TRIMCORE_DLL_IMPORT std::size_t GetUsage64kB (std::size_t cb, Usage64kB * usage = nullptr);

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
            if (this->ptr) {
                Free64kB (this->ptr);
                this->ptr = nullptr;
            }
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
                    throw std::bad_alloc (); // TODO: better exception
            }
        }

        Temporary64kB (Temporary64kB &) = delete;
        Temporary64kB & operator = (Temporary64kB &) = delete;
    };
}

#endif
