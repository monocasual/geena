#include "const.hpp"
#include "core/audioFile.hpp"
#include "core/audioFileFactory.hpp"
#include "core/config.hpp"
#include "core/engine.hpp"
#include "deps/atomic-swapper/src/atomic-swapper.hpp"
#include "deps/mcl-utils/src/log.hpp"
#include "types.hpp"
#include "ui/mainWindow.hpp"
#include <string>
#include <vector>

using namespace mcl;

namespace geena::core
{
Engine g_engine;
} // namespace geena::core

int main(int argc, char** argv)
{
	using namespace geena;

	core::Kernel::Callback cb = [renderer = &core::g_engine.renderer](AudioBuffer& out) {
		AtomicSwapper<core::Layout>::RtLock lock(core::g_engine.layout);

		const core::Layout& layout_RT = lock.get();

		if (!layout_RT.enabled)
			return;

		ReadStatus status   = layout_RT.shared->status.load();
		Frame      position = layout_RT.shared->position.load();

		if (status != ReadStatus::PLAY || !layout_RT.shared->audioFile.isValid())
			return;

		const Frame max = layout_RT.shared->audioFile.countFrames();

		position = renderer->render(layout_RT.shared->audioFile, out, layout_RT.pitch, position);
		if (layout_RT.playMode == PlayMode::SEEK)
		{
			// if position > layout_RT.seekPoint + G_SEEK_LENGTH ---> rewind to layout_RT.seekPoint
			if (position > layout_RT.seekPoint + G_SEEK_LENGTH)
				position = layout_RT.seekPoint;
		}

		if (position >= max)
		{
			status   = ReadStatus::STOP;
			position = 0;
		}

		layout_RT.shared->status.store(status);
		layout_RT.shared->position.store(position);
	};

	/* TODO - move to engine::init() */
	/* TODO - move to engine::init() */
	/* TODO - move to engine::init() */
	std::vector<std::string> args(argv, argv + argc);

	auto configOpt = core::config::read(args[1]);
	if (!configOpt)
		return 1;

	core::Config config = configOpt.value();

	core::g_engine.kernel.init(config, cb);
	core::g_engine.renderer.init(config.bufferSize, 1);
	/* TODO - move to engine::init() */
	/* TODO - move to engine::init() */
	/* TODO - move to engine::init() */

	ui::MainWindow w(0, 0, 640, 700);
	int            res = w.run();

	core::g_engine.kernel.close();

	return res;
}