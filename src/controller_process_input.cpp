#include "include/controller.h"

std::vector<std::string> split_input(std::string input, const char separator)
{
    std::vector<std::string> result;
    std::stringstream input_stream(input);
    std::string token;
    while (std::getline(input_stream, token, separator)) {
        result.emplace(result.begin(),token);
        // result.push_back(token);
    }
    return result;
}

std::string get_next_input(std::vector<std::string> &inputs)
{
    std::string result = inputs.back();
    inputs.pop_back();
    return result;
}

bool parse_index(std::string input, size_t &index)
{
    try {
        index = std::stoi(input);
        return true;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

// bool parse_seating_groups(std::string input, std::vector<std::string> seating_groups_str)
// {

// }


void Controller::process_user_input
(std::string line_input, bool* exit_flag)
{
    output.print_input_received();
    output.print_dashed_line();

    std::vector<std::string> inputs = split_input(line_input,' ');

    if (inputs.empty()) {
        std::cout
        << "EMPTY INPUT!"
        << std::endl;
        return;
    }

    std::string command = get_next_input(inputs);

    if (command == "dump") {
        std::cout
        << "DUMP DATABASE"
        << std::endl;
        dump();
    }
    else
    if (command == "save") {
        std::cout
        << "SAVE CURRENT DATA"
        << std::endl;
        save();
    }
    else
    if (command == "list") {
        if (inputs.size() > 0) {
            std::string list_target = get_next_input(inputs);
            if (list_target == "events") {
                if (inputs.size() == 0) {
                    std::cout
                    << "LIST EVENTS"
                    << std::endl;
                    list_events();
                } else {
                    std::cout
                    << "LIST EVENTS PARAMETER COUNT ERROR!"
                    << "\nshould be 0, is "
                    << inputs.size()
                    << std::endl;
                }
            }
            else
            if (list_target == "event") {
                if (inputs.size() == 1) {
                    std::string event_ident = get_next_input(inputs);
                    size_t event_index = 0;
                    if(parse_index(event_ident, event_index)) {
                        std::cout
                        << "LIST EVENT"
                        << "\nevent_index: "
                        << event_index
                        << std::endl;
                        list_event(event_index);
                    } else {
                        std::cout
                        << "LIST EVENT PARAMETER PARSING ERROR!"
                        << std::endl;
                    }
                } else {
                    std::cout
                    << "LIST EVENT PARAMETER COUNT ERROR!"
                    << "\nshould be 1, is "
                    << inputs.size()
                    << std::endl;
                }
            }
            else
            if (list_target == "showing") {
                if (inputs.size() == 2) {
                    std::string event_ident = get_next_input(inputs);
                    std::string showing_ident = get_next_input(inputs);
                    size_t event_index = 0;
                    size_t showing_index = 0;
                    if( parse_index(event_ident, event_index) &&
                        parse_index(showing_ident, showing_index) )
                    {
                        std::cout
                        << "LIST SHOWING"
                        << "\nevent_index: "
                        << event_index
                        << "\nshowing_index: "
                        << showing_index
                        << std::endl;
                        list_showing(event_index,showing_index);
                    } else {
                        std::cout
                        << "LIST SHOWING PARAMETER PARSING ERROR!"
                        << std::endl;
                    }
                } else {
                    std::cout
                    << "LIST SHOWING PARAMETER COUNT ERROR!"
                    << "\nshould be 2, is "
                    << inputs.size()
                    << std::endl;
                }
            }
            else {
                std::cout
                << "INVALID LIST PARAMETERS!"
                << std::endl;
            }
        }
        else {
            std::cout
            << "MISSING LIST PARAMETERS!"
            << std::endl;
        }
    }
    else
    if (command == "create") {
        if (inputs.size() > 0) {
            std::string create_target = get_next_input(inputs);
            if (create_target == "ticket") {
                if (inputs.size() == 6) {
                    // event_index
                    // showing_index
                    // holder_name
                    // price_name
                    // seatings_groups
                    // seat_name
                    size_t param_index = 0;
                    for (auto param: inputs) {
                        std::cout
                        << param_index
                        << ":["
                        << param
                        << "]\n";
                        param_index++;
                    }
                    std::string event_ident = get_next_input(inputs);
                    std::string showing_ident = get_next_input(inputs);
                    std::string holder_name = get_next_input(inputs);
                    std::string price_name = get_next_input(inputs);
                    std::string seatings_groups_str = get_next_input(inputs);
                    std::vector<std::string> seatings_groups = split_input(seatings_groups_str,',');
                    std::string seat_name = get_next_input(inputs);
                    size_t event_index = 0;
                    size_t showing_index = 0;
                    if( parse_index(event_ident, event_index) &&
                        parse_index(showing_ident, showing_index) )
                    {
                        std::cout
                        << "CREATE TICKET"
                        << "\nevent_index: "
                        << event_index
                        << "\nshowing_index: "
                        << showing_index
                        << "\nholder_name: "
                        << holder_name
                        << "\nprice_name: "
                        << price_name
                        << "\nseatings_groups(" << seatings_groups.size() << "): [ ";
                        for (auto seating_group: seatings_groups)
                            std::cout << seating_group << " ";
                        std::cout
                        << "]\n"
                        << "seat_name: "
                        << seat_name
                        << std::endl;
                        create_ticket(event_index,showing_index,holder_name,price_name,seatings_groups,seat_name);
                    } else {
                        std::cout
                        << "CREATE TICKET PARAMETER PARSING ERROR!"
                        << std::endl;
                    }
                } else {
                    std::cout
                    << "CREATE TICKET PARAMETER COUNT ERROR!"
                    << "\nshould be 6, is "
                    << inputs.size()
                    << std::endl;
                }
            }
        }
        else {
            std::cout
            << "MISSING CREATE PARAMETERS!"
            << std::endl;
        }
    }
    else
    if (command == "delete") {
        if (inputs.size() > 0) {
            std::string create_target = get_next_input(inputs);
            if (create_target == "ticket") {
                if (inputs.size() == 4) {
                    // event_index
                    // showing_index
                    // seatings_groups
                    // seat_name
                    size_t param_index = 0;
                    for (auto param: inputs) {
                        std::cout
                        << param_index
                        << ":["
                        << param
                        << "]\n";
                        param_index++;
                    }
                    std::string event_ident = get_next_input(inputs);
                    std::string showing_ident = get_next_input(inputs);
                    std::string seatings_groups_str = get_next_input(inputs);
                    std::vector<std::string> seatings_groups = split_input(seatings_groups_str,',');
                    std::string seat_name = get_next_input(inputs);
                    size_t event_index = 0;
                    size_t showing_index = 0;
                    if( parse_index(event_ident, event_index) &&
                        parse_index(showing_ident, showing_index) )
                    {
                        std::cout
                        << "DELETE TICKET"
                        << "\nevent_index: "
                        << event_index
                        << "\nshowing_index: "
                        << showing_index
                        << "\nseatings_groups(" << seatings_groups.size() << "): [ ";
                        for (auto seating_group: seatings_groups)
                            std::cout << seating_group << " ";
                        std::cout
                        << "]\n"
                        << "seat_name: "
                        << seat_name
                        << std::endl;
                        delete_ticket(event_index,showing_index,seatings_groups,seat_name);
                    } else {
                        std::cout
                        << "DELETE TICKET PARAMETER PARSING ERROR!"
                        << std::endl;
                    }
                } else {
                    std::cout
                    << "DELETE TICKET PARAMETER COUNT ERROR!"
                    << "\nshould be 6, is "
                    << inputs.size()
                    << std::endl;
                }
            }
        }
        else {
            std::cout
            << "MISSING DELETE PARAMETERS!"
            << std::endl;
        }
    }
    else
    if (command == "exit") {
        *exit_flag = true;
        std::cout
        << "EXIT FLAG SET!\n"
        << "EXITING .."
        << std::endl;

    }
    else {
        std::cout
        << "INVALID INPUT!"
        << std::endl;
    }
    output.print_dashed_line();

    // if (line_input == "dump") {
    //     std::cout
    //     << "DUMP DATABASE"
    //     << std::endl;
    //     dump();
    // }
    // else
    // if (line_input == "save") {
    //     std::cout
    //     << "SAVE CURRENT DATA"
    //     << std::endl;
    //     save();
    // }
    // else
    // if (line_input == "list events") {
    //     std::cout
    //     << "LIST EVENTS"
    //     << std::endl;
    //     list_events();
    // }
    // else
    // if (line_input == "exit") {
    //     *exit_flag = true;
    //     std::cout
    //     << "\nEXIT FLAG SET!\nEXITING .."
    //     << std::endl;

    // }
    // else {
    //     std::cout
    //     << "\nINVALID INPUT!"
    //     << std::endl;
    // }
}
