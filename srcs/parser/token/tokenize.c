/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 16:24:04 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 16:24:04 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

/*
** A multiline command is defined as a command line
** where a single quote ' or a double quote " is not
** properly closed by a matching quote of the same kind.
*/
static bool	has_paired_quotes(const char *str)
{
	char	*ptr;
	int		idx;

	if (!str || !(*str))
		return (true);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '\'' || str[idx] == '\"')
		{
			ptr = ft_strchr(str + idx + 1, str[idx]);
			if (!ptr)
				return (false);
			if (!has_paired_quotes(ptr + 1))
				return (false);
			idx += (int)(ptr - str);
		}
		idx++;
	}
	return (true);
}

/*
** the t_lexer structure does not take ownership
** or dup input_string. It just needs to scan it
*/
static void	lexer_init(t_lexer *lexer, const char *input_string)
{
	lexer->input_string = input_string;
	lexer->string_len = ft_strlen(input_string);
	lexer->idx = 0;
}

/*
** correct_dollar() converts TOK_DOLLAR to TOK_WORD if
** it is not directly followed by a TOK_WORD. It also
** gives a PARSE_FAIL if the TOK_DOLLAR is succeeded by
** any token other than TOK_WORD or TOK_SPACE including
** QUOTE tokens since ANSI-C Quoting is not supported:
** https://www.gnu.org/software/bash/manual/html_node/ANSI_002dC-Quoting.html
** neither is BASHPID ($$)
** https://www.gnu.org/software/bash/manual/html_node/Bash-Variables.html
*/
int	correct_dollar(t_list *tokens)		// TODO testcase
{
	t_list	*node;
	t_token	*token;
	t_token	*next_token;

	node = tokens;
	while (node)
	{
		token = (t_token *)node->content;
		if (token->type == TOK_DOLLAR)
		{
			if (!node->next)
				token->type = TOK_WORD;
			else
			{
				next_token = ((t_token *)node->next->content);
				if (next_token->type != TOK_WORD)
				{
					if (next_token->type != TOK_SPACE)
						return (PARSE_FAIL);
					token->type = TOK_WORD;
				}
			}
		}
		node = node->next;
	}
	return (1);
}

/*
** tokenize() splits the input_string into small
** tokens adding a definition. input_string must
** be a non-empty string.
**
** It does the following:
**	1. multiline commands are not supported so
**	   this counts as a syntax error.
**	2. convert string components to tokens
**	3. convert single TOK_DOLLAR tokens to TOK_WORD
**  4. group tokens like > or < into << or >>
**
** The tokenizer may set shell.exit_code = 2 when a syntax
** error is found. // TODO discuss implementation of exit_code
*/

t_list	*tokenize(const char *input_string)
{
	t_list	*tokens;
	t_list	*node;
	t_token	*token;
	t_lexer	lexer;

	tokens = NULL;
	if (!(*input_string) || !has_paired_quotes(input_string))
		return (NULL);
	lexer_init(&lexer, input_string);
	while (lexer.idx < lexer.string_len)
	{
		token = get_next_token(&lexer);
		node = ft_lstnew(token);
		if (!token || !node)
		{
			free(token);
			ft_lstclear(&tokens, token_del);
			return (NULL);
		}
		ft_lstadd_back(&tokens, node);
	}
	if (!correct_dollar(tokens) || !redirs_merge(tokens))
		return (NULL);
	return (tokens);
}
