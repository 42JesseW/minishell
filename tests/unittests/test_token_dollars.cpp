#include <catch.hpp>
#include <unittest.hpp>

class CorrectDollarFixture : public BaseFixture
{

};

TEST_CASE_METHOD(CorrectDollarFixture, "No TOK_DOLLAR") {
	init_tokens("oh > my < lord |");
	int len = ft_lstsize(tokens);
	REQUIRE(correct_dollar(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
}

TEST_CASE_METHOD(CorrectDollarFixture, "A single TOK_DOLLAR followed by a TOK_WORD") {
	init_tokens("$word");
	int len = ft_lstsize(tokens);
	REQUIRE(correct_dollar(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
	int idx = 0;
	t_token	*token;
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token *)node->content;
		CHECK(token->type == (((idx % 2) == 0) ? TOK_DOLLAR : TOK_WORD));
	}
}

TEST_CASE_METHOD(CorrectDollarFixture, "A single TOK_DOLLAR followed by a space and then TOK_WORD") {
	init_tokens("$ word");
	int len = ft_lstsize(tokens);
	REQUIRE(correct_dollar(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
	int idx = 0;
	t_token	*token;
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token *)node->content;
		CHECK(token->type == (((idx % 2) == 1) ? TOK_SPACE : TOK_WORD));
	}
}

TEST_CASE_METHOD(CorrectDollarFixture, "Multiple combinations of the TOK_DOLLAR + TOK_SPACE and TOK_DOLLAR + TOK_WORD") {
	init_tokens("  $ word $word $  word $");
	int len = ft_lstsize(tokens);
	REQUIRE(correct_dollar(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
	int idx = 0;
	t_token	*token;
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token *)node->content;
		if (idx == 6)
			CHECK(token->type == TOK_DOLLAR);
		else
			CHECK(token->type != TOK_DOLLAR);
	}
}

TEST_CASE_METHOD(CorrectDollarFixture, "Syntax errors") {
	init_tokens("$$");
	REQUIRE(correct_dollar(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens("$\'\'");
	REQUIRE(correct_dollar(tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens("$\"\"");
}

class ResolveDollarsFixture : public BaseFixture
{
protected:
	t_shell	*shell;

public:
	ResolveDollarsFixture()
	{
		shell = shell_init(envp);
		REQUIRE(shell != NULL);
	}

	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
		remove_spaces(&tokens);
		REQUIRE(validate_syntax(tokens));
	}
};

TEST_CASE_METHOD(ResolveDollarsFixture, "No dollars") {
	init_tokens("> word | < word | > word");
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar no resolve") {
	init_tokens("$|$");
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	for (t_list *cur = tokens; cur != NULL; cur = cur->next)
		CHECK(((t_token *)cur->content)->type != TOK_DOLLAR);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "quoting") {
	init_tokens("\"$hasval\"");
	REQUIRE(ft_lstsize(tokens) == 4);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 3);
	ft_lstclear(&tokens, token_del);
	init_tokens("\"\'$hasval\'\"");
	REQUIRE(ft_lstsize(tokens) == 6);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 5);
	ft_lstclear(&tokens, token_del);
	init_tokens("\'$hasval\'");
	REQUIRE(ft_lstsize(tokens) == 4);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 4);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar resolve empty missing environ") {
	int	idx;

	init_tokens("> word | < $missing | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 1)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> $missing | < word | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 0)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> word | < word | > $missing");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 2)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar resolve empty environ no value") {
	int	idx;

	init_tokens("> word | < $noval | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 1)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> $noval | < word | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 0)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> word | < word | > $noval");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 2)
				CHECK(!(*((t_token *)node->content)->token));
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar resolve environ with value") {
	int			idx;
	const char	key[] = "hasval";
	const char	*val = environ_get(shell->environ, key);

	init_tokens("> word | < $hasval | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 1)
				CHECK(strncmp(((t_token *)node->content)->token, val, strlen(val)) == 0);
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> $hasval | < word | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 0)
				CHECK(strncmp(((t_token *)node->content)->token, val, strlen(val)) == 0);
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	init_tokens("> word | < word | > $hasval");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 8);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 2)
				CHECK(strncmp(((t_token *)node->content)->token, val, strlen(val)) == 0);
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "Everything multiple times") {
	int			idx;
	const char	key[] = "hasval";
	const char	*val = environ_get(shell->environ, key);

	init_tokens("$ word | $noval > $missing < $hasval << $hasval >> $missing   $noval $");
	REQUIRE(ft_lstsize(tokens) == 20);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 14);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 2 || idx == 3)
				CHECK(!(*((t_token *)node->content)->token));
			if (idx == 4 || idx == 5)
				CHECK(strncmp(((t_token *)node->content)->token, val, strlen(val)) == 0);
			idx++;
		}
		CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
}

TEST_CASE_METHOD(ResolveDollarsFixture, "Everything multiple times with quotes") {
	int			idx;
	const char	key[] = "hasval";
	const char	*val = environ_get(shell->environ, key);

	init_tokens("$ word | \"$noval\" > $missing < \"\'$hasval\'\" << $hasval >> \'$missing\'   $noval $");
	REQUIRE(ft_lstsize(tokens) == 28);
	REQUIRE(resolve_dollar(shell, &tokens, false) == 1);
	REQUIRE(ft_lstsize(tokens) == 23);
	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		if (((t_token *)node->content)->type == TOK_WORD)
		{
			if (idx == 4 || idx == 7)
				CHECK(!(*((t_token *)node->content)->token));
			if (idx == 11 || idx == 15)
				CHECK(strncmp(((t_token *)node->content)->token, val, strlen(val)) == 0);
		}
		if (idx == 18)
			CHECK(((t_token *)node->content)->type == TOK_DOLLAR);
		else
			CHECK(((t_token *)node->content)->type != TOK_DOLLAR);
	}
}


