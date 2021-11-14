/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_new.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jevan-de <jevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/11/14 14:37:50 by jevan-de      #+#    #+#                 */
/*   Updated: 2021/11/14 14:37:50 by jevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>

t_token	*token_new_def(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = TOK_DOLLAR;
	token->token = NULL;
	return (token);
}

t_token	*token_new_cpy(t_token *cpy)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = cpy->type;
	token->token = NULL;
	if (cpy->token)
		token->token = ft_strdup(cpy->token);
	if (cpy->token && !token->token)
	{
		token_del(token);
		return (NULL);
	}
	return (token);
}

t_token	*token_new_val(t_token_type type, char *token_string)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->token = NULL;
	if (token_string)
		token->token = ft_strdup(token_string);
	if (token_string && !token->token)
	{
		token_del(token);
		return (NULL);
	}
	return (token);
}

/* wrapper for token_new_val to read a whole word from the lexer */
t_token	*token_new_word(t_lexer *lexer)
{
	char	*token_string;
	t_token	*token;
	size_t	len;

	if (!lexer)
		return (NULL);
	len = 0;
	while (!token_is_word_end(lexer->input_string[lexer->idx]))
	{
		lexer->idx++;
		len++;
	}
	token_string = ft_strndup(lexer->input_string + (lexer->idx - len), len);
	if (!token_string)
		return (NULL);
	token = token_new_val(TOK_WORD, token_string);
	free(token_string);
	return (token);
}
