#include <iostream>
#include "engine/types.hpp"
#include "engine/rendering.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"
#include "engine/queue.hpp"
#include "engine/event.hpp"
#include "engine/circular.hpp"
#include "ui/mainWindow.hpp"
#include "utils/log.hpp"


namespace geena::engine
{
Circular<State>  g_state;
Queue<Event, 32> g_queue;
}

int main()
{
	using namespace geena::engine;
	using namespace geena::ui;

	State s;
	g_state.store(s);

	kernel::Callback cb = [] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		std::cout << "--- RENDER ----------------------------\n";

		State state = g_state.load();

		std::cout << "audioFile: " << (state.audioFile != nullptr) << "\n";

		while (auto o = g_queue.pop())
		{
			Event event = o.value();
			G_DEBUG("Pop event " << (int) event.type);
			switch (event.type)
			{
				case EventType::PLAY:
					state.status = Status::PLAY; break;
				case EventType::STOP:
					state.status = Status::STOP; break;
				case EventType::PAUSE:
					state.status = Status::PAUSE; break;
				case EventType::SET_PITCH:
					state.pitch = event.fvalue; break;
				case EventType::SET_AUDIO_FILE:
					state.audioFile = event.audioFile; break;
			}
		}

		if (state.status != Status::PLAY || state.audioFile == nullptr)
		{
			std::cout << "---------------------------------------\n";
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

		std::cout << "---------------------------------------\n";
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 1024 }, cb);

	MainWindow w;
	int res = w.run();

	kernel::close();

	return res;
}