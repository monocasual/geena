#include <iostream>
#include "audioEngine.hpp"


int main()
{
	using namespace geena;

	m::audioEngine::Callback cb = [] (void* out, void* in, unsigned bufferSize)
	{
		puts("x");
	};

	m::audioEngine::init({ 0, 2, 44100, 256 }, cb);

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get( input );


	return 0;
}