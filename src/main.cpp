#include "deps/atomic-swapper/src/atomic-swapper.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/rendering.hpp"
#include "engine/state.hpp"
#include "types.hpp"
#include "ui/mainWindow.hpp"
#include <iostream>

using namespace mcl;

namespace geena::engine
{
State                 g_state;
Data                  g_data;
AtomicSwapper<Layout> g_layout;
} // namespace geena::engine

int main()
{
	using namespace geena;

	engine::g_layout.get().state = &engine::g_state;
	engine::g_layout.swap();

	engine::kernel::Callback cb = [](AudioBuffer& out, Frame bufferSize) {
		AtomicSwapper<engine::Layout>::RtLock lock(engine::g_layout);

		const engine::Layout& layout_RT = lock.get();

		ReadStatus status   = layout_RT.state->status.load();
		Frame      position = layout_RT.state->position.load();

		if (status != ReadStatus::PLAY || layout_RT.audioFile == nullptr)
			return;

		const Frame from = position;
		const Frame to   = position + bufferSize;
		const Frame max  = layout_RT.audioFile->countFrames();

		ML_DEBUG("Render [" << from << ", " << to << ") - " << max);

		position = engine::renderer::render(*layout_RT.audioFile, out, layout_RT.pitch, from, bufferSize);

		if (to > max)
		{
			status   = ReadStatus::STOP;
			position = 0;
		}

		layout_RT.state->status.store(status);
		layout_RT.state->position.store(position);
	};

	engine::renderer::init();
	engine::kernel::init({0, 2, 44100, 4096}, cb);

	ui::MainWindow w(0, 0, 640, 480);
	int            res = w.run();

	engine::kernel::close();

	return res;
}