#include <iostream>
#include "engine/types.hpp"
#include "engine/rendering.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"
#include "engine/queue.hpp"
#include "engine/circular.hpp"
#include "ui/mainWindow.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
Circular<State>  g_state;
Queue<State, 32> g_queue;
}

int main()
{
	using namespace geena::engine;
	using namespace geena::ui;

	State s;
	g_state.store(s);

	kernel::Callback cb = [] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		State state = g_state.load();

		while (auto o = g_queue.pop())
		{
			state = o.value();
			G_DEBUG("Pop new State from queue");
			G_DEBUG("  status=" << (int) state.status);
			G_DEBUG("  position=" << state.position);
			G_DEBUG("  pitch=" << state.pitch);
			G_DEBUG("  audioFile=" << (state.audioFile != nullptr));
		}

		if (state.status != Status::PLAY || state.audioFile == nullptr)
		{
			g_state.store(state);
			return;
		}

		const Frame from = state.position;
		const Frame to   = state.position + bufferSize;
		
		G_DEBUG("Render [" << from << ", " << to << ") - " << state.audioFile->countFrames());

		state.position = renderer::render(*state.audioFile, out, state.pitch, from, bufferSize);

		if (to > state.audioFile->countFrames())
		{
			state.status   = Status::STOP;
			state.position = 0;
		}

		g_state.store(state);
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 1024 }, cb);

	MainWindow w;
	int res = w.run();

	kernel::close();

	return res;
}