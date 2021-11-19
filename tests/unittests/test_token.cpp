#include <catch.hpp>

extern "C" {
	#include <minishell.h>
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

class BaseFixture
{
protected:
	t_list	*tokens;

public:
	BaseFixture() : tokens(NULL) {}
	virtual ~BaseFixture() { ft_lstclear(&tokens, token_del); }

	virtual void	init_tokens(const char *input_string) { tokens = tokenize(input_string); }
};

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

static const char	*envp[] = {
	"__INTELLIJ_COMMAND_HISTFILE__=/Users/jevan-de/Library/Application Support/JetBrains/CLion2020.2/terminal/history/history-25",
	"HOME=/Users/jevan-de",
	"ZDOTDIR=",
	"__CF_USER_TEXT_ENCODING=0x10BA2:0x0:0x0",
	"LOGIN_SHELL=1",
	"PATH=/Users/jevan-de/.brew/bin:/Users/jevan-de/.brew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki",
	"USER=jevan-de",
	"SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.vIgSz1Rghr/Listeners",
	"TMPDIR=/var/folders/zz/zyxvpxvq6csfxvn_n0008bm80022x2/T/",
	"SHELL=/bin/zsh",
	"LOGNAME=jevan-de",
	"XPC_SERVICE_NAME=0",
	"LC_CTYPE=en_US.UTF-8",
	"Apple_PubSub_Socket_Render=/private/tmp/com.apple.launchd.ziwpMyIfRM/Render",
	"XPC_FLAGS=0x0",
	"TERM=xterm-256color",
	"TERMINAL_EMULATOR=JetBrains-JediTerm",
	"SHLVL=1",
	"PWD=/Users/jevan-de/Documents/projects/minishell/shelly/tests",
	"OLDPWD=/Users/jevan-de/Documents/projects/minishell/shelly",
	"ZSH=/Users/jevan-de/.oh-my-zsh",
	"PAGER=less",
	"LESS=-R",
	"LSCOLORS=Gxfxcxdxbxegedabagacad",
	"noval=",
	"hasval=val",
	"_=/usr/bin/env",
};

class ResolveDollarsFixture : public BaseFixture
{
protected:
	t_shell	*shell;
	char	*_input_string;

public:
	ResolveDollarsFixture() : _input_string(NULL)
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
	}
};

TEST_CASE_METHOD(ResolveDollarsFixture, "No dollars") {
	init_tokens("> word | < word | > word");
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar no resolve") {
	init_tokens("$|$");
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "quoting") {
	const char	key[] = "hasval";
	const char	*val = environ_get(shell->environ, key);

	init_tokens("\"$hasval\"");
	REQUIRE(ft_lstsize(tokens) == 4);
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
	REQUIRE(ft_lstsize(tokens) == 3);
	ft_lstclear(&tokens, token_del);
	init_tokens("\"\'$hasval\'\"");
	REQUIRE(ft_lstsize(tokens) == 6);
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
	REQUIRE(ft_lstsize(tokens) == 5);
	ft_lstclear(&tokens, token_del);
	init_tokens("\'$hasval\'");
	REQUIRE(ft_lstsize(tokens) == 4);
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
	REQUIRE(ft_lstsize(tokens) == 4);
	ft_lstclear(&tokens, token_del);
	shell_destroy(&shell);
}

TEST_CASE_METHOD(ResolveDollarsFixture, "One dollar resolve empty missing environ") {
	int	idx;

	init_tokens("> word | < $missing | > word");
	REQUIRE(ft_lstsize(tokens) == 9);
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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
	REQUIRE(resolve_dollar(shell, &tokens) == 1);
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

}

TEST_CASE_METHOD(ResolveQuotesFixture, "one double quoted string") {

}
