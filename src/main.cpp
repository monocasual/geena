#include "core/audioFile.hpp"
#include "core/audioFileFactory.hpp"
#include "core/engine.hpp"
#include "core/kernel.hpp"
#include "core/rendering.hpp"
#include "core/state.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include "types.hpp"
#include "ui/mainWindow.hpp"
#include <iostream>

using namespace mcl;

namespace geena::core
{
Engine g_engine;
} // namespace geena::core

int main()
{
	using namespace geena;

	core::Renderer renderer;
	core::Kernel   kernel;

	core::Kernel::Callback cb = [&renderer](AudioBuffer& out, Frame bufferSize) {
		AtomicSwapper<core::Layout>::RtLock lock(core::g_engine.layout);

		const core::Layout& layout_RT = lock.get();

		if (!layout_RT.enabled)
			return;

		ReadStatus status   = layout_RT.shared->status.load();
		Frame      position = layout_RT.shared->position.load();

		if (status != ReadStatus::PLAY || !layout_RT.shared->audioFile.isValid())
			return;

		const Frame from = position;
		const Frame to   = position + bufferSize;
		const Frame max  = layout_RT.shared->audioFile.countFrames();

		ML_DEBUG("Render [" << from << ", " << to << ") - " << max);

		position = renderer.render(layout_RT.shared->audioFile, out, layout_RT.pitch, from, bufferSize);

		if (to > max)
		{
			status   = ReadStatus::STOP;
			position = 0;
		}

		layout_RT.shared->status.store(status);
		layout_RT.shared->position.store(position);
	};

	kernel.init({0, 2, 44100, 4096}, cb);
	renderer.init();

	ui::MainWindow w(0, 0, 640, 480);
	int            res = w.run();

	kernel.close();

	return res;
}