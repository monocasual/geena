#pragma once


namespace geena::engine
{
template <typename T>
class Swapper
{
public:

    class ScopedLock
    {
    friend Swapper;
    public:

        ScopedLock(Swapper& s) : m_swapper(s)
        {
            m_swapper.rt_lock();
        }

        ~ScopedLock()
        {
            m_swapper.rt_unlock();
        }

        const T& operator*() const
        {
            return m_swapper.rt_get();
        }

    private:

        Swapper& m_swapper;
    };

    /* get
    Returns local data for non-realtime thread. */

    T& get()
    {
        return m_data[(m_bits.load() & BIT_INDEX) ^ 1];
    }

    /* onSwap
    Applies changes to data and swap it with the one currently processed by the
    realtime thread. */

    void onSwap(std::function<void(T&)> f)
    {
        T& data = get();
        f(data);
        swap(data);
    }

private:

    static constexpr int BIT_INDEX = (1 << 0); // 0001
    static constexpr int BIT_BUSY  = (1 << 1); // 0010

    /* [realtime] lock. */

    void rt_lock()
    {
        /* Set the busy bit and also get the current index. */
        m_index = m_bits.fetch_or(BIT_BUSY) & BIT_INDEX;
    }

    /* [realtime] unlock. */

    void rt_unlock()
    {
        m_bits.store(m_index & BIT_INDEX);
    }

    /* [realtime] Get data currently being ready by the rt thread. */

    const T& rt_get() const
    {
        return m_data[m_bits.load() & BIT_INDEX];
    }

    /* [normal] swap
    Swaps a given data 'd' with the existing one. */
    
    void swap(T d)
    {
        int bits    = m_bits.load();

        /* Copy the new object into the non-rt slot. */
        m_data[(bits & BIT_INDEX) ^ 1] = d;

        /* Wait for the audio thread to finish, i.e. until the BUSY bit becomes
        zero. Only then, swap indexes. This will let the audio thread to pick 
        the updated data on its next cycle. */
        int desired;
        do 
        {
            bits    = bits & ~BIT_BUSY;               // Expected: current value without busy bit set
            desired = (bits ^ BIT_INDEX) & BIT_INDEX; // Desired: flipped (xor) index
        }
        while (!m_bits.compare_exchange_weak(bits, desired));

        bits = desired;

        /* Copy the new object again in the non-rt slot, which is now the old
        one coming from the audio thread. */ 
        m_data[(bits & BIT_INDEX) ^ 1] = d;
    }

    std::array<T, 2> m_data;
    std::atomic<int> m_bits{0};
    int              m_index{0};     
};
} // geena::engine::