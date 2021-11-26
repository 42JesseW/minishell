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

class ParseBeforeGroupFixture : public BaseFixture
{
protected:
	t_shell	*shell;
	char	*_input_string;

	typedef struct s_test
	{
		char	*input_string;
		char	*expected_tokens;
		char	**input_tokens;
		char	**check_tokens;
	}	t_test;

private:
	char	**get_input_tokens()
	{
		t_list	*node;
		t_token	*token;
		int		idx;
		char	**input_tokens;

		idx = 0;
		node = tokens;
		input_tokens = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
		REQUIRE(input_tokens != NULL);
		while (node)
		{
			token = (t_token *)node->content;
			input_tokens[idx] = strdup(token->token);
			REQUIRE(input_tokens[idx] != NULL);
			node = node->next;
			idx++;
		}
		input_tokens[idx] = NULL;
		return (input_tokens);
	}

public:
	ParseBeforeGroupFixture() : _input_string(NULL)
	{
		shell = shell_init(envp, &_input_string);
		REQUIRE(shell != NULL);
	}

	void	init_test(t_test *test)
	{
		init_tokens(test->input_string);
		REQUIRE(tokens != NULL);
		test->input_tokens = get_input_tokens();
		test->check_tokens = ft_strsplit(test->expected_tokens, ' ');
		REQUIRE(test->check_tokens != NULL);
	}

	void	do_test(t_test *test)
	{
		for (int idx = 0; test->check_tokens[idx]; idx++)
			CHECK(strncmp(test->check_tokens[idx], test->input_tokens[idx], strlen(test->check_tokens[idx])) == 0);
	}

	void	rm_test(t_test *test)
	{
		ft_lstclear(&tokens, token_del);
		ft_strarrfree(&test->input_tokens);
		ft_strarrfree(&test->check_tokens);
		test->input_string = NULL;
		test->expected_tokens = NULL;
	}

	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
		remove_spaces(&tokens);
		REQUIRE(validate_pipes(tokens));
		REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
		normalize(&tokens);
	}
};

TEST_CASE_METHOD(ParseBeforeGroupFixture, "Valid combinations of tokens") {
	t_test	tests[] = {
		(t_test){"cat < Makefile", "cat < Makefile", NULL, NULL},
		(t_test){"< Makefile", "< Makefile", NULL, NULL},
		(t_test){"< Makefile cat", "cat < Makefile", NULL, NULL},
		(t_test){"< Makefile cat -e", "cat -e < Makefile", NULL, NULL},
		(t_test){"cat < Makefile -e -b", "cat -e -b < Makefile", NULL, NULL},
		(t_test){"cat < Makefile -e -b", "cat -e -b < Makefile", NULL, NULL},
		(t_test){"cat < Makefile -e > OUT -b -s", "cat -e -b -s < Makefile > OUT", NULL, NULL},
		(t_test){"> OUT1 < IN1 cat -e > OUT2 -b -s < IN2", "cat -e -b -s > OUT1 < IN1 > OUT2 < IN2", NULL, NULL},
		(t_test){NULL, NULL, NULL, NULL}
	};

	for (int idx = 0; tests[idx].input_string; idx++)
	{
		init_test(&tests[idx]);
		do_test(&tests[idx]);
		rm_test(&tests[idx]);
	}
}
