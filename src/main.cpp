
#include <iostream>
#include <string>
#include <sstream>

#include "include/application.h"
#include "include/json_model.h"


int main(int argc, char const *argv[])
{
    std::cout
    << "\n"
    << "################################"
    << "\n"
    << "SIMPLEBOXOFFICE DEVELOPMENT GOGO"
    << "\n"
    << "################################"
    << std::endl;

    JsonModel model;
    Application app(argc,argv,model);
    app.run();

    std::cout
    << "\n"
    << "################################"
    << "\n"
    << "SIMPLEBOXOFFICE DEVELOPMENT ENDE"
    << "\n"
    << "################################"
    << std::endl;

    return 0;
}
