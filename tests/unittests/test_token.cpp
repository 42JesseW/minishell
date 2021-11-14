#include <catch.hpp>

extern "C" {
	#include <parser.h>
}

SCENARIO("creating tokens") {
	GIVEN("no additional information") {
		WHEN("creating a token") {
			t_token	*token;

			token = token_new_def();
			REQUIRE(token != NULL);
			THEN("members should have the appropriate values") {
				CHECK(token->type == TOK_DOLLAR);
				CHECK(token->token == NULL);
			}
			token_del(token);
		}
	}

	GIVEN("information for the token") {
		t_token_type	type;
		char			token_string[] = "Makefile";

		type = TOK_WORD;
		WHEN("creating the token") {
			t_token	*token;

			token = token_new_val(TOK_WORD, token_string);
			REQUIRE(token != NULL);
			THEN("members should have the appropriate values") {
				CHECK(token->type == type);
				CHECK(strcmp(token->token, token_string) == 0);
			}
			token_del(token);
		}
	}

	GIVEN("another token") {
		t_token			*cpy;
		t_token_type	type;
		char			token_string[] = "Makefile";

		type = TOK_WORD;
		cpy = token_new_val(type, token_string);
		REQUIRE(cpy != NULL);
		WHEN("creating the token") {
			t_token	*token;

			token = token_new_cpy(cpy);
			REQUIRE(token != NULL);
			THEN("members should have the appropriate values") {
				CHECK(token->type == cpy->type);
				CHECK(strcmp(token->token, cpy->token) == 0);
			}
		}
	}

	GIVEN("a lexer structure containing multiple words with index at 0") {
		char	input_string[] = "cat with food";
		t_lexer lexer;

		lexer = (t_lexer){.input_string = input_string, .string_len = strlen(input_string), .idx = 0};
		WHEN("creating the token") {
			t_token	*token;

			token = token_new_word(&lexer);
			REQUIRE(token != NULL);
			THEN("member should have the appropriate values") {
				CHECK(token->type == TOK_WORD);
				CHECK(strncmp(token->token, input_string, (strchr(input_string, ' ') - input_string)) == 0);
			}
		}
	}

	GIVEN("a lexer structure containing multiple words with index at start of second word") {
		char	input_string[] = "cat with food";
		t_lexer lexer;

		lexer = (t_lexer){.input_string = input_string, .string_len = strlen(input_string), .idx = 4};
		WHEN("creating the token") {
			t_token	*token;

			token = token_new_word(&lexer);
			REQUIRE(token != NULL);
			THEN("member should have the appropriate values") {
				CHECK(token->type == TOK_WORD);
				char	*p = input_string + (strchr(input_string, ' ') - input_string);
				CHECK(strncmp(token->token, p, (strchr(p, ' ') - p)) == 0);
			}
		}
	}
}

TEST_CASE("tokenize empty string") {
	t_list	*tokens;
	char	input_string[] = "";

	tokens = tokenize(input_string);
	REQUIRE(tokens == NULL);
}

TEST_CASE("tokenize string with only words and spaces") {
	t_list	*tokens;
	t_token	*token;
	char	**split;
	int		len;
	int		idx = 0;
	int		split_idx = 0;
	char	input_string[] = "my first words are those of great significance";

	tokens = tokenize(input_string);
	REQUIRE(tokens != NULL);
	split = ft_strsplit(input_string, ' ');
	REQUIRE(split != NULL);
	len = ft_strarrlen(split);
	REQUIRE(ft_lstsize(tokens) == (len * 2 - 1));
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token*)node->content;
		if ((idx % 2) == 0)
		{
			CHECK(token->type == TOK_WORD);
			CHECK(strcmp(token->token, split[split_idx]) == 0);
			split_idx++;
		}
		else
		{
			CHECK(token->type == TOK_SPACE);
			CHECK(strcmp(token->token, " ") == 0);
		}
	}
	ft_strarrfree(&split);
	ft_lstclear(&tokens, token_del);
}

TEST_CASE("tokenize string with only tokens") {

}

TEST_CASE("tokenize string with everything in it") {

}
