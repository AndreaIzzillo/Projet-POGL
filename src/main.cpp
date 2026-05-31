#include <iostream>

#include "core/Application.hpp"

int main(int argc, char **argv)
{
    try
    {
        Application app(argc, argv);
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}