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
	state.audioFile = makeAudioFile("/home/mcl/audio/test sounds/sine.wav").value();

	Callback cb = [&state] (AudioBuffer& out, AudioBuffer& in, Frame bufferSize)
	{
		Frame position = state.position.load();
		Frame count    = bufferSize;

		state.audioFile.render(out, position, count);

		state.position.store(position + count);
	};

	init({ 0, 2, 44100, 1024 }, cb);

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get( input );


	return 0;
}