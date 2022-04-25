#include "include/controller.h"

#define reset "\033[0m"
#define bold "\033[1m"
#define faint "\033[2m"
#define underline "\033[4m"

void Controller::print_model_result(result_t result)
{
    switch (result) {
        case SUCCESS:
            output.print_string("  SUCCESS");
            break;
        case INVALID_EVENT_REQUESTED:
            output.print_string("  INVALID_EVENT_REQUESTED");
            break;
        case INVALID_SHOWING_REQUESTED:
            output.print_string("  INVALID_SHOWING_REQUESTED");
            break;
        case INVALID_SEATING_GROUP_REQUESTED:
            output.print_string("  INVALID_SEATING_GROUP_REQUESTED");
            break;
        case INVALID_SEAT_REQUESTED:
            output.print_string("  INVALID_SEAT_REQUESTED");
            break;
        case INVALID_PRICE_REQUESTED:
            output.print_string("  INVALID_PRICE_REQUESTED");
            break;
        case TICKET_ALREADY_SOLD:
            output.print_string("  TICKET_ALREADY_SOLD");
            break;
        case TICKET_NOT_SOLD:
            output.print_string("  TICKET_NOT_SOLD");
            break;
        case INTERNAL_ERROR:
            output.print_string("  INTERNAL_ERROR");
            break;
        case LOAD_ERROR_FILE_NOT_FOUND:
            output.print_string("  LOAD_ERROR_FILE_NOT_FOUND");
            break;
        case LOAD_ERROR_INVALID_CONFIG:
            output.print_string("  LOAD_ERROR_INVALID_CONFIG");
            break;
        case LOAD_ERROR_INVALID_DATA:
            output.print_string("  LOAD_ERROR_INVALID_DATA");
            break;
        case WRITE_ERROR_FILE_NOT_FOUND:
            output.print_string("  WRITE_ERROR_FILE_NOT_FOUND");
            break;
        case UNKNOWN:
            output.print_string("  UNKNOWN");
            break;
        default:
            output.print_string("  INTERNAL ERROR: RESULT CODE NOT RECOGNIZED");
            break;
    }
}

void Controller::print_model_result_message(result_t result, std::string message)
{
    if (result != SUCCESS) {
        print_model_result(result);
    } else {
        output.print_string(message);
    }
}


bool Controller::load(void)
{
    result_t model_result = model.load();

    if (model_result != SUCCESS) {
        print_model_result(model_result);
        return false;
    } else {
        output.print_string("  Data successfully saved!");
        return true;
    }
}

void Controller::dump(void)
{
    output.print_string(model.dump());
}


void Controller::save(void)
{
    print_model_result_message(
        model.write(),
        "  Data successfully saved!"
    );
}

void Controller::list_events()
{
    std::list<listed_event_t> listed_events;

    result_t model_result = model.list_events(listed_events);

    if (model_result != SUCCESS) {
        print_model_result(model_result);
    } else {
        size_t index = 0;
        std::stringstream result;
        result
        << underline << faint
        << "Table Legend"
        << reset
        << "\n"
        << "[index] Event Name: number of Showings\n"
        << "--------------------------------------\n";
        for (auto& event : listed_events) {
            result
            << "["
            << index
            << "] "
            << event.name
            << ": "
            << event.num_showings
            << "\n";
            index++;
        }
        output.print_string(result.str());
    }
}


void Controller::list_event(size_t event_index)
{
    std::list<listed_showing_compact_t> listed_event;

    result_t model_result = model.list_event(event_index,listed_event);

    if (model_result != SUCCESS) {
        print_model_result(model_result);
    } else {
        size_t index = 0;
        std::stringstream result;
        result
        << underline << faint
        << "Table Legend"
        << reset
        << "\n"
        << "[index] Showing Name  #sold #available\n"
        << "--------------------------------------\n";
        for (auto& showing : listed_event) {
            result
            << "["
            << index
            << "] "
            << showing.name
            << "   "
            << showing.num_tickets_sold
            << "  "
            << showing.num_tickets_available
            << "\n";
            index++;
        }
        output.print_string(result.str());
    }
}

void Controller::list_showing(size_t event_index, size_t showing_index)
{
    listed_showing_extended_t listed_showing;

    result_t model_result = model.list_showing(event_index,showing_index,listed_showing);

    if (model_result != SUCCESS) {
        print_model_result(model_result);
    } else {
        std::stringstream result;
        result
        << "Event: " << listed_showing.event_name << "\n"
        << "Showing: " << listed_showing.summary.name << "\n"
        << "Date/Time: " << listed_showing.summary.datetime << "\n"
        << "Tickets Sold: " << listed_showing.summary.num_tickets_sold << "\n"
        << "Tickets Available: " << listed_showing.summary.num_tickets_available << "\n"
        << "--------------------------------------\n";
        result
        << "Pricing: " << listed_showing.pricing.name << "\n";
        for (auto& price : listed_showing.pricing.prices) {
            result
            << price.first
            << "= "
            << price.second
            << "\n";
        }
        result
        << "------------------------------\n";
        result
        << "Tickets:\n";
        for (auto& listed_seat : listed_showing.listed_seats) {
            std::string seating_groups_str;
            switch (listed_seat.seat.seating_groups.size())
            {
                case 0:
                    seating_groups_str = "None";
                    break;
                case 1:
                    seating_groups_str = listed_seat.seat.seating_groups.front();
                    break;
                default:
                    for (auto seating_group_name : listed_seat.seat.seating_groups) {
                        seating_groups_str += (seating_group_name+"|");
                    }
                    break;
            }
            result
            << "[" << seating_groups_str << "] "
            << listed_seat.seat.name
            << " : ";
            if (listed_seat.ticket_sold)
                result << "Sold\n";
            else
                result << "Available\n";
        }
        output.print_string(result.str());
    }
}

void Controller::create_ticket
(size_t event_index, size_t showing_index,
std::string holder_name, std::string price_name, std::vector<std::string> seating_groups, std::string seat_name)
{
    seat_t seat;
    seat.name = seat_name;
    seat.seating_groups = seating_groups;

    print_model_result_message(
        model.create_ticket(event_index,showing_index,holder_name,price_name,seat),
        "  Ticket successfully created!"
    );
}

void Controller::delete_ticket
(size_t event_index, size_t showing_index,
std::vector<std::string> seating_groups, std::string seat_name)
{
    seat_t seat;
    seat.name = seat_name;
    seat.seating_groups = seating_groups;

    print_model_result_message(
        model.delete_ticket(event_index,showing_index,seat),
        "  Ticket successfully deleted!"
    );
}
