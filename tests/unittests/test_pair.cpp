#include <catch.hpp>

extern "C" {
	#include <minishell.h>
}

SCENARIO("creating pairs") {
	GIVEN("a key and a value") {
		char	key[] = "key";
		char	val[] = "val";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_kv(key, val);
			REQUIRE(pair != NULL);
			THEN("key and val members should have corresponding values") {
				CHECK(strcmp(key, pair->key) == 0);
				CHECK(strcmp(val, pair->val) == 0);
			}
			pair_del(&pair);
		}
	}

	GIVEN("only a key") {
		char	key[] = "key";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_kv(key, NULL);
			REQUIRE(pair != NULL);
			THEN("key and val members should have corresponding values") {
				CHECK(strcmp(key, pair->key) == 0);
				CHECK(pair->val == NULL);
			}
			pair_del(&pair);
		}
	}

	GIVEN("neither key or value") {
		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_def();
			REQUIRE(pair != NULL);
			THEN("key and val members should have corresponding values") {
				CHECK(pair->key == NULL);
				CHECK(pair->val == NULL);
			}
			pair_del(&pair);
		}
	}

	GIVEN("an existing pair") {
		t_pair	*cpy;

		cpy = pair_new_kv("key", "value");
		REQUIRE(cpy != NULL);
		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_cpy(cpy);
			REQUIRE(pair != NULL);
			THEN("key and val members should have corresponding values") {
				CHECK(strcmp(cpy->key, pair->key) == 0);
				CHECK(strcmp(cpy->val, pair->val) == 0);
			}
			pair_del(&pair);
		}
		pair_del(&cpy);
	}

	GIVEN("a full combination of key and value") {
		char	kv[] = "KEY=VAL";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_val(kv);
			REQUIRE(pair != NULL);
			THEN("key and val members should corresponding values") {
				CHECK(strcmp(pair->key, "KEY") == 0);
				CHECK(strcmp(pair->val, "VAL") == 0);
			}
			pair_del(&pair);
		}
	}

	GIVEN("a full combination of more complex key and value") {
		char	kv[] = "TMPDIR=/var/folders/zz/zyxvpxvq6csfxvn_n0008bm80022x2/T/";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_val(kv);
			REQUIRE(pair != NULL);
			THEN("key and val members should corresponding values") {
				CHECK(strcmp(pair->key, "TMPDIR") == 0);
				CHECK(strcmp(pair->val, "/var/folders/zz/zyxvpxvq6csfxvn_n0008bm80022x2/T/") == 0);
			}
			pair_del(&pair);
		}
	}

	GIVEN("a combination of key and empty value") {
		char	kv[] = "KEY=";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_val(kv);
			REQUIRE(pair != NULL);
			THEN("key and val members should corresponding values") {
				CHECK(strcmp(pair->key, "KEY") == 0);
				CHECK(strlen(pair->val) == 0);
			}
			pair_del(&pair);
		}
	}

	GIVEN("a combination of key and no value") {
		char	kv[] = "KEY";

		WHEN("Creating a new pair") {
			t_pair	*pair;

			pair = pair_new_val(kv);
			REQUIRE(pair != NULL);
			THEN("key and val members should corresponding values") {
				CHECK(strcmp(pair->key, "KEY") == 0);
				CHECK(pair->val == NULL);
			}
			pair_del(&pair);
		}
	}
}

SCENARIO("joining pairs") {
	GIVEN("a pair with both key and value") {
		t_pair	*pair;

		pair = pair_new_kv("key", "value");
		REQUIRE(pair->val != NULL);
		REQUIRE(pair->key != NULL);
		WHEN("joining pairs") {
			char	*kv;

			kv = pair_join(pair);
			REQUIRE(kv != NULL);
			THEN("the joined pair should have the format {key=value}") {
				CHECK(strcmp(kv, "key=value") == 0);
			}
			free(kv);
		}
		pair_del(&pair);
	}

	GIVEN("a pair with only a key") {
		t_pair	*pair;

		pair = pair_new_kv("key", NULL);
		REQUIRE(pair->val == NULL);
		REQUIRE(pair->key != NULL);
		WHEN("joining pairs") {
			char	*kv;

			kv = pair_join(pair);
			REQUIRE(kv != NULL);
			THEN("the joined pair should have the format {key}") {
				CHECK(strcmp(kv, "key") == 0);
			}
			free(kv);
		}
		pair_del(&pair);
	}

	GIVEN("a pair with a key and an empty value") {
		t_pair	*pair;

		pair = pair_new_kv("key", "");
		REQUIRE(pair->val != NULL);
		REQUIRE(pair->key != NULL);
		WHEN("joining pairs") {
			char	*kv;

			kv = pair_join(pair);
			REQUIRE(kv != NULL);
			THEN("the joined pair should have the format {key=value}") {
				CHECK(strcmp(kv, "key=") == 0);
			}
			free(kv);
		}
		pair_del(&pair);
	}
}
