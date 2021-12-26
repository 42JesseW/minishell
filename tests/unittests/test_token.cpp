#include <catch.hpp>
#include <unittest.hpp>

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
	t_list			*tokens;
	t_token			*token;
	int				idx = 0;
	char			input_string[] = "< > |  >>  <<  |";
	t_token_type	type;

	tokens = tokenize(input_string);
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == strlen(input_string));
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token*)node->content;
		if (idx < 5)
		{
			switch (idx) {
				case 0: 	type = TOK_LESS; break;
				case 2: 	type = TOK_GREAT; break;
				case 4:		type = TOK_PIPE; break;
				default:	type = TOK_SPACE; break ;
			}
		}
		else
		{
			if (idx == 15)
				type = TOK_PIPE;
			else if (idx >= 7 && idx <= 8)
				type = TOK_GREAT;
			else if (idx >= 11 && idx <= 12)
				type = TOK_LESS;
			else
				type = TOK_SPACE;
		}
		CHECK(token->type == type);
		CHECK(strncmp(token->token, input_string + idx, 1) == 0);
	}
}

TEST_CASE("tokenize string with everything in it") {
	t_list			*tokens;
	t_token			*token;
	t_token_type	type;
	char			**split;
	int				idx = 0;
	int				split_idx = 0;
	char			input_string[] = "one < > |  >> two <<  | three";

	tokens = tokenize(input_string);
	REQUIRE(tokens != NULL);
	split = ft_strsplit(input_string, ' ');
	REQUIRE(split != NULL);
	REQUIRE(ft_lstsize(tokens) == strlen(input_string) - 8);
	for (t_list *node = tokens; node != NULL; node = node->next, idx++)
	{
		token = (t_token*)node->content;
		if (idx < 7)
		{
			switch (idx) {
				case 0:		type = TOK_WORD; break;
				case 2: 	type = TOK_LESS; break;
				case 4: 	type = TOK_GREAT; break;
				case 6:		type = TOK_PIPE; break;
				default:	type = TOK_SPACE; break;
			}
		}
		else
		{
			if (idx == 18)
				type = TOK_PIPE;
			else if (idx == 12 || idx == 20)
				type = TOK_WORD;
			else if (idx >= 9 && idx <= 10)
				type = TOK_GREAT;
			else if (idx >= 14 && idx <= 15)
				type = TOK_LESS;
			else
				type = TOK_SPACE;
		}
		CHECK(token->type == type);
		if (idx == 0 || idx == 12 || idx == 20)
		{
			CHECK(strcmp(token->token, split[(split_idx == 0) ? 0 : (split_idx == 1) ? 5 : 8]) == 0);
			split_idx++;
		}
	}
}

