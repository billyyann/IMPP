#include "ThreadPool.h"


ThreadPool::ThreadWorker::ThreadWorker(ThreadPool* pool, const int id) : m_pool(pool), m_id(id)
{
}

void ThreadPool::ThreadWorker::operator()()
{
    std::function<void()> func;
    bool dequeued;
    while (!m_pool->m_shutdown)
    {
        {
            std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
            if (m_pool->m_queue.empty())
            {
                m_pool->m_conditional_lock.wait(lock);
            }
            dequeued = m_pool->m_queue.dequeue(func);
        }
        if (dequeued)
            func();
    }
}


void ThreadPool::init()
{
    for (int i = 0; i < m_threads.size(); ++i)
    {
        m_threads.at(i) = std::thread(ThreadWorker(this, i));
    }
}

void ThreadPool::shutdown()
{
    m_shutdown = true;
    m_conditional_lock.notify_all();
    for (int i = 0; i < m_threads.size(); ++i)
    {
        if (m_threads.at(i).joinable())
        {
            m_threads.at(i).join();
        }
    }
}
