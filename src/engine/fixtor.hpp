#pragma once


#include <array>
#include <cassert>


namespace geena::engine
{
template<typename T, std::size_t size=1>
class Fixtor
{
public:

    Fixtor() : m_head(0) {}

    std::size_t size() const
    {
        return m_head;
    }

    T& get(std::size_t i) const
    {
        assert(i < m_head);
        return m_data[i];
    }

    bool push_back(T&& data)
    {
        if (m_head == m_data.size())
            return false; // Full
        m_data[m_head++] = std::move(data);
        return true;
    }

    void erase(std::size_t i)
    {
        assert(i < m_head);
        // TODO - swap with last one and delete last
    }

private:

    std::array<T, size> m_data;
    std::size_t         m_head;
};
} // geena::engine::