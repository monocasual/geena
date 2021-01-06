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


int main()
{
	using namespace geena;
	using namespace geena::engine;
	using namespace geena::ui;

	engine::Layout& layout = engine::getLayout();
	engine::State&  state  = engine::getState();
	layout.status   = &state.status; 
	layout.position = &state.position;
	engine::swapLayout(layout); 

	engine::kernel::Callback cb = [] (engine::AudioBuffer& out, Frame bufferSize)
	{
		const engine::Layout& layout = engine::rt_lock();

		ReadStatus status   = layout.status->load();
		Frame      position = layout.position->load();

		if (status != ReadStatus::PLAY || layout.audioFile == nullptr)
		{
			engine::rt_unlock();
			return;
		}

		const Frame from = position;
		const Frame to   = position + bufferSize;
		const Frame max  = layout.audioFile->countFrames();
		
		G_DEBUG("Render [" << from << ", " << to << ") - " << max);

		position = renderer::render(*layout.audioFile, out, layout.pitch, from, bufferSize);

		if (to > max)
		{
			status   = ReadStatus::STOP;
			position = 0;
		}

		layout.status->store(status);
		layout.position->store(position);

		engine::rt_unlock();
	};

	renderer::init();
	kernel::init({ 0, 2, 44100, 4096 }, cb);

	MainWindow w(0, 0, 640, 480);
	int res = w.run();

	kernel::close();

	return res;
}