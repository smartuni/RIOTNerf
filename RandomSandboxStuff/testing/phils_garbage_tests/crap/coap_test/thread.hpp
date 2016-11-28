#ifndef APP_THREAD_HPP
#define APP_THREAD_HPP
#include <utility> // std::forward, std::move
#include <riot/thread.hpp> // riot::thread

namespace app
{
    class joining_thread {
    public:
        using this_type = joining_thread;

        template <typename ...Args>
        joining_thread(Args &&...args)
            : m_thd(std::forward<Args>(args)...) { }

        ~joining_thread();

        joining_thread(this_type const &) = delete;

        joining_thread(this_type &&other) noexcept;

        this_type &operator=(this_type const &) = delete;

        this_type &operator=(this_type &&other) noexcept;

        void swap(this_type &other) noexcept;
    private:
        riot::thread m_thd;
    }; // END of class joining_thread

    void swap(joining_thread &lhs, joining_thread &rhs) noexcept;
} // END of namespace app
#endif /* APP_THREAD_HPP */
