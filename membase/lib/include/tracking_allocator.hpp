#pragma once

#include <atomic>
#include <memory>

#include <fmt/format.h>
#include <glog/logging.h>

namespace mb {
template <class T>
class TrackingAllocator;

class AllocatorsMemory {
public:
    AllocatorsMemory(size_t memory) noexcept
        : total_memory(memory)
    {
    }

    bool IsEnoughFor(size_t size) const noexcept
    {
        return used_memory + size <= total_memory;
    }

    void Allocate(size_t size) noexcept
    {
        used_memory += size;
    }

    void Deallocate(size_t size) noexcept
    {
        used_memory -= size;
    }

    size_t GetTotal() const noexcept
    {
        return total_memory;
    }

    size_t GetUsed() const noexcept
    {
        return used_memory;
    }

private:
    size_t total_memory { 0 };
    size_t used_memory { 0 };
};

inline AllocatorsMemory allocatorsMemory(0);

template <class T>
class TrackingAllocator {
public:
    typedef T value_type;

    TrackingAllocator() = default;

    template <class U>
    constexpr TrackingAllocator(const TrackingAllocator<U>&) noexcept { }

    template <class U>
    constexpr TrackingAllocator(const TrackingAllocator<U>&&) noexcept { }

    [[nodiscard]] T* allocate(std::size_t n) noexcept
    {
        if (!allocatorsMemory.IsEnoughFor(n * sizeof(T))) {
            LOG(FATAL) << fmt::format("Reached limits for allocations! Trying to allocate {} bytes while used: {}, total: {}",
                n * sizeof(T), allocatorsMemory.GetUsed(), allocatorsMemory.GetTotal());
        }

        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            LOG(FATAL) << fmt::format("Allocation is out of numerical limit! trying to allocate {} objects of size {}",
                n, sizeof(T));
        }

        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
            allocatorsMemory.Allocate(sizeof(T) * n);
            return p;
        } else {
            LOG(FATAL) << fmt::format("Failed to allocate {} bytes. Is your system out of memory?",
                n * sizeof(T));
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        allocatorsMemory.Deallocate(sizeof(T) * n);
        std::free(p);
    }
};

template <class T, class U>
bool operator==(const TrackingAllocator<T>&, const TrackingAllocator<U>&) { return true; }

template <class T, class U>
bool operator!=(const TrackingAllocator<T>&, const TrackingAllocator<U>&) { return false; }

} // namespace mb
