#include "Engine.h"

int main()
{
    if (!Engine::init())
    {
        std::cout << "Failed to initialize engine" << std::endl;
        return -1;
    }

    Engine::run();

    return 0;
}


