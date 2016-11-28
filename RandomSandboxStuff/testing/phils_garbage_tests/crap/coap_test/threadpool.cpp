#include "threadpool.hpp"
#include <thread.h> // ::thread_yield
#include <sched.h> // ::sched_run

namespace app
{
    threadpool::threadpool(int amt_threads)
        : m_finish(false),
          m_joined(false),
          m_amt_threads(amt_threads) {

        for (auto i = 0; i < m_amt_threads; ++i) {
            m_threads.push_back(
                std::make_shared<riot::thread>(
                    [this] {
                        return thd_fun();
                    }
                )
            );
        }
    }

    threadpool::~threadpool() {
        if (!m_joined) {
            join();
        }
    }

    int threadpool::get_thread_count() const noexcept {
        return m_amt_threads;
    }

    void threadpool::join() {
        {
            lock_type l(m_mutex);
            m_finish = true;
        }
        m_cond.notify_all();
        for (auto const &thd : m_threads) {
            if (thd->joinable()) {
                thd->join();
            }
        }
    }

    threadpool::executor_base::executor_base(int priority)
        : m_priority(priority) { }

    threadpool::executor_base::~executor_base() = default;

    bool operator<(threadpool::executor_base const &lhs,
    threadpool::executor_base const &rhs) {
        return lhs.m_priority < rhs.m_priority;
    }

    void threadpool::thd_fun() {
        auto running = true;
        while (running) {
            lock_type l(m_mutex);
            m_cond.wait(l,
                        [this] {
                            return m_finish || !m_tasks.empty();
                        });
            if (!m_tasks.empty()) {
                auto task = m_tasks.top();
                m_tasks.pop();
                l.unlock();
                (*task)();
            } else {
                running = !m_finish;
            }
            riot::this_thread::yield();
            ::sched_run();
            ::thread_yield();
        } // END while
    }
} // END of namespace app
