#ifndef JSON_MODEL_H
#define JSON_MODEL_H

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>

#include "../../lib/json.h"
using json = nlohmann::json;

#include "model.h"

enum helper_result_t {
    JSON_MODEL_SUCCESS = 100,
    JSON_MODEL_OPEN_FILE_FAILED = 201,
    JSON_MODEL_READING_JSON_FILE_FAILED = 202
};

class JsonModel : public Model {
    private:
        std::string jsonfile;
        json data;
        helper_result_t read_json_file
            (std::string path, json &result);
        helper_result_t write_json_file
            (std::string path, json &json_object);
        helper_result_t parse_datetime_string
            (std::string datetime_str, asap::datetime &result);
        void count_tickets
            (json &seats, size_t &num_tickets_sold, size_t &num_tickets_available);
        listed_showing_compact_t generate_compact_showing_listing
            (json &showing);
        pricing_t get_pricing
            (json &showing);
        void collect_seats
            (json &seating_subset, std::vector<std::string> seating_groups, std::vector<seat_t> &result);
        seating_t get_seating
            (json &showing);
        void collect_tickets
            (json &seats_subset, std::vector<std::string> seating_groups, std::string pricing_name, std::vector<ticket_t> &result);
        std::vector<ticket_t> get_tickets
            (json &showing);
        void collect_listed_seats
            (json &seating_subset, std::vector<std::string> seating_groups, std::vector<listed_seat_t> &result);
        std::vector<listed_seat_t> list_seats
            (json &showing);
        result_t replace_ticket
            (size_t event_index, size_t showing_index, seat_t seat, json new_ticket_value);
        result_t test_event_index
            (size_t event_index);
        result_t test_showing_index
            (size_t event_index, size_t showing_index);
        result_t test_price_name
            (size_t event_index, size_t showing_index, std::string price_name);
        result_t test_seat
            (size_t event_index, size_t showing_index, seat_t seat);
    public:
        JsonModel();
        ~JsonModel();
        // void write(void);
        // std::string to_string(void);
        // std::list<listed_event_t> list_events(void);
        // // size_t find_event(std::string event_name);
        // std::list<listed_showing_compact_t> list_event(size_t event_index);
        // // size_t find_showing(size_t event_index, std::string showing_name);
        // listed_showing_extended_t list_showing(size_t event_index, size_t showing_index);
        // // result_t get_price(size_t event_index, size_t showing_index, std::string price_name, price_t &result);
        // result_t create_ticket(size_t event_index, size_t showing_index, std::string holder_name, std::string price_name, seat_t seat);
        // result_t delete_ticket(size_t event_index, size_t showing_index, seat_t seat);
        result_t load
            (void);
        result_t write
            (void);
        // result_t dump
        //     (std::string &result);
        std::string dump
            (void);
        result_t list_events
            (std::list<listed_event_t> &result);
        result_t list_event
            (size_t event_index, std::list<listed_showing_compact_t> &result);
        result_t list_showing
            (size_t event_index, size_t showing_index, listed_showing_extended_t &result);
        result_t create_ticket
            (size_t event_index, size_t showing_index, std::string holder_name, std::string price_name, seat_t seat);
        result_t delete_ticket
            (size_t event_index, size_t showing_index, seat_t seat);
};


#endif
