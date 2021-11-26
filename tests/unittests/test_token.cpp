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
	BaseFixture::~BaseFixture();
	init_tokens("><");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	BaseFixture::~BaseFixture();
	init_tokens("<<<");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	BaseFixture::~BaseFixture();
	init_tokens(">>>");
	REQUIRE(redir_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	BaseFixture::~BaseFixture();
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

class EvaluateTokensFixture : public BaseFixture
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

TEST_CASE_METHOD(EvaluateTokensFixture, "No pipes") {
	init_tokens("> word <");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "One pipe") {
	init_tokens("> wo | rd <");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "Multiple pipes") {
	init_tokens("> word | < word | > word");
	REQUIRE(validate_pipes(tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "Valid pipes") {
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

TEST_CASE_METHOD(EvaluateTokensFixture, "invalid pipes") {
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
