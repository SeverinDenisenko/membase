#pragma once

#include "defines.hpp"
#include <cstddef>

namespace mb {
template <typename Object, typename Allocator>
class BasicVector {
public:
    explicit BasicVector(size_t size, size_t capacity) noexcept
        : size_(size)
        , capacity_(capacity)
        , data_(reinterpret_cast<Object*>(Allocator().allocate(capacity_ * sizeof(Object))))
    {
        Init(data_, data_ + size_);
    }

    BasicVector(BasicVector& other) = delete;
    BasicVector& operator=(BasicVector& other) = delete;

    BasicVector(BasicVector&& other)
    {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    BasicVector& operator=(BasicVector&& other)
    {
        if (this == std::addressof(other)) {
            return *this;
        }

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;

        return this;
    }

    ~BasicVector()
    {
        Destruct(data_, data_ + size_);
        Allocator().deallocate(reinterpret_cast<std::byte*>(data_), capacity_ * sizeof(Object));
    }

    Object& operator[](size_t index) noexcept
    {
        return data_[index];
    }

    const BasicVector& operator[](size_t index) const noexcept
    {
        return data_[index];
    }

    size_t Length() const noexcept
    {
        return size_;
    }

    void Push(Object&& object) noexcept
    {
        Emplace(object);
    }

    template <class... Args>
    void Emplace(Args&&... args)
    {
        if (size_ >= capacity_) {
            Grow();
        }

        new (data_ + size_++) Object(std::forward<Args>(args)...);
    }

    void Pop() noexcept
    {
        (data_ + --size_)->~Object();
    }

private:
    void
    Init(Object* begin, Object* end) noexcept
    {
        for (Object* current = begin; current < end; ++current) {
            new (current) Object();
        }
    }

    void Destruct(Object* begin, Object* end) noexcept
    {
        for (Object* current = begin; current < end; ++current) {
            current->~Object();
        }
    }

    void Move(Object* src, Object* dist, size_t size) noexcept
    {
        std::memmove(dist, src, size * sizeof(Object));
    }

    void Grow() noexcept
    {
        size_t capacity = capacity_ * 2;
        Object* data = reinterpret_cast<Object*>(Allocator().allocate(capacity * sizeof(Object)));
        Move(data_, data, size_);
        Allocator().deallocate(reinterpret_cast<std::byte*>(data_), capacity_ * sizeof(Object));
        data_ = data;
        capacity_ = capacity;
    }

    size_t size_ { 0 };
    size_t capacity_ { 0 };
    Object* data_ { nullptr };
};

template <typename Object>
using Vector = BasicVector<Object, std::allocator<std::byte>>;
}
