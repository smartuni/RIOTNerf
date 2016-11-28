#ifndef APP_THREADPOOL_HPP
#define APP_THREADPOOL_HPP
#include <queue> // std::priority_queue
#include <memory> // std::shared_ptr
#include <vector> // std::vector
#include <list> // std::list
#include <limits> // std::numeric_limits
#include <future> // std::future, std::promise
#include <functional> // std::less, std::bind
#include <utility> // std::forward
#include <riot/thread.hpp> // riot::thread
#include <riot/condition_variable.hpp> // riot::condition_variable
#include <riot/mutex.hpp> // riot::mutex, riot::unique_lock

namespace app
{
    class threadpool final 
    {
        struct executor_base;
        template <typename Fun, typename Ret>
        struct executor;
        struct deref_comparator;

        friend bool operator<(executor_base const &lhs,
                                executor_base const &rhs);

    public:
        using this_type = threadpool;
        using mutex_type = riot::mutex;
        using lock_type = riot::unique_lock<mutex_type>;
        using cv_type = riot::condition_variable;
        
        explicit threadpool(int amt_threads = 4);

        threadpool(this_type const &) = delete;
        this_type &operator=(this_type const &) = delete;

        ~threadpool();

        int get_thread_count() const noexcept;

        template <typename Fun, typename ...Args>
        auto add_task(Fun &&fun, Args &&...args) {
            return add_task(std::numeric_limits<int>::min(),
                            std::forward<Fun>(fun), 
                            std::forward<Args>(args)...);
        }

        template <typename Fun, typename ...Args>
        auto add_task(int priority, Fun &&fun, Args &&...args) {
            using Ret = decltype(std::forward<Fun>(fun)(std::forward<Args>(args)...));
            lock_type l(m_mutex);
            auto ptr = std::make_shared<executor<
                                           decltype(std::bind(std::forward<Fun>(fun),
                                                              std::forward<Args>(args)...)),
                                           Ret>>(
                std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...), priority);
            m_tasks.push(ptr);
            m_cond.notify_one();
            return ptr->m_result.get_future();
        }

        void join();

    private:
        struct executor_base {
            using this_type = executor_base;

            explicit executor_base(int priority = std::numeric_limits<int>::max());
            
            virtual ~executor_base();
            
            virtual void operator()() = 0;

            int m_priority;
        }; // END of struct executor_base

        template <typename Fun, typename Ret>
        struct executor final 
            : public executor_base {
            using this_type = executor;
            using base_type = executor_base;

            executor(Fun &&fun, int priority)
                : base_type(priority),
                  m_fun(std::forward<Fun>(fun)) { }

            virtual void operator()() override {
                try {
                    m_result.set_value(m_fun());
                } catch (...) {
                    m_result.set_exception(std::current_exception());
                }
            }

            Fun m_fun;
            std::promise<Ret> m_result;
        }; // END of struct executor

        template <typename Fun>
        struct executor<Fun, void> final 
            : public executor_base {
            using this_type = executor;
            using base_type = executor_base;

            executor(Fun &&fun, int priority)
                : base_type(priority),
                  m_fun(std::forward<Fun>(fun)) { }

            virtual void operator()() override {
                try {
                    m_fun();
                    m_result.set_value();
                } catch (...) {
                    m_result.set_exception(std::current_exception());
                }
            } 

            Fun m_fun;
            std::promise<void> m_result;  
        }; // END of struct executor

        struct deref_comparator final {
            using this_type = deref_comparator;

            template <typename Ptr>
            bool operator()(Ptr const &lhs, Ptr const &rhs) {
                std::less<decltype(*lhs)> l{ };
                return l(*lhs, *rhs);
            }
        }; // END of struct deref_comparator

        void thd_fun();

        std::priority_queue<
            std::shared_ptr<executor_base>,
            std::vector<std::shared_ptr<executor_base>>,
            deref_comparator> 
        m_tasks;
        std::list<std::shared_ptr<riot::thread>> m_threads;
        mutex_type m_mutex;
        cv_type m_cond;
        bool m_finish;
        bool m_joined;
        int m_amt_threads;
    }; // END of class threadpool
} // END of namespace app

#endif /* APP_THREADPOOL_HPP */
