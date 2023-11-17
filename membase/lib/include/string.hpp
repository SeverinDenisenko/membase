#pragma once

#include <algorithm>
#include <memory>
#include <string>

#include <boost/container_hash/hash.hpp>
#include <leveldb/slice.h>

#include "defines.hpp"

namespace mb {

template <typename Allocator>
class BasicStringView;

template <typename Allocator>
class BasicString {
public:
    class Iterator {
    public:
        explicit Iterator(std::byte* ptr) noexcept
            : ptr_(ptr)
        {
        }

        Iterator operator++() noexcept
        {
            ++ptr_;
            return *this;
        }

        Iterator operator--() noexcept
        {
            --ptr_;
            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator tmp(ptr_);
            ++ptr_;
            return tmp;
        }

        Iterator operator--(int) noexcept
        {
            Iterator tmp(ptr_);
            --ptr_;
            return tmp;
        }

        Iterator operator+(size_t index) const noexcept
        {
            return Iterator(ptr_ + index);
        }

        Iterator operator-(size_t index) const noexcept
        {
            return Iterator(ptr_ - index);
        }

        friend size_t operator-(const Iterator& lhs, const Iterator& rhs) noexcept
        {
            return lhs.ptr_ - rhs.ptr_;
        }

        std::byte& operator*() noexcept
        {
            return *ptr_;
        }

        const std::byte& operator*() const noexcept
        {
            return *ptr_;
        }

        friend bool operator==(Iterator lhs, Iterator rhs)
        {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator<(Iterator lhs, Iterator rhs)
        {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator<=(Iterator lhs, Iterator rhs)
        {
            return lhs.ptr_ <= rhs.ptr_;
        }

    private:
        std::byte* ptr_ { nullptr };
    };

    explicit BasicString(size_t len) noexcept
        : len_(len)
        , data_(Allocator().allocate(len))
    {
        Reset();
    }

    BasicString(const char* str) noexcept
        : len_(std::strlen(str))
        , data_(Allocator().allocate(len_))
    {
        std::memmove(data_, str, len_);
    }

    BasicString(const std::string& str) noexcept
        : len_(str.size())
        , data_(Allocator().allocate(len_))
    {
        std::memmove(data_, str.data(), len_);
    }

    template <typename OtherAllocator>
    BasicString(typename BasicStringView<OtherAllocator>::Iterator begin, typename BasicStringView<OtherAllocator>::Iterator end) noexcept
        : len_(end - begin)
        , data_(Allocator().allocate(len_))
    {
        std::memmove(data_, &(*begin), len_);
    }

    template <typename OtherAllocator>
    explicit BasicString(BasicStringView<OtherAllocator> view) noexcept
        : len_(view.End() - view.Begin())
        , data_(Allocator().allocate(len_))
    {
        std::memmove(data_, &(*view.Begin()), len_);
    }

    BasicString() noexcept
        : len_(0)
        , data_(nullptr)
    {
    }

    BasicString(BasicString& other) = delete;
    BasicString& operator=(BasicString& other) = delete;

    template <typename OtherAllocator>
    BasicString<OtherAllocator> Copy() const noexcept
    {
        BasicString<OtherAllocator> other { len_ };

        std::memmove(other.Data(), data_, len_);

        return other;
    }

    BasicString(BasicString&& other) noexcept
    {
        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;
    }

    BasicString& operator=(BasicString&& other) noexcept
    {
        if (this == std::addressof(other)) {
            return *this;
        }

        data_ = other.data_;
        len_ = other.len_;
        other.data_ = nullptr;
        other.len_ = 0;

        return *this;
    }

    ~BasicString() noexcept
    {
        Allocator().deallocate(data_, len_);
        len_ = 0;
    }

    const std::byte& operator[](size_t index) const noexcept
    {
        return data_[index];
    }

    std::byte& operator[](size_t index) noexcept
    {
        return data_[index];
    }

    void Reset() noexcept
    {
        std::memset(data_, 0, len_);
    }

    std::byte* Data() noexcept
    {
        return data_;
    }

    size_t Length() const noexcept
    {
        return len_;
    }

    Iterator Begin() const noexcept
    {
        return Iterator(data_);
    }

    Iterator End() const noexcept
    {
        return Iterator(data_ + len_);
    }

    const std::string ToStdString() const
    {
        return { (char*)data_, len_ };
    }

    leveldb::Slice ToSlice() const
    {
        return { (char*)data_, len_ };
    }

    friend bool operator==(const BasicString& lhs, const BasicString& rhs) noexcept
    {
        if (lhs.Length() != rhs.Length()) {
            return false;
        }

        for (size_t i = 0; i < rhs.Length(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }

        return true;
    }

    friend bool operator<(const BasicString& lhs, const BasicString& rhs) noexcept
    {
        return std::lexicographical_compare(lhs.data_, lhs.data_ + lhs.len_,
            rhs.data_, rhs.data_ + rhs.len_);
    }

    std::size_t Hash() const
    {
        std::size_t res = 0;

        for (size_t i = 0; i < len_; ++i) {
            res += (std::size_t)data_[i];
        }

        return std::hash<std::size_t>()(res);
    }

private:
    size_t len_ { 0 };
    std::byte* data_ { nullptr };
};

template <typename Allocator>
class BasicStringView {
public:
    explicit BasicStringView(typename BasicString<Allocator>::Iterator begin, typename BasicString<Allocator>::Iterator end) noexcept
        : begin_(begin)
        , end_(end)
    {
    }

    BasicStringView() noexcept
        : begin_(nullptr)
        , end_(nullptr)
    {
    }

    ~BasicStringView() noexcept
    {
    }

    const std::byte& operator[](size_t index) const noexcept
    {
        return *(begin_ + index);
    }

    std::byte& operator[](size_t index) noexcept
    {
        return *(begin_ + index);
    }

    size_t Length() const noexcept
    {
        return end_ - begin_;
    }

    typename BasicString<Allocator>::Iterator Begin() noexcept
    {
        return begin_;
    }

    typename BasicString<Allocator>::Iterator End() noexcept
    {
        return end_;
    }

    std::byte* Data() noexcept
    {
        return &(*begin_);
    }

    std::string ToStdString() const noexcept
    {
        return std::string((char*)&(*begin_), end_ - begin_);
    }

    const leveldb::Slice ToSlice() const noexcept
    {
        return leveldb::Slice((char*)&(*begin_), end_ - begin_);
    }

private:
    typename BasicString<Allocator>::Iterator begin_ { nullptr };
    typename BasicString<Allocator>::Iterator end_ { nullptr };
};

using String = BasicString<std::allocator<std::byte>>;
using StringView = BasicStringView<std::allocator<std::byte>>;

} // namespace mb

template <>
struct std::hash<mb::String> {
    std::size_t operator()(const mb::String& str) const
    {
        return str.Hash();
    }
};
