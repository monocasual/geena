#include <iostream>
#include "engine/audioEngine.hpp"


int main()
{
	using namespace geena;

	engine::audioEngine::Callback cb = [] (void* out, void* in, unsigned bufferSize)
	{
		puts("x");
	};

	engine::audioEngine::init({ 0, 2, 44100, 256 }, cb);

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get( input );


	return 0;
}