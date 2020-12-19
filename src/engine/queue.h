#pragma once


#include <array>
#include <atomic>
#include <optional>


namespace geena::engine
{
/* Queue
Single producer, single consumer lock-free queue. */

template<typename T, std::size_t size>
class Queue
{
public:

    Queue() : m_head(0), m_tail(0) {}
    Queue(const Queue&) = delete;


    std::optional<T> pop()
    {
        std::size_t curr = m_head.load();
        if (curr == m_tail.load())  // Queue empty, nothing to do
            return {};

        T item = m_data[curr];
        m_head.store(increment(curr));
        return {item};
    }


    bool push(const T& item)
    {
        std::size_t curr = m_tail.load();
        std::size_t next = increment(curr);

        if (next == m_head.load()) // Queue full, nothing to do
            return false;

        m_data[curr] = item;
        m_tail.store(next);
        return true;
    }

private:

    std::size_t increment(std::size_t i) const
    {
        return (i + 1) % size;
    }


    std::array<T, size> m_data;
    std::atomic<std::size_t> m_head;
    std::atomic<std::size_t> m_tail;
};
} // geena::engine::