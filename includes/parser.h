/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jessevanderwolf <jessevanderwolf@student...  +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/07 13:50:16 by jessevander...#+#    #+#                 */
/*   Updated: 2022/01/07 13:50:16 by jessevander...########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H

# define PARSER_H

# include <stdio.h>
# include <stdbool.h>
# include <libft.h>

# define REDIR_ALL 0
# define REDIR_SINGLE 1
# define HEREDOC_FILE "/.shelly_heredoc.tmp"

# ifndef SYS_ERROR
#  define SYS_ERROR -1
# endif
# define PARSE_FAIL 0

typedef enum e_token_type
{
	TOK_LESS,
	TOK_GREAT,
	TOK_DGREAT,
	TOK_DLESS,
	TOK_DOLLAR,
	TOK_QUOTE,
	TOK_SPACE,
	TOK_PIPE,
	TOK_WORD,
	TOK_DQUOTE,
	TOK_MERGE,
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

bool	has_paired_quotes(const char *str);
bool	is_redir_type(t_token_type type, bool check_single);

void	normalize(t_list **tokens);
int		redir_merge(t_list *tokens);
int		validate_syntax(t_list *tokens);
int		correct_dollar(t_list *tokens);
int		correct_dollar_heredoc(t_list *tokens);
int		resolve_quotes(t_list **tokens);

t_token	*get_next_token(t_lexer *lexer);
t_list	*tokenize(const char *input_string);

void	token_display_stdout(t_list *tokens);
bool	token_is_word_end(int c);

int		remove_spaces(t_list **tokens);
void	token_del(void *token);

t_token	*token_new_def(void);
t_token	*token_new_cpy(t_token *cpy);
t_token	*token_new_val(t_token_type type, char *token_string);
t_token	*token_new_word(t_lexer *lexer);

typedef struct s_quote
{
	bool			between;
	t_token_type	type;
}	t_quote;

bool	quote_is_type(bool match_type, t_quote *quote, t_token_type type);

typedef struct s_bound
{
	t_list	*prev;
	t_list	*start;
	t_list	*end;
	t_list	*next;
}	t_bound;

#endif
