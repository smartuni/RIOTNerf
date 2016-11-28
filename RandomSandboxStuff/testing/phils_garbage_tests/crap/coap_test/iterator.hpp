#ifndef APP_ITERATOR_HPP
#define APP_ITERATOR_HPP
#include <cstddef> // std::size_t
#include <iterator> // std::reverse_iterator

namespace app
{
    template <typename Cont>
    inline typename Cont::iterator begin(Cont &cont) {
        return cont.begin();
    }
    
    template <typename Cont>
    inline typename Cont::const_iterator begin(Cont const &cont) {
        return cont.begin();
    }
    
    template <typename Cont>
    inline typename Cont::const_iterator cbegin(Cont const &cont) {
        return cont.begin();
    }
    
    template <typename Ty, std::size_t Num>
    inline Ty *begin(Ty (&arr)[Num]) O3_NOEXCEPT {
        return arr;
    };
    
    template <typename Ty, std::size_t Num>
    inline Ty const *begin(Ty const (&arr)[Num]) O3_NOEXCEPT {
        return arr;
    };
    
    template <typename Ty, std::size_t Num>
    inline Ty const *cbegin(Ty const (&arr)[Num]) O3_NOEXCEPT {
        return arr;
    };
    
    template <typename Cont>
    inline typename Cont::iterator end(Cont &cont) {
        return cont.end();
    }
    
    template <typename Cont>
    inline typename Cont::const_iterator end(Cont const &cont) {
        return cont.end();
    }
    
    template <typename Cont>
    inline typename Cont::const_iterator cend(Cont const &cont) {
        return cont.end();
    }
    
    template <typename Ty, std::size_t  Num>
    inline Ty *end(Ty (&arr)[Num]) O3_NOEXCEPT {
        return arr + Num;
    };
    
    template <typename Ty, std::size_t Num>
    inline Ty const *end(Ty const (&arr)[Num]) O3_NOEXCEPT {
        return arr + Num;
    };
    
    template <typename Ty, std::size_t Num>
    inline Ty const *cend(Ty const (&arr)[Num]) O3_NOEXCEPT {
        return arr + Num;
    };
    
    template <typename Cont>
    inline typename Cont::reverse_iterator rbegin(Cont &cont) {
        return cont.rbegin();
    }
    
    template <typename Cont>
    inline typename Cont::const_reverse_iterator rbegin(Cont const &cont) {
        return cont.rbegin();
    }
    
    template <typename Cont>
    inline typename Cont::const_reverse_iterator crbegin(Cont const &cont) {
        return cont.rbegin();
    }
    
    template <typename Cont>
    inline typename Cont::reverse_iterator rend(Cont &cont) {
        return cont.rend();
    }
    
    template <typename Cont>
    inline typename Cont::const_reverse_iterator rend(Cont const &cont) {
        return cont.rend();
    }
    
    template <typename Cont>
    inline typename Cont::const_reverse_iterator crend(Cont const &cont) {
        return cont.rend();
    }
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty *> rbegin(Ty (&arr)[Num]) {
        return std::reverse_iterator<Ty *>(arr + Num);
    };
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty const *>rbegin(Ty const (&arr)[Num]) {
        return std::reverse_iterator<Ty const *>(arr + Num);
    };
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty const *>crbegin(Ty const (&arr)[Num]) {
        return std::reverse_iterator<Ty const *>(arr + Num);
    };
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty *> rend(Ty (&arr)[Num]) {
        return std::reverse_iterator<Ty *>(arr);
    };
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty const *> rend(Ty const (&arr)[Num]) {
        return std::reverse_iterator<Ty const *>(arr);
    };
    
    template <typename Ty, std::size_t Num>
    inline std::reverse_iterator<Ty const *> crend(Ty const (&arr)[Num]) {
        return std::reverse_iterator<Ty const *>(arr);
    };    
} // END of namespace app

#endif /* APP_ITERATOR_HPP */
