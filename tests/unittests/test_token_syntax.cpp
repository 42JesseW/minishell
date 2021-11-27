#include <catch.hpp>
#include <unittest.hpp>

class ValidatePipesFixture : public BaseFixture
{
public:
	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
		remove_spaces(&tokens);
	}
};

TEST_CASE_METHOD(ValidatePipesFixture, "No pipes") {
	init_tokens("> word <");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(ValidatePipesFixture, "One pipe") {
	init_tokens("> wo | rd <");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(ValidatePipesFixture, "Multiple pipes") {
	init_tokens("> word | < word | > word");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(ValidatePipesFixture, "Valid pipes") {
	init_tokens("\"word\"|\"word\" word");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("\'word\'|\'word\' word");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$word|$");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$|$word");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$|$");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens(" $ | $ ");
	REQUIRE(validate_pipes(tokens));
	ft_lstclear(&tokens, token_del);
}

TEST_CASE_METHOD(ValidatePipesFixture, "invalid pipes") {
	init_tokens("||");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" | | ");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word > |");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word < |");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word >> |");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word << |");
	REQUIRE(validate_pipes(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
}
