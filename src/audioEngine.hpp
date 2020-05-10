#include <functional>


namespace geena::m::audioEngine
{
using Callback = std::function<void(void*, void*, unsigned)>;

struct Config
{
    int deviceId;
    int channels;
    int sampleRate;
    int bufferSize;
};

bool init (Config c, Callback f);
void close();
} // geena::m::