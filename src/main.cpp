#include <iostream>
#include "types.hpp"
#include "engine/rendering.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"
#include "engine/queue.hpp"
#include "ui/mainWindow.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
Queue<State, 32> g_queue_mainToAudio;
Queue<State, 32> g_queue_audioToMain;
State            g_state; // main thread state
}


int main()
{
	using namespace geena;
	using namespace geena::engine;
	using namespace geena::ui;

	State initialState;
	g_queue_mainToAudio.push(initialState);


	State state; // audio thread state
	kernel::Callback cb = [&state] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		while (auto o = g_queue_mainToAudio.pop())
		{
			state = o.value();
			G_DEBUG("Pop new State from main thread");
			G_DEBUG("  status=" << (int) state.status);
			G_DEBUG("  position=" << state.position);
			G_DEBUG("  pitch=" << state.pitch);
			G_DEBUG("  audioFile=" << (state.audioFile != nullptr));
		}

		if (state.status != ReadStatus::PLAY || state.audioFile == nullptr)
			return;

		const Frame from = state.position;
		const Frame to   = state.position + bufferSize;
		
		G_DEBUG("Render [" << from << ", " << to << ") - " << state.audioFile->countFrames());

		state.position = renderer::render(*state.audioFile, out, state.pitch, from, bufferSize);

		if (to > state.audioFile->countFrames())
		{
			state.status   = ReadStatus::STOP;
			state.position = 0;
		}

		// Notify main thread of audio changes. 
		if (!g_queue_audioToMain.push(state))
			puts("FULL!!!!!!!!!!!!");
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 1024 }, cb);

	MainWindow w(0, 0, 640, 480);
	int res = w.run();

	kernel::close();

	return res;
}