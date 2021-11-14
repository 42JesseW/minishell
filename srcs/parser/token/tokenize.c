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
** tokenize() splits the input_string into
** small tokens adding a definition. input_string
** must be a non-empty string.
**
** The tokenizer may set shell.exit_code = 2 when a syntax
** error is found. // TODO discuss implementation of exit_code
*/

t_list	*tokenize(const char *input_string)
{
	t_list	*root;
	t_list	*node;
	t_token	*token;
	t_lexer	lexer;

	root = NULL;
	if (!(*input_string))
		return (root);
	lexer_init(&lexer, input_string);
	while (lexer.idx < lexer.string_len)
	{
		token = get_next_token(&lexer);
		node = ft_lstnew(token);
		if (!token || !node)
		{
			free(token);
			ft_lstclear(&root, token_del);
			return (NULL);
		}
		ft_lstadd_back(&root, node);
	}
	return (root);
}
