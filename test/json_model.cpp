
#include "../lib/doctest.h"

#include "../lib/json.h"
using json = nlohmann::json;

#include "../src/include/json_model.h"
#include "../src/json_model.cpp"
#include "../src/json_model_helper.cpp"

void print_seperators(void)
{
    // if (true)
    if (false)
    std::cout
    << "\n!!!!!!!!!\n########################\n#########################\n";
}

void print_json_compare(json expected, json test);

json get_testdata()
{
    json test_pricing1 = {
        "test_pricing1", {
            {"test_price1", 1.25},
            {"test_price2", 2.5}
        }
    };
    json test_pricing2 = {
        "test_pricing2", {
            {"test_price", 2.9}
        }
    };

    json test_seating = {
        {"test_seat1", {
            {"ticket", nullptr}
        }},
        {"test_seat2", {
            {"ticket", nullptr}
        }},
        {"test_seating_group1", {
            {"test_seat3", {
                {"ticket", nullptr}
            }},
            {"test_seat4", {
                {"ticket", nullptr}
            }}
        }}
    };

    json test_seats = {
        {"test_seat1", {
            {"ticket", {
                {"holder_name", "test_guest1"},
                {"price", "test_price1"}
            }}
        }},
        {"test_seat2", {
            {"ticket", nullptr}
        }},
        {"test_seating_group1", {
            {"test_seat3", {
                {"ticket", {
                    {"holder_name", "test_guest2"},
                    {"price", "test_price2"}
                }}
            }},
            {"test_seat4", {
                {"ticket", nullptr}
            }}
        }}
    };

    json test_showing = {
        {"name", "test_showing1"},
        {"datetime", "13.04.2022 13:37:00"},
        {"pricing", {test_pricing1}},
        {"venue", "test_venue1"},
        {"room", "test_room1"},
        {"seats", test_seats}
    };

    json test_event = {
        {"name", "test_event1"},
        {"showings",
            json::array({test_showing})
        }
    };

    json testdata = {
        {"events",json::array({test_event})},
        {"pricing", {test_pricing1,test_pricing2}},
        {"venues", {
            {"test_venue1", {
                {"test_room1", {
                    {"seating", test_seating}
                }}
            }}
        }}
    };

    return testdata;
}

TEST_CASE("constructor") {
    json expected_data = get_testdata();
    result_t expected_result = SUCCESS;

    JsonModel testmodel;
    result_t test_result = testmodel.load();


    // test_create_compare_results(
    //     INVALID_SEATING_GROUP_REQUESTED,
    //     test_result,
    //     expected_data,
    //     json::parse(testmodel.dump())
    // );

    std::string expected_str = expected_data.dump();
    std::string test_str = testmodel.dump();

    // std::cout
    // << "\nexpected:\n"
    // << expected_str
    // << "\n\n";

    // std::cout
    // << "\ntest:\n"
    // << test_str
    // << "\n\n";

    CHECK(expected_result == test_result);
    CHECK(expected_str == test_str);
}

TEST_CASE("list events") {
    JsonModel testmodel;
    testmodel.load();

    std::list<listed_event_t> expected_listed_events;
    listed_event_t expected_listed_event;
    expected_listed_event.name = "test_event1";
    expected_listed_event.num_showings = 1;
    expected_listed_events.push_back(expected_listed_event);
    result_t expected_result = SUCCESS;

    // std::cout
    // << "\nexpected_listed_events: size\n"
    // << expected_listed_events.size()
    // << "\nexpected_listed_events: front\n"
    // << expected_listed_events.front().name
    // << "\n"
    // << expected_listed_events.front().num_showings
    // << "\n\n";


    std::list<listed_event_t> test_listed_events;
    result_t test_result = testmodel.list_events(test_listed_events);

    // test_create_compare_results(
    //     INVALID_SEATING_GROUP_REQUESTED,
    //     test_result,
    //     expected_data,
    //     json::parse(testmodel.dump())
    // );

    // std::cout
    // << "\ntest_listed_events: size\n"
    // << test_listed_events.size()
    // << "\ntest_listed_events: front\n"
    // << test_listed_events.front().name
    // << "\n"
    // << test_listed_events.front().num_showings
    // << "\n\n";

    CHECK(expected_result == test_result);
    CHECK(expected_listed_events == test_listed_events);


}

