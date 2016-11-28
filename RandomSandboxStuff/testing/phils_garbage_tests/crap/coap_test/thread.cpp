#include "thread.hpp"
#include <exception> // std::terminate

namespace app
{
    joining_thread::~joining_thread() {
        if (m_thd.joinable()) {
            m_thd.join();
        } else {
            std::terminate();
        }
    }

    joining_thread::joining_thread(joining_thread &&other) noexcept
        : m_thd(std::move(other.m_thd)) { }

    joining_thread &joining_thread::operator=(joining_thread &&other) noexcept {
        m_thd = std::move(other.m_thd);
        return *this;
    }

    void joining_thread::swap(joining_thread &other) noexcept {
        m_thd.swap(other.m_thd);
    }    

    void swap(joining_thread &lhs, joining_thread &rhs) noexcept {
        lhs.swap(rhs);
    }
} // END of namespace app
