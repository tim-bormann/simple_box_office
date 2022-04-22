#include "include/output.h"

Output::Output()
{
    std::cout
    << "INITIALIZING DISPLAY .."
    << std::endl;
}

Output::~Output()
{
    std::cout
    << "SHUTTING DOWN DISPLAY .."
    << std::endl;
}

void Output::print_welcome(void)
{
    std::cout
    << "WELCOME TO"
    << "SIMPLE BOX OFFICE DEVELOPMENT"
    << std::endl;
}

void Output::print_string
(std::string str)
{
    std::cout
    << str
    << std::endl;
}


#define SEPERATOR_SIZE 32
void Output::print_seperator
(std::string character,std::string line_end)
{
    std::string line = "";
    for (size_t i=0; i<SEPERATOR_SIZE; i++)
        line += character;
    std::cout
    << line
    << line_end;
}


void Output::print_input_prompt(void)
{
    print_seperator("_","\n");
    std::cout
    << "INPUT:\n";
    print_seperator("=","");
    std::cout
    << std::endl;
}


void Output::print_input_received(void)
{
    print_seperator("#","\n");
    std::cout
    << "### processing user input .. ###\n";
    print_seperator("#","\n");
}

void Output::print_dashed_line(void)
{
    print_seperator("-","\n");
}

void Output::print_call_seperator(void)
{
    std::cout << "\n\n\n";
    print_seperator("-","");
    std::cout << "\n\n\n";
}
