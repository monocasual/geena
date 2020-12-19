#pragma once


namespace geena::engine
{
template <typename T>
class Circular
{
public:
	
	Circular() : m_index(0)
	{
	}
	
	Circular(T t)
	{
		store(t);
	}

	void store(T t)
	{
		std::size_t i_curr = m_index.load();
		std::size_t i_next = (i_curr + 1) % SIZE;

		m_array[i_curr] = t;
		
		m_index.fetch_add(i_next);
	}

	T load()
	{
		return m_array[m_index.load()];
	}

private:
	
	static constexpr std::size_t SIZE = 2;

	std::array<T, SIZE>      m_array;
	std::atomic<std::size_t> m_index;
};
} // geena::engine::