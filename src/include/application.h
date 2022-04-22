#ifndef APPLICATION_H
#define APPLICATION_H

#include "input.h"
#include "output.h"
#include "model.h"
#include "controller.h"

class Application {
    private:
        Input input;
        Output output;
        Model &model;
        Controller controller;
    public:
        Application(int argc, char const *argv[], Model &model);
        ~Application();
        void run(void);
};



#endif