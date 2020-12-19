#include <iostream>
#include "engine/types.hpp"
#include "engine/rendering.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"
#include "ui/mainWindow.hpp"
#include "utils/log.hpp"


using namespace geena::engine;
using namespace geena::ui;


State g_state;


int main()
{
	kernel::Callback cb = [] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		if (g_state.status.load() != Status::PLAY)
			return;

		State::Lock lock(g_state);

		const AudioFile* audioFile = g_state.getAudioFile();
		if (audioFile == nullptr)
			return;

		Frame position = g_state.position.load();
		float pitch    = g_state.pitch.load();
		Frame count    = bufferSize;
		
		G_DEBUG("Render [" << position << ", " << position + count << ") - " << audioFile->countFrames());

		position = renderer::render(*audioFile, out, pitch, position, bufferSize);

		if (position + count > audioFile->countFrames())
		{
			g_state.status.store(Status::OFF);
			position = 0;
		}

		g_state.position.store(position);
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 1024 }, cb);

	MainWindow w;
	int res = w.run();

	kernel::close();

	return res;
}