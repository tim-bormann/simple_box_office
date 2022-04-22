#include "include/application.h"

Application::Application(int argc, char const *argv[], Model &model)
:model {model}, controller {model,input,output}
{
    std::cout
    << "INITIALIZING APPLICATION .."
    << "\nargc: "
    << argc;
    for (int i=0; i<argc; i++) {
        std::cout
        << "\nargv["
        << i
        << "]: "
        << argv[i]
        << "\n";
    }
    std::cout
    << std::endl;
}

Application::~Application()
{
    std::cout
    << "SHUTTING DOWN APPLICATION .."
    << std::endl;
}

void Application::run(void)
{
    std::cout
    << "LOAD DATA"
    << std::endl;
    if(controller.load()) {
        std::cout
        << "DATA LOADED" << "\n"
        << "RUN APPLICATION"
        << std::endl;
        controller.run();
    } else {
        std::cout
        << "LOAD DATA FAILED" << "\n"
        << "EXITING .."
        << std::endl;
    }

}
