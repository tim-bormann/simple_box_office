#include "include/controller.h"


Controller::Controller
(Model &model, Input &input, Output &output)
: model {model}, input {input}, output {output}
{
    std::cout
    << "CREATE CONTROLLER"
    << "\n    ref_model: "
    << &model
    << "\n    ref_input: "
    << &input
    << "\n    ref_output: "
    << &output
    << std::endl;
}

Controller::~Controller(void)
{
    std::cout
    << "DELETE CONTROLLER"
    << std::endl;
}

void Controller::run(void)
{
    bool exit_flag = false;

    std::cout
    << "STARTING USER INTERFACE .."
    << std::endl;

#define USER_INPUT_TEST
#ifdef USER_INPUT_TEST
    // process_user_input(
    //     // "create ticket 0 0 testname test_priceX test_seating_group1 test_seat4"
    //     "create ticket 0 0 testname test_price1 test_seating_group1 test_seat4"
        
    //     , &exit_flag);

    process_user_input("list events", &exit_flag);
    output.print_call_seperator();
    process_user_input("list event 0", &exit_flag);
    output.print_call_seperator();
    process_user_input("list event 1", &exit_flag);
    output.print_call_seperator();
    process_user_input("list showing 0 0", &exit_flag);
    output.print_call_seperator();
    process_user_input("list showing 0 1", &exit_flag);
    output.print_call_seperator();

    output.print_call_seperator();

    process_user_input("create ticket 1 0 testname test_price1 test_seating_group1 test_seat4", &exit_flag);
    output.print_call_seperator();
    process_user_input("create ticket 0 1 testname test_price1 test_seating_group1 test_seat4", &exit_flag);
    output.print_call_seperator();
    process_user_input("create ticket 0 0 testname test_priceX test_seating_group1 test_seat4", &exit_flag);
    output.print_call_seperator();
    process_user_input("create ticket 0 0 testname test_price1 test_seating_groupX test_seat4", &exit_flag);
    output.print_call_seperator();
    process_user_input("create ticket 0 0 testname test_price1 test_seating_group1 test_seatX", &exit_flag);

    output.print_call_seperator();

    output.print_call_seperator();
    process_user_input("create ticket 0 0 testname test_price1 test_seating_group1 test_seat4", &exit_flag);

    output.print_call_seperator();
    process_user_input("delete ticket 0 0 test_seating_group1 test_seat4", &exit_flag);
    output.print_call_seperator();
    process_user_input("delete ticket 0 0 test_seating_group1 test_seat3", &exit_flag);
    output.print_call_seperator();
    process_user_input("delete ticket 0 0 test_seating_group1 test_seat2", &exit_flag);

#else
    do {
        output.print_input_prompt();
        process_user_input(input.get_line(), &exit_flag);
    } while (!exit_flag);
#endif

    std::cout
    << "EXITING USER INTERFACE .."
    << std::endl;
}
