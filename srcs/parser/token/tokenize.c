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
** tokenize() splits the input_string into small
** tokens adding a definition. input_string must
** be a non-empty string.
**
** It does the following:
**	1. multiline commands are not supported so
**	   this counts as a syntax error.
**	2. convert string components to tokens
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
	return (tokens);
}