TEST_CASE("list event") {
    JsonModel testmodel;
    testmodel.load();

    std::list<listed_showing_compact_t> expected_listed_event;
    listed_showing_compact_t expected_listed_showing;
    expected_listed_showing.name = "test_showing1";
    // asap library example documentation:
    //   setting locale helps with local time format
    setlocale(LC_ALL, "");
    asap::datetime expected_datetime("13.04.2022 13:37:00");
    // expected_datetime.clear();
    // expected_datetime.year = 2022;
    // expected_datetime.month = 4;
    // expected_datetime.day = 13;
    // expected_datetime.hour = 13;
    // expected_datetime.minute = 37;
    // expected_datetime.second = 0;
    // expected_datetime.tzd = 60;
    expected_listed_showing.datetime = expected_datetime;
    expected_listed_showing.num_tickets_sold = 2;
    expected_listed_showing.num_tickets_available = 2;
    expected_listed_event.push_back(expected_listed_showing);
    result_t expected_result = SUCCESS;

    // std::cout
    // << "\nexpected_listed_event: size\n"
    // << expected_listed_event.size()
    // << "\nexpected_listed_event: front\n"
    // << expected_listed_event.front().name
    // << "\n"
    // << expected_listed_event.front().datetime.str()
    // << "\n"
    // << expected_listed_event.front().num_tickets_sold
    // << "\n"
    // << expected_listed_event.front().num_tickets_available
    // << "\n\n";


    std::list<listed_showing_compact_t> test_listed_event;
    result_t test_result = testmodel.list_event(0,test_listed_event);

    // std::cout
    // << "\ntest_listed_events: size\n"
    // << test_listed_events.size()
    // << "\ntest_listed_events: front\n"
    // << test_listed_events.front().name
    // << "\n"
    // << test_listed_events.front().datetime.str()
    // << "\n"
    // << test_listed_events.front().num_tickets_sold
    // << "\n"
    // << test_listed_events.front().num_tickets_available
    // << "\n\n";

    CHECK(expected_result == test_result);
    CHECK(expected_listed_event == test_listed_event);
}

void print_listed_showing(listed_showing_extended_t lse)
{
    std::cout
    << "\n-------------------"
    << "\n event_name:\n"
    << "    " << lse.event_name
    << "\n-------------------"
    << "\n summary.name: \n"
    << "    " << lse.summary.name
    << "\n summary.datetime: \n"
    << "    " << lse.summary.datetime
    << "\n summary.num_tickets_available: \n"
    << "    " << lse.summary.num_tickets_available
    << "\n summary.num_tickets_sold: \n"
    << "    " << lse.summary.num_tickets_sold
    << "\n";

    std::cout
    << "\n pricing.name: \n"
    << "    " << lse.pricing.name
    << "\n";
    for (auto& price : lse.pricing.prices) {
        std::cout
        << "      "
        << price.first
        << ","
        << price.second
        << "\n";
    }

    std::cout
    << "\n seating.venue_name: \n"
    << "    " << lse.seating.venue_name
    << "\n seating.room_name: \n"
    << "    " << lse.seating.room_name
    << "\n";
    for (auto& seat : lse.seating.seats) {
        std::cout
        << "      "
        << seat.name
        << ":"
        << "\n"
        << "            ";
        for (auto& seating_group : seat.seating_groups) {
            std::cout
            << seating_group
            << ",";
        }
        std::cout
        << "\n";
    }

    std::cout
    << "\n tickets: \n";
    for (auto& ticket : lse.tickets) {
        std::cout
        << "       ticket holder:"
        << ticket.holder_name
        << " price:"
        << ticket.price.first
        << ","
        << ticket.price.second
        << "  seat:"
        << ticket.seat.name
        << " [";
        for (auto& seating_group : ticket.seat.seating_groups) {
            std::cout
            << seating_group
            << ",";
        }
        std::cout
        << "]\n";
    }

}

