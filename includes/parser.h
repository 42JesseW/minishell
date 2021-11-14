#ifndef PARSER_H

# define PARSER_H

# include <libft.h>
# include <stdio.h>
# include <stdbool.h>

typedef enum e_token_type
{
	TOK_DOLLAR,
	TOK_QUOTE,
	TOK_SPACE,
	TOK_PIPE,
	TOK_LESS,
	TOK_GREAT,
	TOK_WORD,
	TOK_DLESS,
	TOK_DQUOTE,
	TOK_DGREAT
}	t_token_type;

typedef struct s_token_map
{
	t_token_type	type;
	char			*c;
}	t_token_map;

typedef struct s_token
{
	t_token_type	type;
	char			*token;
}	t_token;

typedef struct s_lexer
{
	const char	*input_string;
	size_t		string_len;
	size_t		idx;
}	t_lexer;

t_token	*get_next_token(t_lexer *lexer);
t_list	*tokenize(const char *input_string);	// TODO testcase

void	token_display_stdout(t_list *tokens);
bool	token_is_word_end(int c);

void	token_del(void *token);
t_token	*token_new_def(void);
t_token	*token_new_cpy(t_token *cpy);
t_token	*token_new_val(t_token_type type, char *token_string);
t_token	*token_new_word(t_lexer *lexer);

#endif
