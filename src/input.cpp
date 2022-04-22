#include "include/input.h"

Input::Input()
{
    std::cout
    << "INITIALIZING USER INPUT .."
    << std::endl;
}

Input::~Input()
{
    std::cout
    << "SHUTTING DOWN USER INPUT .."
    << std::endl;
}

std::string Input::get_line(void)
{
    std::string line_input;
    std::getline(std::cin,line_input);
    return line_input;
}

