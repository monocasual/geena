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
Model g_model;
/*
Queue<State, 32> g_queue_mainToAudio;
Queue<State, 32> g_queue_audioToMain;
State            g_state; // main thread state*/
}


int main()
{
	using namespace geena;
	using namespace geena::engine;
	using namespace geena::ui;

	kernel::Callback cb = [] (AudioBuffer& out, Frame bufferSize)
	{
		g_model.state.valid = true;

		/* Parse incoming commands from main thread, if any. */
		g_model.rt_applyChanges();

		if (g_model.state.status != ReadStatus::PLAY || g_model.layout.audioFile == nullptr)
			return;

		const Frame from = g_model.state.position;
		const Frame to   = g_model.state.position + bufferSize;
		const Frame max  = g_model.layout.audioFile->countFrames();
		
		G_DEBUG("Render [" << from << ", " << to << ") - " << max);

		g_model.state.position = renderer::render(*g_model.layout.audioFile, out, g_model.layout.pitch, from, bufferSize);

		if (to > max)
		{
			g_model.state.status   = ReadStatus::STOP;
			g_model.state.position = 0;
		}
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 4096 }, cb);

	MainWindow w(0, 0, 640, 480);
	int res = w.run();

	kernel::close();

	return res;
}