#include <catch.hpp>

extern "C" {
	#include <parser.h>
}

SCENARIO("creating new node structures") {
    GIVEN("no input information") {
        WHEN("creating a new node") {
            t_node  *node;

            node = node_new_def();
            THEN("all members should have the appropriate values") {
                CHECK(node->next == NULL);
                CHECK(node->cmd == NULL);
            }
        }
    }

    GIVEN("input information for the node") {

    }

    GIVEN("a copy of a node") {

    }
}

SCENARIO("creating lists of node structures") {

}
