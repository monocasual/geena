#include <cassert>
#include "state.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
namespace
{
constexpr int BIT_INDEX = (1 << 0); // 0001
constexpr int BIT_BUSY  = (1 << 1); // 0010

std::array<Layout, 2> layouts_;
State                 state_;
Data                  data_;
std::atomic<int>      index_{0};
int                   currIndex_ = 0; 


/* -------------------------------------------------------------------------- */


void swapLayout_(Layout l)
{
    int current = index_.load();

    /* Copy the new Layout into the non-rt slot (!current). */
    layouts_[(current & BIT_INDEX) ^ 1] = l;

    /* Wait for the audio thread to finish, i.e. until the BUSY bit becomes
    zero. Only then, swap indexes. This will let the audio thread to pick 
    the updated layout on its next cycle. */
    int desired;
    do 
    {
        current = current & ~BIT_BUSY;               // Expected: current value without busy bit set
        desired = (current ^ BIT_INDEX) & BIT_INDEX; // Desired: flipped (xor) index
    }
    while (!index_.compare_exchange_weak(current, desired));

    current = desired;

    /* Copy the new layout again in the non-rt slot, which is now the old
    one coming from the audio thread. */ 
    layouts_[(current & BIT_INDEX) ^ 1] = l;
}
} // {anonymous}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


const Layout& rt_lock()
{
    /* Set the busy bit and also get the current index. */
    currIndex_ = index_.fetch_or(BIT_BUSY) & BIT_INDEX;
    return layouts_[currIndex_]; 
}


void rt_unlock()
{
    /* Cleanup the busy bit, but keep the current index. */
    index_.store(currIndex_ & BIT_INDEX);
}


/* -------------------------------------------------------------------------- */


Layout& getLayout()
{
    /* Return the other index (!current). */
    return layouts_[(index_.load() & BIT_INDEX) ^ 1];
}


State& getState() { return state_; }
Data& getData()   { return data_; }


/* -------------------------------------------------------------------------- */


void onSwapLayout(std::function<void(Layout&)> f)
{
	Layout& layout = getLayout();
    f(layout);
	swapLayout_(layout);     
}

} // geena::engine::