TEST_CASE("list showing") {
    JsonModel testmodel;
    testmodel.load();

    listed_showing_extended_t expected_listed_showing;
    expected_listed_showing.event_name = "test_event1";
    
    listed_showing_compact_t expected_summary;
    expected_summary.name = "test_showing1";
    // asap library example documentation:
    //   setting locale helps with local time format
    setlocale(LC_ALL, "");
    asap::datetime expected_datetime("13.04.2022 13:37:00");
    expected_summary.datetime = expected_datetime;
    expected_summary.num_tickets_sold = 2;
    expected_summary.num_tickets_available = 2;
    expected_listed_showing.summary = expected_summary;

    pricing_t expected_pricing;
    expected_pricing.name = "test_pricing1";
    std::pair<std::string,float> expected_price1 = std::pair<std::string,float>("test_price1",1.25);
    std::pair<std::string,float> expected_price2 = std::pair<std::string,float>("test_price2",2.5);
    expected_pricing.prices.insert(expected_price1);
    expected_pricing.prices.insert(expected_price2);
    expected_listed_showing.pricing = expected_pricing;

    seating_t expected_seating;
    expected_seating.venue_name = "test_venue1";
    expected_seating.room_name = "test_room1";
    seat_t expected_seat1;
    expected_seat1.name = "test_seat1";
    seat_t expected_seat2;
    expected_seat2.name = "test_seat2";
    seat_t expected_seat3;
    expected_seat3.name = "test_seat3";
    expected_seat3.seating_groups.push_back("test_seating_group1");
    seat_t expected_seat4;
    expected_seat4.seating_groups.push_back("test_seating_group1");
    expected_seat4.name = "test_seat4";
    expected_seating.seats.push_back(expected_seat1);
    expected_seating.seats.push_back(expected_seat2);
    expected_seating.seats.push_back(expected_seat3);
    expected_seating.seats.push_back(expected_seat4);
    expected_listed_showing.seating = expected_seating;

    std::vector<ticket_t> expected_tickets;
    ticket_t expected_ticket1;
    expected_ticket1.holder_name = "test_guest1";
    expected_ticket1.seat = expected_seat1;
    expected_ticket1.price = expected_price1;
    ticket_t expected_ticket2;
    expected_ticket2.holder_name = "test_guest2";
    expected_ticket2.seat = expected_seat3;
    expected_ticket2.price = expected_price2;
    expected_tickets.push_back(expected_ticket1);
    expected_tickets.push_back(expected_ticket2);
    expected_listed_showing.tickets = expected_tickets;

    std::vector<listed_seat_t> expected_listed_seats;
    listed_seat_t expected_listed_seat1;
    expected_listed_seat1.seat = expected_seat1;
    expected_listed_seat1.ticket_sold = true;
    listed_seat_t expected_listed_seat2;
    expected_listed_seat2.seat = expected_seat2;
    expected_listed_seat2.ticket_sold = false;
    listed_seat_t expected_listed_seat3;
    expected_listed_seat3.seat = expected_seat3;
    expected_listed_seat3.ticket_sold = true;
    listed_seat_t expected_listed_seat4;
    expected_listed_seat4.seat = expected_seat4;
    expected_listed_seat4.ticket_sold = false;
    expected_listed_seats.push_back(expected_listed_seat1);
    expected_listed_seats.push_back(expected_listed_seat2);
    expected_listed_seats.push_back(expected_listed_seat3);
    expected_listed_seats.push_back(expected_listed_seat4);
    expected_listed_showing.listed_seats = expected_listed_seats;
    result_t expected_result = SUCCESS;

    listed_showing_extended_t test_listed_showing;

    result_t test_result = testmodel.list_showing(0,0,test_listed_showing);

    // std::cout << "\n expected_listed_showing: \n";
    // print_listed_showing(expected_listed_showing);
    // std::cout << "\n test_listed_showing: \n";
    // print_listed_showing(test_listed_showing);

    CHECK(expected_result == test_result);
    CHECK(expected_listed_showing == test_listed_showing);
}

void print_json_compare(json expected, json test)
{
    std::cout
    << "\nEXPECTED ------------- expected: \n"
    << expected.dump()
    << "\n ###########################";

    std::cout
    << "\nTEST ------------- test: \n"
    << test.dump()
    << "\n ###########################\n";

    std::cout
    << "\nDIFF ---- test.patch --> expected    --------- diff: \n"
    << json::diff(test,expected).dump()
    << "\n ###########################";

    std::cout
    << "\nDIFF ---- expected.patch --> test    --------- diff: \n"
    << json::diff(expected,test).dump()
    << "\n ###########################\n";
}

