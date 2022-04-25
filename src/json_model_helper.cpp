#include "include/json_model.h"

helper_result_t JsonModel::read_json_file
(std::string path, json &result)
{
    std::ifstream input_stream(path);
    if (!input_stream.is_open())
        return JSON_MODEL_OPEN_FILE_FAILED;
    try {
        input_stream >> result;
    } catch(const json::exception& exception) {
        std::cerr << exception.what() << '\n';
        input_stream.close();
        return JSON_MODEL_READING_JSON_FILE_FAILED;
    }
    input_stream.close();
    return JSON_MODEL_SUCCESS;
}

helper_result_t JsonModel::write_json_file
(std::string path, json &json_object)
{
    std::ofstream output_stream(path);
    if (!output_stream.is_open())
        return JSON_MODEL_OPEN_FILE_FAILED;
    // std::setw(4) for pretty printing
    output_stream << std::setw(4) << json_object;
    output_stream.flush();
    output_stream.close();
    return JSON_MODEL_SUCCESS;
}

helper_result_t JsonModel::parse_datetime_string
(std::string datetime_str, asap::datetime &result)
{
    // asap library example documentation:
    //   setting locale helps with local time format
    setlocale(LC_ALL, "");
    asap::datetime parsed_datetime(datetime_str);
    result = parsed_datetime;
    return JSON_MODEL_SUCCESS;
}

void JsonModel::count_tickets
(json &seats, size_t &num_tickets_sold, size_t &num_tickets_available)
{
    for (auto& seating_element : seats) {
        if (seating_element.contains("ticket")) {
            if (seating_element["ticket"].is_null()) {
                num_tickets_available++;
            }
            else {
                num_tickets_sold++;
            }
        }
        else {
            count_tickets(seating_element,num_tickets_sold,num_tickets_available);
        }
    }
}

listed_showing_compact_t JsonModel::generate_compact_showing_listing(json &showing)
{
    listed_showing_compact_t result;
    result.name = showing["name"];
    parse_datetime_string(showing["datetime"],result.datetime);
    result.num_tickets_sold = 0;
    result.num_tickets_available = 0;
    count_tickets(showing["seats"],result.num_tickets_sold,result.num_tickets_available);
    return result;
}


pricing_t JsonModel::get_pricing(json &showing)
{
    pricing_t result;
    result.name = showing["pricing"].begin().key();
    for (
        json::iterator price = showing["pricing"][result.name].begin();
        price != showing["pricing"][result.name].end();
        ++price
    ) {
        result.prices.insert(std::pair<std::string,float>(price.key(),price.value()));
    }
    return result;
}

void JsonModel::collect_seats
(json &seating_subset, std::vector<std::string> seating_groups, std::vector<seat_t> &result)
{
    for (
        json::iterator seating_element = seating_subset.begin();
        seating_element != seating_subset.end();
        ++seating_element
    ) {
        if (seating_element.value().contains("ticket")) {
            seat_t new_seat;
            new_seat.name = seating_element.key();
            new_seat.seating_groups = seating_groups;
            result.push_back(new_seat);
        }
        else {
            std::vector<std::string> seating_groups_next(seating_groups);
            seating_groups_next.push_back(seating_element.key());
            collect_seats(seating_element.value(),seating_groups_next,result);
        }
    }
}

seating_t JsonModel::get_seating(json &showing)
{
    seating_t result;
    result.venue_name = showing["venue"];
    result.room_name = showing["room"];
    std::vector<std::string> seating_groups;
    json seating = data["venues"][result.venue_name][result.room_name]["seating"];
    collect_seats(seating,seating_groups,result.seats);
    return result;
}

void JsonModel::collect_tickets
(json &seats_subset, std::vector<std::string> seating_groups, std::string pricing_name, std::vector<ticket_t> &result)
{
    for (
        json::iterator seating_element = seats_subset.begin();
        seating_element != seats_subset.end();
        ++seating_element
    ) {
        if (seating_element.value().contains("ticket")) {
            if (!(seating_element.value()["ticket"].is_null())) {
                ticket_t new_ticket;
                new_ticket.holder_name = seating_element.value()["ticket"]["holder_name"];
                seat_t new_seat;
                new_seat.name = seating_element.key();
                new_seat.seating_groups = seating_groups;
                new_ticket.seat = new_seat;
                std::string price_name = seating_element.value()["ticket"]["price"];
                float price_value = data["pricing"][pricing_name][price_name];
                price_t new_price = price_t(price_name,price_value);
                new_ticket.price = new_price;
                result.push_back(new_ticket);
            }
        }
        else {
            std::vector<std::string> seating_groups_next(seating_groups);
            seating_groups_next.push_back(seating_element.key());
            collect_tickets(seating_element.value(),seating_groups_next,pricing_name,result);
        }
    }
}

