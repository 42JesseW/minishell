#include <catch.hpp>
#include <unittest.hpp>

class NormalizeFixture : public BaseFixture
{
protected:
	t_shell	*shell;

public:
	NormalizeFixture()
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
		REQUIRE(validate_pipes(tokens));
		REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
	}
};

TEST_CASE_METHOD(NormalizeFixture, "No normalisation"){
	char	cmd[] = "cat < Makefile";
	char	**split = ft_strsplit(cmd, ' ');
	t_list	*cur;

	init_tokens(cmd);
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
	cur = tokens;
	for (int idx = 0; split[idx]; idx++, cur = cur->next)
		CHECK(strncmp(((t_token *)cur->content)->token, split[idx], strlen(split[idx])) == 0);
	ft_strarrfree(&split);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-one") {
	char	*types[] = {"cat", "<", "Makefile", NULL};
	t_list	*cur;

	init_tokens("< Makefile cat");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-two") {
	char	*types[] = {"cat", "<", "Makefile", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("< Makefile > OUT cat");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 5);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-three") {
	char	*types[] = {"cat", "-e", ">", "OUT", "<", "Makefile", NULL};
	t_list	*cur;

	init_tokens("> OUT cat -e < Makefile");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 6);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-multiple-one") {
	char	*types[] = {"cat", "-e", "<", "Makefile", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("< Makefile > OUT cat -e");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 6);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-multiple-two") {
	char	*types[] = {"cat", "-e", "<", "Makefile", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("cat < Makefile > OUT -e");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 6);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-multiple-three") {
	char	*types[] = {"cat", "-e", "-b", "<", "Makefile", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("cat < Makefile > OUT -e -b");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 7);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-multiple-four") {
	char	*types[] = {"cat", "-e", "-b", "<", "Makefile", ">", "FILE1", ">", "FILE2", NULL};
	t_list	*cur;

	init_tokens("cat < Makefile -e > FILE1 -b > FILE2");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 9);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "simple-multiple-five") {
	char	*types[] = {"cat", "-e", "IN1", "IN2", "<", "IN4", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("cat < IN4 -e > OUT IN1 IN2");
	REQUIRE(ft_lstsize(tokens) == 8);
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 8);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "with pipes") {
	char	*types[] = {"cat", "<", "Makefile", "|", "cat", ">", "OUT", NULL};
	t_list	*cur;

	init_tokens("< Makefile cat | > OUT cat");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == 7);
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(NormalizeFixture, "multiple flags") {
	char	*types[] = {
			"cat", "-e", "-b", "<",
			"Makefile", ">", "FILE1",
			">", "FILE2", "|", "cat",
			"-e", ">", "OUT1", ">",
			"OUT2", NULL
	};
	t_list	*cur;

	init_tokens("cat < Makefile -e > FILE1 -b > FILE2 | > OUT1 > OUT2 cat -e");
	normalize(&tokens);
	REQUIRE(ft_lstsize(tokens) == ft_strarrlen(types));
	cur = tokens;
	for (int idx = 0; types[idx]; idx++, cur = cur->next)
		CHECK(strncmp(types[idx], ((t_token *)cur->content)->token, strlen(types[idx])) == 0);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}
