#pragma once

#include <array>
template<class T, std::size_t N>
class static_circular_deque {
public:
    using container_type = std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, N>;

    using container_iterator = typename container_type::iterator;
    using container_const_iterator = typename container_type::const_iterator;

    using value_type = T;
    using size_type = typename container_type::size_type;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

protected:
    container_type c{};
    container_iterator first = c.end();
    container_iterator last = c.end();

public:

    ~static_circular_deque() { clear(); }

    reference front() { return *std::launder(reinterpret_cast<pointer>(first)); }
    const_reference front() const { *std::launder(reinterpret_cast<const_pointer>(first)); }

    reference back() { return *std::launder(reinterpret_cast<pointer>(last)); }
    const_reference back() const { return *std::launder(reinterpret_cast<const_pointer>(last)); }

    [[nodiscard]] bool empty() const noexcept { return size() == 0; }
    size_type size() const noexcept { return (first != c.end()) ? ((first <= last) ? (1 + last - first) : (max_size() - (first - last ) + 1)) : 0; }

    constexpr size_type max_size() const noexcept { return c.max_size(); }
    constexpr size_type capacity() const noexcept { return max_size(); }

    void push_back(value_type const& value) { push_back(); new(last) value_type(value); }
    void push_back(value_type&& value) { push_back(); new(last) value_type(std::move(value)); }
    template<class ...Args> reference emplace_back(Args&& ...args) { push_back(); return new(last) value_type(args...);}

    void push_front(value_type const& value) { push_front(); new(first) value_type(value); }
    void push_front(value_type&& value) { push_back(); new(first) value_type(std::move(value)); }
    template<class ...Args> reference emplace_front(Args&& ...args) { push_back(); return new(first) value_type(args...);}

    void clear() noexcept { for(auto i = size(); i--; ) pop_front();}



    void pop_front() {
        std::launder(reinterpret_cast<pointer>(first))->~value_type();
        if(first == last) first = last = c.end();
        else {
            ++first;
            if(c.end() == first) first = c.begin();
        }
    }

    void pop_back() {
        std::launder(reinterpret_cast<pointer>(last))->~value_type();
        if(last == first) last = first = c.end();
        else {
            if(c.begin() == last) last = c.end() - 1;
            else --last;
        }
    }



    void swap(static_circular_deque &other) {
        swap(c, other.c);
        swap(first, other.first);
        swap(last, other.last);
    }

private:
    void push_back() {
        if(c.end() == last) last = first = c.begin();
        else if(c.end() == ++last) last = c.begin();
    }

    void push_front() {
        if(c.end() == first) last = first = c.begin();
        else if(c.begin() == first) first = c.end() - 1;
        else --first;
    }
};
