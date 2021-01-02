#include <cassert>
#include "state.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
void Model::applyChanges()
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
    requestChange([&queue = m_queueOutLayout, &layout = layout] 
    { 
        queue.push(Layout(layout)); 
    });

    Layout layout;
    while (m_queueOutLayout.pop(layout));
    return layout;
}


/* -------------------------------------------------------------------------- */


State Model::requestState()
{
    requestChange([&queue = m_queueOutState, &state = state] 
    { 
        queue.push(State(state)); 
    });
    
    State state;
    while (m_queueOutState.pop(state));
    return state;
}
} // geena::engine::