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
#include "src/deps/atomic-swapper/src/atomic-swapper.hpp"


using namespace monocasual;


namespace geena::engine
{
State                 g_state;
Data                  g_data;
AtomicSwapper<Layout> g_layout;
}

int main()
{
	using namespace geena;

	engine::g_layout.get().state = &engine::g_state;
	engine::g_layout.swap();

	engine::kernel::Callback cb = [] (AudioBuffer& out, Frame bufferSize)
	{
		AtomicSwapper<engine::Layout>::RtLock lock(engine::g_layout);
		const engine::Layout& layout = lock.get();

		ReadStatus status   = layout.state->status.load();
		Frame      position = layout.state->position.load();

		if (status != ReadStatus::PLAY || layout.audioFile == nullptr)
			return;

		const Frame from = position;
		const Frame to   = position + bufferSize;
		const Frame max  = layout.audioFile->countFrames();
		
		G_DEBUG("Render [" << from << ", " << to << ") - " << max);

		position = engine::renderer::render(*layout.audioFile, out, layout.pitch, from, bufferSize);

		if (to > max)
		{
			status   = ReadStatus::STOP;
			position = 0;
		}

		layout.state->status.store(status);
		layout.state->position.store(position);
	};

	engine::renderer::init();
	engine::kernel::init({ 0, 2, 44100, 4096 }, cb);

	ui::MainWindow w(0, 0, 640, 480);
	int res = w.run();

	engine::kernel::close();

	return res;
}