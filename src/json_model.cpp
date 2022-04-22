#include "include/json_model.h"

// try {
// } catch(const json::exception& exception) {
//     std::cerr << exception.what() << '\n';
// }


JsonModel::JsonModel()
{
    // std::cout
    // << "CREATE JSON MODEL"
    // << std::endl;
}

JsonModel::~JsonModel()
{
    // std::cout
    // << "DELETE JSON MODEL"
    // << std::endl;
}

result_t JsonModel::load
(void)
{
    // std::cout
    // << "LOAD JSON MODEL DATA"
    // << std::endl;

    std::string jsonfile;
    json config;

    // helper_result_t load_data_result = read_json_file("config.json",config);
    switch (read_json_file("config.json",config)) {
        case JSON_MODEL_OPEN_FILE_FAILED:
            return LOAD_ERROR_FILE_NOT_FOUND;
        case JSON_MODEL_READING_JSON_FILE_FAILED:
            return LOAD_ERROR_INVALID_CONFIG;
        case JSON_MODEL_SUCCESS:
            break;
        default:
            return INTERNAL_ERROR;
    }

    try {
        jsonfile = config["json"]["file"];
    } catch(const json::exception& exception) {
        std::cerr << exception.what() << '\n';
        return LOAD_ERROR_INVALID_CONFIG;
    }
    
    // helper_result_t load_data_result = read_json_file(jsonfile,data);
    switch (read_json_file(jsonfile,data)) {
        case JSON_MODEL_OPEN_FILE_FAILED:
            return LOAD_ERROR_FILE_NOT_FOUND;
        case JSON_MODEL_READING_JSON_FILE_FAILED:
            return LOAD_ERROR_INVALID_DATA;
        case JSON_MODEL_SUCCESS:
            break;
        default:
            return INTERNAL_ERROR;
    }

    // std::cout
    // << " json data successfully loaded\n"
    // << " jsonfile: "
    // << jsonfile
    // << std::endl;

    return SUCCESS;
}

result_t JsonModel::write
(void)
{
    // std::cout
    // << "WRITE JSON MODEL"
    // << "\n    filename: "
    // << jsonfile
    // << std::endl;
    // helper_result_t write_data_result = write_json_file(jsonfile,data);
    switch (write_json_file(jsonfile,data)) {
        case JSON_MODEL_OPEN_FILE_FAILED:
            return WRITE_ERROR_FILE_NOT_FOUND;
        case JSON_MODEL_SUCCESS:
            return SUCCESS;
        default:
            return INTERNAL_ERROR;
    }
}

std::string JsonModel::dump
(void)
{
    // std::cout
    // << "DUMP JSON MODEL"
    // << std::endl;
    return data.dump();
}

result_t JsonModel::list_events
(std::list<listed_event_t> &result)
{
    result.clear();
    for (auto event : data["events"]) {
        listed_event_t listed_event;
        listed_event.name = event["name"];
        listed_event.num_showings = event["showings"].size();
        result.push_back(listed_event);
    }
    return SUCCESS;
}

result_t JsonModel::list_event
(size_t event_index, std::list<listed_showing_compact_t> &result)
{
    result_t test_result = UNKNOWN;
    test_result = test_event_index(event_index);
    if (test_result != SUCCESS) return test_result;

    result.clear();
    size_t showing_index = 0;
    for (auto& showing : data["events"][event_index]["showings"]) {
        result.push_back(generate_compact_showing_listing(showing));
        showing_index++;
    }
    return SUCCESS;
}

result_t JsonModel::list_showing
(size_t event_index, size_t showing_index, listed_showing_extended_t &result)
{
    result_t test_result = UNKNOWN;
    test_result = test_event_index(event_index);
    if (test_result != SUCCESS) return test_result;
    test_result = test_showing_index(event_index,showing_index);
    if (test_result != SUCCESS) return test_result;

    json showing = data["events"][event_index]["showings"][showing_index];
    result.event_name = data["events"][event_index]["name"];
    result.summary = generate_compact_showing_listing(showing);
    result.pricing = get_pricing(showing);
    result.seating = get_seating(showing);
    result.tickets = get_tickets(showing);
    result.listed_seats = list_seats(showing);
    return SUCCESS;
}

result_t JsonModel::create_ticket
(size_t event_index, size_t showing_index, std::string holder_name, std::string price_name, seat_t seat)
{
    result_t test_result = UNKNOWN;
    test_result = test_event_index(event_index);
    if (test_result != SUCCESS) return test_result;
    test_result = test_showing_index(event_index,showing_index);
    if (test_result != SUCCESS) return test_result;
    test_result = test_price_name(event_index,showing_index,price_name);
    if (test_result != SUCCESS) return test_result;
    test_result = test_seat(event_index,showing_index,seat);
    if (test_result != SUCCESS) return test_result;

    // // #include <iterator>
    // std::stringstream patch_path_stream;
    // std::copy(
    //     ticket.seat.seating_groups.begin(), ticket.seat.seating_groups.end(),
    //     std::ostream_iterator<std::string>(patch_path_stream, "/")
    // );
    // std::string seat_path = "/" + patch_path_stream.str() + ticket.seat.name;
    // std::string ticket_path = seat_path + "/ticket";

    // std::string new_ticket_patch_path = std::accumulate(
    //     seating_groups.begin(), seating_groups.end(), "/", [](std::string a, std::string b) {
    //         return (std::string)(a+"/"+b);
    //     }
    // );

    // json test_seat_valid_patch = {{
    //     {"op", "test"},
    //     {"path", seat_path},
    //     {"value", {{"ticket",nullptr}}}
    // }};

    // json test_seat_taken_patch = {{
    //     {"op", "test"},
    //     {"path", ticket_path},
    //     {"value", nullptr}
    // }};

    // json test_ticket_created_patch = {{
    //     {"op", "test"},
    //     {"path", ticket_path},
    //     {"value", new_ticket_patch_value}
    // }};

    // try {
    //     seats.patch(test_seat_valid_patch);
    // } catch(const json::exception& exception) {
    //     std::cerr << exception.id << ":[" << exception.what() << "]\n";
    //     return INVALID_SEAT_REQUESTED;
    // }

    // try {
    //     seats.patch(test_seat_taken_patch);
    // } catch(const json::exception& exception) {
    //     std::cerr << exception.id << ":[" << exception.what() << "]\n";
    //     return TICKET_ALREADY_SOLD;
    // }

    return replace_ticket(event_index,showing_index,seat,{
        {"holder_name", holder_name},
        {"price", price_name}
    });
}


result_t JsonModel::delete_ticket
(size_t event_index, size_t showing_index, seat_t seat)
{
    result_t test_result = UNKNOWN;
    test_result = test_event_index(event_index);
    if (test_result != SUCCESS) return test_result;
    test_result = test_showing_index(event_index,showing_index);
    if (test_result != SUCCESS) return test_result;
    test_result = test_seat(event_index,showing_index,seat);
    if (test_result != SUCCESS) return test_result;

    return replace_ticket(event_index,showing_index,seat,
        nullptr
    );
}