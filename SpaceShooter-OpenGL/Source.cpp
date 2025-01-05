#include "Engine.h"

int main()
{
    Engine engine;

    if (!engine.init())
    {
        std::cout << "Failed to initialize engine" << std::endl;
        return -1;
    }

    engine.run();

    return 0;
}


