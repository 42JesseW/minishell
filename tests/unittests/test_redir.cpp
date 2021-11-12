#include <catch.hpp>

extern "C" {
	#include <parser.h>
}

SCENARIO("creating new t_redir structures") {
	GIVEN("no input data") {
		WHEN("creating a new t_redir structure") {
			t_redir	*redir;

			redir = redir_new_def();
			REQUIRE(redir != NULL);
			THEN("members") {

			}
		}
	}

	GIVEN("another t_redir structure"){

	}

	GIVEN("input values") {

	}
}

SCENARIO("creating lists of t_redir structures") {

}