void test_create_compare_results (
    result_t expected_result, result_t test_result,
    json expected_json, json test_json
) {
    if (expected_json != test_json)
        print_json_compare(expected_json,test_json);
    CHECK(expected_result == test_result);
    CHECK(expected_json == test_json);
    //CHECK(expected_json.dump() == test_json.dump());
}

TEST_CASE("create ticket") {
    JsonModel testmodel;
    testmodel.load();

    json expected_data = get_testdata();

    ticket_t test_ticket;
    test_ticket.seat.seating_groups.push_back("test_seating_group1");
    test_ticket.seat.name = "test_seat4";
    test_ticket.price.first = "test_price2";
    test_ticket.price.second = 2.5;
    test_ticket.holder_name = "test_guest_created1";


    SUBCASE("success") {
        print_seperators();
        expected_data["events"][0]["showings"][0]["seats"]["test_seating_group1"]["test_seat4"]["ticket"] = {
            {"holder_name", "test_guest_created1"},
            {"price", "test_price2"}
        };
        result_t test_result = testmodel.create_ticket(0,0, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            SUCCESS,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid event") {
        print_seperators();
        result_t test_result = testmodel.create_ticket(1,0, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            INVALID_EVENT_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid showing") {
        print_seperators();
        result_t test_result = testmodel.create_ticket(0,1, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            INVALID_SHOWING_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid seating group") {
        print_seperators();
        // test_ticket.seat.seating_groups.push_back("invalid_test_seating_group");
        test_ticket.seat.seating_groups[0] = "invalid_test_seating_group";
        result_t test_result = testmodel.create_ticket(0,0, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            INVALID_SEATING_GROUP_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid seat") {
        print_seperators();
        test_ticket.seat.name = "invalid_test_seat";
        result_t test_result = testmodel.create_ticket(0,0, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            INVALID_SEAT_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("ticket already sold") {
        print_seperators();
        test_ticket.seat.seating_groups[0] = "test_seating_group1";
        test_ticket.seat.name = "test_seat3";
        test_ticket.price.first = "test_price1";
        test_ticket.price.second = 1.25;
        test_ticket.holder_name = "test_guest_already_created";
        result_t test_result = testmodel.create_ticket(0,0, test_ticket.holder_name, test_ticket.price.first, test_ticket.seat);
        test_create_compare_results(
            TICKET_ALREADY_SOLD,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }
}

TEST_CASE("delete ticket") {
    JsonModel testmodel;
    testmodel.load();

    json expected_data = get_testdata();

    seat_t test_seat;
    test_seat.seating_groups.push_back("test_seating_group1");
    test_seat.name = "test_seat3";

    SUBCASE("success") {
        print_seperators();
        expected_data["events"][0]["showings"][0]["seats"]["test_seating_group1"]["test_seat3"]["ticket"] = nullptr;
        result_t test_result = testmodel.delete_ticket(0,0, test_seat);
        test_create_compare_results(
            SUCCESS,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid event") {
        print_seperators();
        result_t test_result = testmodel.delete_ticket(1,0, test_seat);
        test_create_compare_results(
            INVALID_EVENT_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid showing") {
        print_seperators();
        result_t test_result = testmodel.delete_ticket(0,1, test_seat);
        test_create_compare_results(
            INVALID_SHOWING_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid seating group") {
        print_seperators();
        test_seat.seating_groups[0] = "invalid_test_seating_group";
        result_t test_result = testmodel.delete_ticket(0,0, test_seat);
        test_create_compare_results(
            INVALID_SEATING_GROUP_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("invalid seat") {
        print_seperators();
        test_seat.name = "invalid_test_seat";
        result_t test_result = testmodel.delete_ticket(0,0, test_seat);
        test_create_compare_results(
            INVALID_SEAT_REQUESTED,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }

    SUBCASE("ticket not sold") {
        print_seperators();
        test_seat.seating_groups[0] = "test_seating_group1";
        test_seat.name = "test_seat4";
        result_t test_result = testmodel.delete_ticket(0,0, test_seat);
        test_create_compare_results(
            TICKET_NOT_SOLD,
            test_result,
            expected_data,
            json::parse(testmodel.dump())
        );
    }
}