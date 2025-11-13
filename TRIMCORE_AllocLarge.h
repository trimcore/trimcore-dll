#ifndef TRIMCORE_DLL_ALLOCLARGE_H
#define TRIMCORE_DLL_ALLOCLARGE_H

#include <array>
#include <new>

namespace TRIMCORE {
    
    // AllocationParameters
    //  -
    //
    struct AllocationParameters {
        void *      preferred = nullptr;                    // address to place allocation at, if possible
        std::size_t reserve = 0;                            // total address space to reserve; ignored if less than 'size'
        USHORT      numa = (USHORT) NUMA_NO_PREFERRED_NODE; // preferred NUMA node
        bool        reserved = false;  
        bool        large = false;                          // use 2M pages or 1G pages, if possible
        DWORD       timeout = 0;                            // timeout (in ms) for Hyper-V Dynamic Memory -friendly allocation
    };

    // AllocLarge
    //  - 
    //
    TRIMCORE_DLL_IMPORT void * TRIMCORE_APIENTRY AllocLarge (std::size_t size, const AllocationParameters * = nullptr);

    // ResizeLarge
    //  - attempt to in-place reallocate the large buffer
    //  - max size (address space) must be reserved through AllocationParameters::reserve parameter
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY ResizeLarge (void *, std::size_t);

    // FreeLarge
    //  - 
    //
    TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY FreeLarge (void *);

    // LargeBuffer
    //  - automatically managed (destroyed) AllocLarge/FreeLarge buffer
    //  - moved from object is invalid
    //
    class LargeBuffer {
    protected:
        void *      data;
        std::size_t size;
    public:
        inline explicit LargeBuffer (std::size_t size, const AllocationParameters * parameters = nullptr)
            : data (AllocLarge (size, parameters))
            , size (data ? size : 0) {

            if (!this->data) {
                throw std::bad_alloc (); // TODO: trimcore log exception?
            }
        }
        inline LargeBuffer (LargeBuffer && other) noexcept
            : data (other.data)
            , size (other.size) {

            other.data = nullptr;
            other.size = 0;
        }
        inline LargeBuffer & operator = (LargeBuffer && other) noexcept {
            FreeLarge (this->data);
            this->data = other.data;
            this->size = other.size;
            other.data = nullptr;
            other.size = 0;
            return *this;
        }
        inline ~LargeBuffer () {
            if (this->data) {
                FreeLarge (this->data);
                this->data = nullptr;
                this->size = 0;
            }
        }
    public:
        bool resize (std::size_t size) {
            return ResizeLarge (this->data, size);
        }
    private:
        LargeBuffer (LargeBuffer &) = delete;
        LargeBuffer & operator = (LargeBuffer &) = delete;
    };
}

#endif
