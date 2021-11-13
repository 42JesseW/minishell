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
			THEN("members should have the approriate values") {
				CHECK(redir->file == NULL);
				CHECK(redir->type == REDIR_IN);
			}
			redir_del(redir);
		}
	}

	GIVEN("another t_redir structure"){
		t_redir	*cpy;

		cpy = redir_new_val(REDIR_OUT, "filename");
		REQUIRE(cpy != NULL);
		WHEN("creating a new t_redir structure") {
			t_redir	*redir;

			redir = (t_redir *)redir_new_cpy((void*)cpy);
			REQUIRE(redir != NULL);
			THEN("members should have the appropriate values") {
				CHECK(redir->type == cpy->type);
				CHECK(strcmp(redir->file, cpy->file) == 0);
			}
			redir_del(redir);
		}
		redir_del(cpy);
	}

	GIVEN("input values") {
		t_redir_type	type;
		char			*file;

		type = REDIR_APP;
		file = strdup("filename");
		REQUIRE(file != NULL);
		WHEN("creating a new t_redir structure") {
			t_redir	*redir;

			redir = redir_new_val(type, file);
			REQUIRE(redir != NULL);
			THEN("members should have the appropriate values") {
				CHECK(redir->type == type);
				CHECK(strcmp(redir->file, file) == 0);
			}
			redir_del(redir);
		}
		free(file);
	}
}
