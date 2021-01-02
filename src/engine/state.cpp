#include <cassert>
#include "state.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
void Model::rt_applyChanges()
{
    std::function<void()> func;
    while (m_queueIn.pop(func))
    {
        G_DEBUG("New command from MAIN thread");
        func();
    }
}


/* -------------------------------------------------------------------------- */


void Model::requestChange(std::function<void()>&& f)
{
    m_queueIn.push(std::move(f));
}


/* -------------------------------------------------------------------------- */


Layout Model::requestLayout()
{
    while (m_queueOutLayout.pop(m_layoutOut));

    requestChange([&queue = m_queueOutLayout, &layout = layout] 
    { 
        queue.push(Layout(layout)); 
    });

    return m_layoutOut;
}


/* -------------------------------------------------------------------------- */


State Model::requestState()
{   
    while (m_queueOutState.pop(m_stateOut));

    requestChange([&queue = m_queueOutState, &state = state] 
    { 
        queue.push(State(state)); 
    });

    return m_stateOut;
}
} // geena::engine::