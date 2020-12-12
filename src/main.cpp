#include <iostream>
#include "engine/types.hpp"
#include "engine/audioEngine.hpp"
#include "engine/audioFile.hpp"
#include "engine/audioFileFactory.hpp"
#include "engine/state.hpp"


int main()
{
	using namespace geena::engine;

	State state;
	state.setAudioFile(makeAudioFile("/home/mcl/audio/test sounds/sine.wav").value());

	Callback cb = [&state] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		state.lock();

		Frame position = state.position.load();
		Frame count    = bufferSize;

		state.getAudioFile().render(out, position, count);

		state.position.store(position + count);

		state.unlock();
	};

	init({ 0, 2, 44100, 1024 }, cb);

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get( input );


	return 0;
}