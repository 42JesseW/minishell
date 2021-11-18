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

TEST_CASE("tokenize multi line command single quotes") {

}

TEST_CASE("tokenize command line with unknown tokens") {

}

TEST_CASE("valid tokenization strings") {
	t_list *tokens = tokenize("\"word\"|\"word\" word");
	REQUIRE(tokens != NULL);
}

static void	lexer_init(t_lexer *lexer, const char *input_string)
{
	lexer->input_string = input_string;
	lexer->string_len = ft_strlen(input_string);
	lexer->idx = 0;
}

class MergeRedirsFixture
{
protected:
	t_list	*tokens;

public:
	MergeRedirsFixture() : tokens(NULL) {}
	virtual ~MergeRedirsFixture() { ft_lstclear(&tokens, token_del); }

	virtual void	init_tokens(const char *input_string) { tokens = tokenize(input_string); }
};

TEST_CASE_METHOD(MergeRedirsFixture, "Nothing to be merged")
{
	init_tokens("> < > <");
	int	len = ft_lstsize(tokens);
	REQUIRE(redirs_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len);
}

TEST_CASE_METHOD(MergeRedirsFixture, "Merge just one") {
	init_tokens(">>");
	int len = ft_lstsize(tokens);
	REQUIRE(redirs_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 1);
}

TEST_CASE_METHOD(MergeRedirsFixture, "Merge multiple") {
	init_tokens(">> <<");
	int len = ft_lstsize(tokens);
	REQUIRE(redirs_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(MergeRedirsFixture, "Merge some") {
	init_tokens("> << < >> <");
	int len = ft_lstsize(tokens);
	REQUIRE(redirs_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(MergeRedirsFixture, "Merging with different tokens in string") {
	init_tokens("> << word | < | word >> <");
	int len = ft_lstsize(tokens);
	REQUIRE(redirs_merge(tokens));
	REQUIRE(tokens != NULL);
	REQUIRE(ft_lstsize(tokens) == len - 2);
}

TEST_CASE_METHOD(MergeRedirsFixture, "Syntax error because unknown tokens") {
	init_tokens("<>");
	REQUIRE(redirs_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	MergeRedirsFixture::~MergeRedirsFixture();
	init_tokens("><");
	REQUIRE(redirs_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	MergeRedirsFixture::~MergeRedirsFixture();
	init_tokens("<<<");
	REQUIRE(redirs_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	MergeRedirsFixture::~MergeRedirsFixture();
	init_tokens(">>>");
	REQUIRE(redirs_merge(tokens) == PARSE_FAIL);
	REQUIRE(tokens != NULL);
	MergeRedirsFixture::~MergeRedirsFixture();
}

class CorrectDollarFixture : public MergeRedirsFixture
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

class RemoveSpacesFixture : public MergeRedirsFixture
{

};

TEST_CASE_METHOD(RemoveSpacesFixture, "Only spaces") {
	init_tokens("  ");
	REQUIRE(ft_lstsize(tokens) == 2);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 0);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "start, middle and end of string") {
	init_tokens(" > < ");
	REQUIRE(ft_lstsize(tokens) == 5);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 2);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Combination of different tokens") {
	init_tokens(">>  <    LoL | < << <<  ");
	REQUIRE(redirs_merge(tokens));
	REQUIRE(ft_lstsize(tokens) == 19);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 7);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "With single quotes") {
	init_tokens("\' \'");
	REQUIRE(ft_lstsize(tokens) == 3);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "With double quotes") {
	init_tokens("\" \"");
	REQUIRE(ft_lstsize(tokens) == 3);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 3);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Combinations of everything") {
	init_tokens("\' \" $ lol > | <  $ epic \" \'");
	REQUIRE(ft_lstsize(tokens) == 22);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 22);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Some inside, some outside") {
	init_tokens("outside > \" inside | \" < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Single quote in double quote") {
	init_tokens("outside > \' inside \" \' < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Multiple single quotes in double quote") {
	init_tokens("outside > \' inside \" \' < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

TEST_CASE_METHOD(RemoveSpacesFixture, "Multiple double quotes in single quote") {
	init_tokens("outside > \" inside \' \" < outside");
	REQUIRE(ft_lstsize(tokens) == 15);
	remove_spaces(&tokens);
	REQUIRE(ft_lstsize(tokens) == 11);
}

class EvaluateTokensFixture : public MergeRedirsFixture
{
public:
	void	init_tokens(const char *input_string) override {
		tokens = tokenize(input_string);
		REQUIRE(tokens != NULL);
		REQUIRE(redirs_merge(tokens));
		REQUIRE(correct_dollar(tokens));
	}
};

TEST_CASE_METHOD(EvaluateTokensFixture, "No pipes") {
	init_tokens("> word <");
	REQUIRE(evaluate(&tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "One pipe") {
	init_tokens("> wo | rd <");
	REQUIRE(evaluate(&tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "Multiple pipes") {
	init_tokens("> word | < word | > word");
	REQUIRE(evaluate(&tokens));
}

TEST_CASE_METHOD(EvaluateTokensFixture, "Valid pipes") {
	init_tokens("\"word\"|\"word\" word");
	REQUIRE(evaluate(&tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("\'word\'|\'word\' word");
	REQUIRE(evaluate(&tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$word|$");
	REQUIRE(evaluate(&tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$|$word");
	REQUIRE(evaluate(&tokens));
	ft_lstclear(&tokens, token_del);
	init_tokens("$|$");
	REQUIRE(evaluate(&tokens));
	ft_lstclear(&tokens, token_del);
}

TEST_CASE_METHOD(EvaluateTokensFixture, "invalid pipes") {
	init_tokens("||");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" | | ");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word > |");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word < |");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word >> |");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
	init_tokens(" word | word << |");
	REQUIRE(evaluate(&tokens) == PARSE_FAIL);
	ft_lstclear(&tokens, token_del);
}

