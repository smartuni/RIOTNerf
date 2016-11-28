#ifndef APP_BUFFER_HPP
#define APP_BUFFER_HPP
#include <cstddef>    // std::size_t
#include <iterator>   // std::reverse_iterator
#include <array>      // std::array
#include <cstddef>    // std::size_t, std::ptrdiff_t
#include "common.hpp" // app::byte, app::ssize_t

namespace app
{
    class buffer_base
    {
    public:
        using this_type = buffer_base;
        using size_type = ssize_t;
        using iterator = byte *;
        using const_iterator = byte const *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using value_type = byte;
        using pointer = value_type *;
        using const_pointer = value_type const *;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = value_type const &;

        virtual reference operator[](size_type idx) noexcept = 0;
        virtual const_reference operator[](size_type idx) const noexcept = 0;
        virtual reference at(size_type idx) = 0;
        virtual const_reference at(size_type idx) const = 0;

        virtual pointer data() noexcept = 0;
        virtual const_pointer data() const noexcept = 0;
        virtual const_pointer const_data() const noexcept = 0;
        virtual size_type size() const noexcept = 0;

        virtual iterator begin() noexcept = 0;
        virtual const_iterator begin() const noexcept = 0;
        virtual const_iterator cbegin() const noexcept = 0;
        virtual iterator end() noexcept = 0;
        virtual const_iterator end() const noexcept = 0;
        virtual const_iterator cend() const noexcept = 0;
        virtual reverse_iterator rbegin() noexcept = 0;
        virtual const_reverse_iterator rbegin() const noexcept = 0;
        virtual const_reverse_iterator crbegin() const noexcept = 0;
        virtual reverse_iterator rend() noexcept = 0;
        virtual const_reverse_iterator rend() const noexcept = 0;
        virtual const_reverse_iterator crend() const noexcept = 0;

        virtual ~buffer_base();
    }; // END of class buffer_base

    template <ssize_t buf_siz>
    class buffer final
        : public buffer_base 
    {
    public:
        using this_type = buffer;
        using base_type = buffer_base;
        using size_type = base_type::size_type;
        using iterator = base_type::iterator;
        using const_iterator = base_type::const_iterator;
        using reverse_iterator = base_type::reverse_iterator;
        using const_reverse_iterator = base_type::const_reverse_iterator;
        using value_type = base_type::value_type;
        using pointer = base_type::pointer;
        using const_pointer = base_type::const_pointer;
        using difference_type = base_type::difference_type;
        using reference = base_type::reference;
        using const_reference = base_type::const_reference;

        static_assert(buf_siz > 0, "buf_siz in app::buffer must be larger than 0");

        buffer()
            : m_ary{ '\0' } { }

        virtual reference operator[](size_type idx) noexcept override {
            return m_ary[static_cast<std::size_t>(idx)];
        }

        virtual const_reference operator[](size_type idx) const noexcept override {
            return const_cast<this_type *>(this)->operator[](idx);
        }

        virtual reference at(size_type idx) override {
            return m_ary.at(static_cast<std::size_t>(idx));
        }

        virtual const_reference at(size_type idx) const override {
            return const_cast<this_type *>(this)->at(idx);
        }

        virtual pointer data() noexcept override {
            return m_ary.data();
        }

        virtual const_pointer data() const noexcept override {
            return const_cast<this_type *>(this)->data();
        }

        virtual const_pointer const_data() const noexcept override {
            return data();
        }
 
        virtual size_type size() const noexcept override {
            return buf_siz;
        }

        virtual iterator begin() noexcept override {
            return m_ary.begin();
        }

        virtual const_iterator begin() const noexcept override {
            return m_ary.begin();
        }

        virtual const_iterator cbegin() const noexcept override {
            return begin();
        }

        virtual iterator end() noexcept override {
            return m_ary.end();
        }

        virtual const_iterator end() const noexcept override {
            return m_ary.end();
        }

        virtual const_iterator cend() const noexcept override {
            return end();
        }

        virtual reverse_iterator rbegin() noexcept override {
            return reverse_iterator{ end() };
        }

        virtual const_reverse_iterator rbegin() const noexcept override {
            return const_reverse_iterator{ end() };
        }

        virtual const_reverse_iterator crbegin() const noexcept override {
            return rbegin();
        }

        virtual reverse_iterator rend() noexcept override {
            return reverse_iterator{ begin() };
        }

        virtual const_reverse_iterator rend() const noexcept override {
            return const_reverse_iterator{ begin() };
        }

        virtual const_reverse_iterator crend() const noexcept override {
            return rend();
        }

    private:
        std::array<byte, static_cast<std::size_t>(buf_siz)> m_ary;
    }; // END of class buffer
} // END of namespace app

#endif /* APP_BUFFER_HPP */