TEST_CASE_METHOD(BaseFixture, "Nothing to be merged")
{
	init_tokens("> < > <");
	int	len = ft_lstsize(tokens);
	REQUIRE(redir_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
}

TEST_CASE_METHOD(BaseFixture, "Merge just one") {
	init_tokens(">>");
	int len = ft_lstsize(tokens);
	REQUIRE(redir_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 1);
}

TEST_CASE_METHOD(BaseFixture, "Merge multiple") {
	init_tokens(">> <<");
	int len = ft_lstsize(tokens);
	REQUIRE(redir_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(BaseFixture, "Merge some") {
	init_tokens("> << < >> <");
	int len = ft_lstsize(tokens);
	REQUIRE(redir_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(BaseFixture, "Merging with different tokens in string") {
	init_tokens("> << word | < | word >> <");
	int len = ft_lstsize(tokens);
	REQUIRE(redir_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(BaseFixture, "Syntax error because unknown tokens") {
	init_tokens("<>");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
    ft_lstclear(&tokens, token_del);
	init_tokens("><");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
    ft_lstclear(&tokens, token_del);
	init_tokens("<<<");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
    ft_lstclear(&tokens, token_del);
	init_tokens(">>>");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
    ft_lstclear(&tokens, token_del);
}

TEST_CASE_METHOD(BaseFixture, "Only spaces") {
	init_tokens("  ");
	REQUIRE(ft_lstsize(tokens) == 2);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 0);
}

TEST_CASE_METHOD(BaseFixture, "start, middle and end of string") {
	init_tokens(" > < ");
	REQUIRE(ft_lstsize(tokens) == 5);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 2);
}

TEST_CASE_METHOD(BaseFixture, "Combination of different tokens") {
	init_tokens(">>  <    LoL | < << <<  ");
	REQUIRE(redir_merge(tokens));
	REQUIRE(ft_lstsize(tokens) == 19);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 7);
}

TEST_CASE_METHOD(BaseFixture, "With single quotes") {
	init_tokens("\' \'");
	REQUIRE(ft_lstsize(tokens) == 3);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
}

TEST_CASE_METHOD(BaseFixture, "With double quotes") {
	init_tokens("\" \"");
	REQUIRE(ft_lstsize(tokens) == 3);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
}

TEST_CASE_METHOD(BaseFixture, "Combinations of everything") {
	init_tokens("\' \" $ lol > | <  $ epic \" \'");
	REQUIRE(ft_lstsize(tokens) == 22);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 22);
}

TEST_CASE_METHOD(BaseFixture, "Some inside, some outside") {
	init_tokens("outside > \" inside | \" < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(BaseFixture, "Single quote in double quote") {
	init_tokens("outside > \' inside \" \' < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(BaseFixture, "Multiple single quotes in double quote") {
	init_tokens("outside > \' inside \" \' < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(BaseFixture, "Multiple double quotes in single quote") {
	init_tokens("outside > \" inside \' \" < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

void	check_nodes(t_list *root, int check_idx, char *check_str, char *str[])
{
	t_list	*cur;
	int		idx;
	int		cur_idx;

	idx = 0;
	cur = root;
	cur_idx = 0;
	for (; str[idx]; cur = cur->next)
	{
		if (cur_idx == check_idx)
			CHECK(strncmp((char*)cur->content, check_str, strlen(check_str)) == 0);
		else
		{
			CHECK(strncmp((char*)cur->content, str[idx], strlen(str[idx])) == 0);
			idx++;
		}
		cur_idx++;
	}
}

TEST_CASE("ft_lstinsert / ft_lstunlink") {
	t_list	*list;
	t_list	*node;
	char	*str[] = {strdup("One"), strdup("Two"), strdup("three"), NULL};
	char	*s = strdup("NEW");

	list = NULL;
	for (int idx = 0; str[idx]; idx++)
	{
		node = ft_lstnew(str[idx]);
		REQUIRE(node != NULL);
		ft_lstadd_back(&list, node);
	}
	REQUIRE(ft_lstsize(list) == 3);
	node = ft_lstnew(s);
	REQUIRE(node != NULL);
	/* insert and unlink at start */
	ft_lstinsert(&list, node, 0);
	REQUIRE(ft_lstsize(list) == 4);
	check_nodes(list, 0, s, str);
	node = ft_lstunlink(&list, node);
	REQUIRE(node != NULL);
	REQUIRE(ft_lstsize(list) == 3);
	check_nodes(list, -1, NULL, str);
	/* insert and unlink in middle */
	ft_lstinsert(&list, node, 2);
	REQUIRE(ft_lstsize(list) == 4);
	check_nodes(list, 2, s, str);
	node = ft_lstunlink(&list, node);
	REQUIRE(node != NULL);
	REQUIRE(ft_lstsize(list) == 3);
	check_nodes(list, -1, NULL, str);
	/* insert and unlink at end */
	ft_lstinsert(&list, node, 3);
	REQUIRE(ft_lstsize(list) == 4);
	check_nodes(list, 3, s, str);
	check_nodes(list, 3, s, str);
	node = ft_lstunlink(&list, node);
	REQUIRE(node != NULL);
	REQUIRE(ft_lstsize(list) == 3);
	check_nodes(list, -1, NULL, str);
	ft_lstclear(&list, free);
	ft_lstdelone(node, free);
}

class ParseBeforeGroupFixture : public BaseFixture
{
protected:
	t_shell	*shell;

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
	ParseBeforeGroupFixture()
	{
		shell = shell_init(envp);
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
		REQUIRE(validate_syntax(tokens));
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

class GroupTokensFixture : public BaseFixture
{
protected:
	t_shell	*shell;

public:
	GroupTokensFixture()
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
		REQUIRE(resolve_dollar(shell, &tokens) != SYS_ERROR);
		REQUIRE(resolve_quotes(&tokens) != SYS_ERROR);
		normalize(&tokens);
	}
};

bool	ft_strarrcmp(char **arr1, char **arr2)
{
	int		idx;

	idx = 0;
	while (arr1[idx] && arr2[idx])
	{
		if (ft_strcmp(arr1[idx], arr2[idx]) != 0)
			return (false);
		idx++;
	}
	return (!arr1[idx] && !arr2[idx]);
}

TEST_CASE_METHOD(GroupTokensFixture, "Small edge cases") {
	init_tokens("  ");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	REQUIRE(shell->cmd_nodes == NULL);
	ft_lstclear(&tokens, token_del);
	init_tokens("\"\"<OUT2");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	REQUIRE(shell->cmd_nodes != NULL);
	REQUIRE(((t_node *)shell->cmd_nodes->content)->cmd[0] != NULL);
	REQUIRE(((t_node *)shell->cmd_nodes->content)->cmd[0][0] == 0);
	ft_lstclear(&tokens, token_del);
	ft_lstclear(&shell->cmd_nodes, node_del);
	init_tokens("<OUT2");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	REQUIRE(((t_node *)shell->cmd_nodes->content)->cmd == NULL);
	REQUIRE(ft_lstsize(((t_node *)shell->cmd_nodes->content)->redir) == 1);
	ft_lstclear(&tokens, token_del);
	ft_lstclear(&shell->cmd_nodes, node_del);
	init_tokens("cat<IN1|>OUT");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	REQUIRE(ft_lstsize(((t_node *)shell->cmd_nodes->content)->redir) == 1);
	REQUIRE(((t_node *)shell->cmd_nodes->next->content)->cmd == NULL);
	REQUIRE(ft_lstsize(((t_node *)shell->cmd_nodes->next->content)->redir) == 1);
	ft_lstclear(&tokens, token_del);
	ft_lstclear(&shell->cmd_nodes, node_del);
	init_tokens("<IN1|>OUT");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	REQUIRE(((t_node *)shell->cmd_nodes->content)->cmd == NULL);
	REQUIRE(ft_lstsize(((t_node *)shell->cmd_nodes->content)->redir) == 1);
	REQUIRE(((t_node *)shell->cmd_nodes->next->content)->cmd == NULL);
	REQUIRE(ft_lstsize(((t_node *)shell->cmd_nodes->next->content)->redir) == 1);
	ft_lstclear(&tokens, token_del);
	ft_lstclear(&shell->cmd_nodes, node_del);
}

TEST_CASE_METHOD(GroupTokensFixture, "One simple node") {
	t_node	*node;
	t_redir	*redir;
	char	**cmd;

	init_tokens("cat < Makefile");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	for (t_list *cmd_node = shell->cmd_nodes; cmd_node != NULL; cmd_node = cmd_node->next)
	{
		node = (t_node *)cmd_node->content;
		cmd = ft_strsplit("cat", ' ');
		REQUIRE(cmd != NULL);
		REQUIRE(ft_strarrcmp(node->cmd, cmd));
		REQUIRE(ft_lstsize(node->redir) == 1);
		for (t_list *redir_node = node->redir; redir_node != NULL; redir_node = redir_node->next)
		{
			redir = (t_redir *)redir_node->content;
			CHECK(redir->type == REDIR_IN);
		}
		ft_strarrfree(&cmd);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(GroupTokensFixture, "One complex node") {
	t_node			*node;
	t_redir			*redir;
	char			**cmd;
	int				idx;
	t_redir_type	types[] = {REDIR_IN, REDIR_OUT, REDIR_IN, REDIR_OUT};

	init_tokens("cat -e -b -s < IN1 > OUT1 < IN2 > OUT2");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	for (t_list *cmd_node = shell->cmd_nodes; cmd_node != NULL; cmd_node = cmd_node->next)
	{
		node = (t_node *)cmd_node->content;
		cmd = ft_strsplit("cat -e -b -s", ' ');
		REQUIRE(cmd != NULL);
		REQUIRE(ft_strarrcmp(node->cmd, cmd));
		REQUIRE(ft_lstsize(node->redir) == 4);
		idx = 0;
		for (t_list *redir_node = node->redir; redir_node != NULL; redir_node = redir_node->next, idx++)
		{
			redir = (t_redir *)redir_node->content;
			CHECK(redir->type == types[idx]);
		}
		ft_strarrfree(&cmd);
	}
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(GroupTokensFixture, "Two simple nodes") {
	t_node			*node;
	t_redir			*redir;
	char			**cmd;
	int				i, j;
	int				list_sizes[2] = {1, 1};
	char			*input_strings[] = {strdup("cat"), strdup("cat -e"), NULL};
	t_redir_type	types[2][1] = {{REDIR_IN}, {REDIR_OUT}};

	init_tokens("cat < Makefile | cat -e > OUT");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	i = 0;
	for (t_list *cmd_node = shell->cmd_nodes; cmd_node != NULL; cmd_node = cmd_node->next, i++)
	{
		node = (t_node *)cmd_node->content;
		cmd = ft_strsplit(input_strings[i], ' ');
		REQUIRE(cmd != NULL);
		REQUIRE(ft_strarrcmp(node->cmd, cmd));
		REQUIRE(ft_lstsize(node->redir) == list_sizes[i]);
		j = 0;
		for (t_list *redir_node = node->redir; redir_node != NULL; redir_node = redir_node->next, j++)
		{
			redir = (t_redir *)redir_node->content;
			CHECK(redir->type == types[i][j]);
		}
		ft_strarrfree(&cmd);
	}
	for (int idx = 0; input_strings[idx]; idx++)
		free(input_strings[idx]);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(GroupTokensFixture, "Two complex nodes") {
	t_node			*node;
	t_redir			*redir;
	char			**cmd;
	int				i, j;
	int				list_sizes[2] = {4, 0};
	char			*input_strings[] = {strdup("cat -e -b -s"), strdup("grep -A 2 -B 4 -n test"), NULL};
	t_redir_type	types[2][4] = {{REDIR_IN, REDIR_OUT, REDIR_IN, REDIR_OUT}, {}};

	init_tokens("cat -e -b -s < IN1 > OUT1 < IN2 > OUT2 | grep -A 2 -B 4 -n 'test'");
	REQUIRE(group_tokens(shell, &tokens) != SYS_ERROR);
	i = 0;
	for (t_list *cmd_node = shell->cmd_nodes; cmd_node != NULL; cmd_node = cmd_node->next, i++)
	{
		node = (t_node *)cmd_node->content;
		cmd = ft_strsplit(input_strings[i], ' ');
		REQUIRE(cmd != NULL);
		REQUIRE(ft_strarrcmp(node->cmd, cmd));
		REQUIRE(ft_lstsize(node->redir) == list_sizes[i]);
		j = 0;
		for (t_list *redir_node = node->redir; redir_node != NULL; redir_node = redir_node->next, j++)
		{
			redir = (t_redir *)redir_node->content;
			CHECK(redir->type == types[i][j]);
		}
		ft_strarrfree(&cmd);
	}
	for (int idx = 0; input_strings[idx]; idx++)
		free(input_strings[idx]);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}
