#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <sstream>
#include <iostream>

#include "input.h"
#include "output.h"
#include "model.h"

class Controller {
    private:
        // controller
        Model &model;
        Input &input;
        Output &output;
        void print_model_result
            (result_t result);
        void print_model_result_message
            (result_t result, std::string message);
        void process_user_input
            (std::string line_input, bool* exit_flag);

        // controller_functions
        void dump
            (void);
        void save
            (void);
        void list_events
            (void);
        void list_event
            (size_t event_index);
        void list_showing
            (size_t event_index, size_t showing_index);
        void create_ticket
            (size_t event_index, size_t showing_index,
            std::string holder_name, std::string price_name,
            std::vector<std::string> seatings_groups, std::string seat_name);
        void delete_ticket
            (size_t event_index, size_t showing_index,
            std::vector<std::string> seating_groups, std::string seat_name);
    public:
        Controller(Model &model, Input &input, Output &output);
        ~Controller();
        bool load(void);
        void run(void);
};


#endif