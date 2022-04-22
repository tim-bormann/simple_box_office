#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <string>
#include <sstream>

class Output {
    private:
        void print_seperator
            (std::string character,std::string line_end);
    public: 
        Output();
        ~Output();
        void print_welcome(void);
        void print_string(std::string str);
        void print_input_prompt(void);
        void print_input_received(void);
        void print_dashed_line(void);
        void print_call_seperator(void);
        // void flush(void);
};


#endif