#include <catch.hpp>
#include <unittest.hpp>

class WordSplittingFixture : public BaseFixture
{
protected:
	t_shell * shell;

public:
	WordSplittingFixture()
	{
		shell = shell_init(envp);
		REQUIRE(shell != NULL);
	}

	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redir_merge(tokens));
		REQUIRE(correct_dollar(tokens));
	}
};

TEST_CASE_METHOD(WordSplittingFixture, "Do not add any tokens") {
	init_tokens("echo test");
	insert_merge_token(&tokens);
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type != TOK_MERGE);
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Do not add any tokens with quotes") {
	init_tokens(R"(echo "test")");
	insert_merge_token(&tokens);
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type != TOK_MERGE);
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Add one token same quote type") {
	init_tokens(R"(echo "one""two")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 5)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - One") {
	init_tokens(R"("")");
	insert_merge_token(&tokens);
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type != TOK_MERGE);
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Two") {
	init_tokens(R"(''"one"'')");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 2 || idx == 6)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Three") {
	init_tokens(R"(export TEST="cat")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 3)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Four") {
	init_tokens(R"(export TEST= "cat")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Five") {
	init_tokens(R"(echo "$PWD"$SHLVL)");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 6)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Six") {
	init_tokens(R"(echo "$PWD"SHLVL)");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 6)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Seven") {
	init_tokens(R"(''""''"")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 2 || idx == 5 || idx == 8)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Eight") {
	init_tokens(R"(echo one$USER)");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 3)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Edge cases - Nine") {
	init_tokens(R"(echo one$USER$PWD)");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 3 || idx == 6)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Add two token same quote type") {
	init_tokens(R"(echo "one""two""three")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 5 || idx == 9)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Add two token different quote type") {
	init_tokens(R"(echo "one"'two'"three")");
	insert_merge_token(&tokens);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 5 || idx == 9)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Combine with other parse functions - One") {
	init_tokens(R"(echo "one"'two'"three")");
	insert_merge_token(&tokens);
	remove_spaces(&tokens);
	REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 4 || idx == 8)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Combine with other parse functions - Two") {
	init_tokens(R"(echo "one"'two'"three" 'four' "five"'six' "$PWD")");
	insert_merge_token(&tokens);
	remove_spaces(&tokens);
	REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
	int	idx = 0;
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		if (idx == 4 || idx == 8 || idx == 18)
			CHECK(token->type == TOK_MERGE);
		else
			CHECK(token->type != TOK_MERGE);
		idx++;
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Resolve quotes - One") {
	init_tokens(R"(echo "one"'two'"three" 'four' "five"'six' "$PWD")");
	insert_merge_token(&tokens);
	remove_spaces(&tokens);
	REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(tokens) == 5);
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type == TOK_WORD);
	}
}

TEST_CASE_METHOD(WordSplittingFixture, "Resolve quotes - Two") {
	init_tokens(R"(export TEST="cat")");
	insert_merge_token(&tokens);
	remove_spaces(&tokens);
	REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
	REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(tokens) == 2);
	for (t_list *node = tokens; node != NULL; node = node->next)
	{
		auto	*token = (t_token *)node->content;
		CHECK(token->type == TOK_WORD);
	}
}
