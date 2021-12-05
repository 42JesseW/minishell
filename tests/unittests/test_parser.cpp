#include <catch.hpp>
#include <unittest.hpp>

extern "C" {
	#include <minishell.h>
}

class ConvertHereDocFixture : public BaseFixture
{
protected:
	t_shell	*shell;

public:
	ConvertHereDocFixture()
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
		normalize(&tokens);
		REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	}
};

TEST_CASE_METHOD(ConvertHereDocFixture, "One HereDoc") {
	init_tokens("cat << EOF");
	nodes_print_stdout(shell->cmd_nodes);
	ft_lstclear(&shell->cmd_nodes, node_del);
	ft_lstclear(&tokens, token_del);
}
