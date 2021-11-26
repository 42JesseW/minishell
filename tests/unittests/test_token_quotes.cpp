#include <catch.hpp>
#include <unittest.hpp>

class ResolveQuotesFixture : public BaseFixture
{
protected:
	t_shell	*shell;
	char	*_input_string;

public:
	ResolveQuotesFixture() : _input_string(NULL)
	{
		shell = shell_init(envp, &_input_string);
		REQUIRE(shell != NULL);
	}

	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
		remove_spaces(&tokens);
		REQUIRE(validate_pipes(tokens));
		REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
	}
};

TEST_CASE_METHOD(ResolveQuotesFixture, "No quotes") {
	init_tokens(" word | word | word > word > word < word < ");
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveQuotesFixture, "one double quoted string") {
	init_tokens("\"word\"");
	REQUIRE(ft_lstsize(tokens) == 3);
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(tokens) == 1);
	REQUIRE(strncmp(((t_token *)tokens->content)->token, "word", 4) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveQuotesFixture, "One single quoted string") {
	init_tokens("\'word\'");
	REQUIRE(ft_lstsize(tokens) == 3);
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(tokens) == 1);
	REQUIRE(strncmp(((t_token *)tokens->content)->token, "word", 4) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveQuotesFixture, "Multiple double quoted strings") {
	const char	*input_strings[] = {
			"\"word\" | word | word",
			"word | \"word\" | word",
			"word | word | \"word\"",
			"\"word\" | \"word\" | word",
			"word | \"word\" | \"word\"",
			"\"word\" | word | \"word\"",
			"\"word\" | \"word\" | \"word\"",
			NULL
	};
	int		word_amount = 3;
	int		pipe_amount = 2;
	int		quote_amounts[] = {2, 2, 2, 4, 4, 4, 6};
	for (int idx = 0; input_strings[idx]; idx++)
	{
		REQUIRE(input_strings[idx] != NULL);
		init_tokens(input_strings[idx]);
		REQUIRE(ft_lstsize(tokens) == (word_amount + pipe_amount + quote_amounts[idx]));
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
		REQUIRE(ft_lstsize(tokens) == (word_amount + pipe_amount));
		for (t_list *node = tokens; node != NULL; node = node->next)
		{
			if (((t_token *)node->content)->type == TOK_WORD)
				CHECK(strncmp(((t_token *)node->content)->token, "word", 4) == 0);
		}
		ft_lstclear(&tokens, token_del);
	}
}

TEST_CASE_METHOD(ResolveQuotesFixture, "Multiple single quoted strings") {
	const char	*input_strings[] = {
			"\'word\' | word | word",
			"word | \'word\' | word",
			"word | word | \'word\'",
			"\'word\' | \'word\' | word",
			"word | \'word\' | \'word\'",
			"\'word\' | word | \'word\'",
			"\'word\' | \'word\' | \'word\'",
			NULL
	};
	int		word_amount = 3;
	int		pipe_amount = 2;
	int		quote_amounts[] = {2, 2, 2, 4, 4, 4, 6};
	for (int idx = 0; input_strings[idx]; idx++)
	{
		REQUIRE(input_strings[idx] != NULL);
		init_tokens(input_strings[idx]);
		REQUIRE(ft_lstsize(tokens) == (word_amount + pipe_amount + quote_amounts[idx]));
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
		REQUIRE(ft_lstsize(tokens) == (word_amount + pipe_amount));
		for (t_list *node = tokens; node != NULL; node = node->next)
		{
			if (((t_token *)node->content)->type == TOK_WORD)
				CHECK(strncmp(((t_token *)node->content)->token, "word", 4) == 0);
		}
		ft_lstclear(&tokens, token_del);
	}
}

TEST_CASE_METHOD(ResolveQuotesFixture, "Resolving quoted environment variables") {
	const char	*env_shlvl;
	const char	*env_pwd;
	int			word_count = 0;
	int			redir_count = 0;
	int			pipe_count = 0;

	env_shlvl = environ_get(shell->environ, "SHLVL");
	env_pwd = environ_get(shell->environ, "PWD");
	init_tokens("cat -e < Makefile > OUT | \"$PWD\" | \"$SHLVL\" > OUT | \'$LESS\'");
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == 21);
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(tokens) == 14);
	int idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
			word_count++;
		if (((t_token *)node->content)->type == TOK_LESS || ((t_token *)node->content)->type == TOK_GREAT)
			redir_count++;
		if (((t_token *)node->content)->type == TOK_PIPE)
			pipe_count++;
		if (idx == 7)
			CHECK(strncmp(((t_token *)node->content)->token, env_pwd, strlen(env_pwd)) == 0);
		if (idx == 9)
			CHECK(strncmp(((t_token *)node->content)->token, env_shlvl, strlen(env_shlvl)) == 0);
		if (idx == 13)
			CHECK(strncmp(((t_token *)node->content)->token, "$LESS", 5) == 0);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}
