#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <sstream>

class Input {
    public: 
        Input();
        ~Input();
        std::string get_line(void);
};


#endif