std::vector<ticket_t> JsonModel::get_tickets(json &showing)
{
    std::vector<ticket_t> result;
    std::vector<std::string> seating_groups;
    collect_tickets(showing["seats"],seating_groups,showing["pricing"].begin().key(),result);
    return result;
}

void JsonModel::collect_listed_seats
(json &seating_subset, std::vector<std::string> seating_groups, std::vector<listed_seat_t> &result)
{
    for (
        json::iterator seating_element = seating_subset.begin();
        seating_element != seating_subset.end();
        ++seating_element
    ) {
        if (seating_element.value().contains("ticket")) {
            listed_seat_t new_listed_seat;
            new_listed_seat.seat.name = seating_element.key();
            new_listed_seat.seat.seating_groups = seating_groups;
            new_listed_seat.ticket_sold = !(seating_element.value()["ticket"].is_null());
            result.push_back(new_listed_seat);
        }
        else {
            std::vector<std::string> seating_groups_next(seating_groups);
            seating_groups_next.push_back(seating_element.key());
            collect_listed_seats(seating_element.value(),seating_groups_next,result);
        }
    }
}

std::vector<listed_seat_t> JsonModel::list_seats(json &showing)
{
    std::vector<listed_seat_t> result;
    std::vector<std::string> seating_groups;
    collect_listed_seats(showing["seats"],seating_groups,result);
    return result;
}

result_t JsonModel::replace_ticket
(size_t event_index, size_t showing_index, seat_t target_seat, json new_ticket_value)
{
    std::string ticket_path;
    json seating_group = data["events"][event_index]["showings"][showing_index]["seats"];
    for (auto seating_group_name : target_seat.seating_groups) {
        seating_group = seating_group[seating_group_name];
        ticket_path += ("/"+seating_group_name);
    }

    json seat = seating_group[target_seat.name];

    if (!(seat.contains("ticket"))) {
        std::cerr
        << "JSON MODEL INTERNAL ERROR: "
        << "seat exists, but does not contain ticket object\nseat="
        << seat.dump() << "\n";
        return INTERNAL_ERROR;
    }

    if (!(new_ticket_value.is_null()) && !(seat["ticket"].is_null())) {
        return TICKET_ALREADY_SOLD;
    }

    if (new_ticket_value.is_null() && seat["ticket"].is_null()) {
        return TICKET_NOT_SOLD;
    }

    ticket_path += "/"+ target_seat.name + "/ticket";

    json new_ticket_patch = {{
        {"op", "replace"},
        {"path", ticket_path},
        {"value", new_ticket_value}
    }};

    json &seats = data["events"][event_index]["showings"][showing_index]["seats"];

    try {
        seats = seats.patch(new_ticket_patch);
    } catch(const json::exception& exception) {
        std::cerr
        << "JSON MODEL INTERNAL ERROR: "
        << "json exception (id=" << exception.id << ")\n"
        << exception.what() << "\n";
        return INTERNAL_ERROR;
    }

    return SUCCESS;
}

result_t JsonModel::test_event_index
(size_t event_index)
{
    if (event_index >= data["events"].size())
        return INVALID_EVENT_REQUESTED;
    else
        return SUCCESS;
}

result_t JsonModel::test_showing_index
(size_t event_index, size_t showing_index)
{
    if (showing_index >= data["events"][event_index]["showings"].size())
        return INVALID_SHOWING_REQUESTED;
    else
        return SUCCESS;
}

result_t JsonModel::test_price_name
(size_t event_index, size_t showing_index, std::string price_name)
{
    if (!(data["events"][event_index]["showings"][showing_index]["pricing"].front().contains(price_name)))
        return INVALID_PRICE_REQUESTED;
    else
        return SUCCESS;
}

result_t JsonModel::test_seat
(size_t event_index, size_t showing_index, seat_t seat)
{
    json seating_group = data["events"][event_index]["showings"][showing_index]["seats"];
    for (auto seating_group_name : seat.seating_groups) {
        if (seating_group.contains(seating_group_name)) {
            seating_group = seating_group[seating_group_name];
        } else {
            return INVALID_SEATING_GROUP_REQUESTED;
        }
    }

    if (!(seating_group.contains(seat.name))) {
        return INVALID_SEAT_REQUESTED;
    }

    return SUCCESS;
}

