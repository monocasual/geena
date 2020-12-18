#include <iostream>
#include "engine/types.hpp"
#include "engine/rendering.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"
#include "ui/mainWindow.hpp"


using namespace geena::engine;
using namespace geena::ui;


State g_state;


int main()
{
	g_state.lock();
	g_state.setAudioFile(makeAudioFile("/home/mcl/audio/test sounds/Unknown Artist - Kcik 18.wav", 44100).value());
	g_state.unlock();

	kernel::Callback cb = [&g_state] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		g_state.lock();

		Frame position = g_state.position.load();
		float pitch    = g_state.pitch.load();
		Frame count    = bufferSize;

		const AudioFile& audioFile = g_state.getAudioFile();

		position = renderer::render(audioFile, out, pitch, position, bufferSize);
//printf("%d\n ", position);
		g_state.position.store(position);

		/*
		if (position + count < audioFile.countFrames())
		{
			audioFile.render(out, position, count);
			g_state.position.store(position + count);
		}*/

		g_state.unlock();
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 1024 }, cb);

	MainWindow w;
	int res = w.run();

	kernel::close();

	return res;
}