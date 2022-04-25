#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <list>
#include <tuple>
#include <ctime>
#include <vector>
#include <map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "../../lib/asap/asap.h"
#pragma GCC diagnostic pop

typedef struct listed_event {
    std::string name;
    size_t num_showings;
} listed_event_t;

typedef struct listed_showing_compact{
    std::string name;
    asap::datetime datetime;
    size_t num_tickets_sold;
    size_t num_tickets_available;
    listed_showing_compact() {
        name = "";
        num_tickets_sold = 0;
        num_tickets_available = 0;
        datetime = asap::datetime(0);
    }
} listed_showing_compact_t;

typedef struct {
    std::string name;
    std::vector<std::string> seating_groups;
} seat_t;

typedef struct {
    std::string venue_name;
    std::string room_name;
    std::vector<seat_t> seats;
} seating_t;

typedef std::pair<std::string,float> price_t;
typedef std::map<std::string,float> prices_t;

typedef struct {
    std::string name;
    prices_t prices;
} pricing_t;

typedef struct {
    std::string holder_name;
    seat_t seat;
    price_t price;
} ticket_t;

typedef struct {
    seat_t seat;
    bool ticket_sold;
} listed_seat_t;

typedef struct listed_showing_extended {
    std::string event_name;
    listed_showing_compact_t summary;
    pricing_t pricing;
    seating_t seating;
    std::vector<ticket_t> tickets;
    std::vector<listed_seat_t> listed_seats;
} listed_showing_extended_t;



enum result_t {
    SUCCESS = 100,
    INVALID_EVENT_REQUESTED = 201,
    INVALID_SHOWING_REQUESTED = 202,
    INVALID_SEATING_GROUP_REQUESTED = 203,
    INVALID_SEAT_REQUESTED = 204,
    INVALID_PRICE_REQUESTED = 205,
    TICKET_ALREADY_SOLD = 301,
    TICKET_NOT_SOLD = 302,
    INTERNAL_ERROR = 500,
    LOAD_ERROR_FILE_NOT_FOUND = 501,
    LOAD_ERROR_INVALID_CONFIG = 502,
    LOAD_ERROR_INVALID_DATA = 503,
    WRITE_ERROR_FILE_NOT_FOUND = 511,
    UNKNOWN = 600
};

class Model {
    public:
        Model() {};
        ~Model() {};
        virtual result_t write
            (void) = 0;
        virtual result_t load
            (void) = 0;
        // virtual result_t dump
        //     (std::string &result) = 0;
        virtual std::string dump
            (void) = 0;
        virtual result_t list_events
            (std::list<listed_event_t> &result) = 0;
        virtual result_t list_event
            (size_t event_index, std::list<listed_showing_compact_t> &result) = 0;
        virtual result_t list_showing
            (size_t event_index, size_t showing_index, listed_showing_extended_t &result) = 0;
        virtual result_t create_ticket
            (size_t event_index, size_t showing_index, std::string holder_name, std::string price_name, seat_t seat) = 0;
        virtual result_t delete_ticket
            (size_t event_index, size_t showing_index, seat_t seat) = 0;
        // virtual result_t find_event
        //     (std::string event_name, size_t &result) = 0;
        // virtual result_t find_showing
        //     (size_t event_index, std::string showing_name, size_t &result) = 0;
};



inline bool operator == (const listed_event_t& lhs, const listed_event_t& rhs) {
    return 
        std::tie(lhs.name, lhs.num_showings) ==
        std::tie(rhs.name, rhs.num_showings);
}

inline bool operator == (const listed_showing_compact_t& lhs, const listed_showing_compact_t& rhs) {
    return (
        (std::tie(lhs.name, lhs.num_tickets_sold, lhs.num_tickets_available) ==
        std::tie(rhs.name, rhs.num_tickets_sold, rhs.num_tickets_available))
        &&
        (lhs.datetime.timestamp() == rhs.datetime.timestamp())
    );
}

inline bool operator == (const seat_t& lhs, const seat_t& rhs) {
    return (
        (std::tie(lhs.name, lhs.seating_groups) ==
        std::tie(rhs.name, rhs.seating_groups))
    );
}

inline bool operator == (const seating_t& lhs, const seating_t& rhs) {
    return (
        (std::tie(lhs.venue_name, lhs.room_name, lhs.seats) ==
        std::tie(rhs.venue_name, rhs.room_name, rhs.seats))
    );
}

inline bool operator == (const pricing_t& lhs, const pricing_t& rhs) {
    return (
        (std::tie(lhs.name, lhs.prices) ==
        std::tie(rhs.name, rhs.prices))
    );
}

inline bool operator == (const ticket_t& lhs, const ticket_t& rhs) {
    return (
        (std::tie(lhs.holder_name, lhs.seat, lhs.price) ==
        std::tie(rhs.holder_name, rhs.seat, rhs.price))
    );
}

inline bool operator == (const listed_seat_t& lhs, const listed_seat_t& rhs) {
    return (
        (std::tie(lhs.seat, lhs.ticket_sold) ==
        std::tie(rhs.seat, rhs.ticket_sold))
    );
}

inline bool operator == (const listed_showing_extended_t& lhs, const listed_showing_extended_t& rhs) {
    return (
        (std::tie(lhs.event_name, lhs.summary, lhs.pricing, lhs.seating, lhs.tickets, lhs.listed_seats) ==
        std::tie(rhs.event_name, rhs.summary, rhs.pricing, rhs.seating, rhs.tickets, rhs.listed_seats))
    );
}


#endif